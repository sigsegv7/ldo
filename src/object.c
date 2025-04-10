/*
 * Copyright (c) 2025 Ian Marco Moffett and the Osmora Team.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Hyra nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/queue.h>
#include <sys/errno.h>
#include <ldo/object.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

/*
 * Initialize an object queue.
 *
 * @qp: Object queue pointer.
 * @cap: Capacity
 *
 * XXX: `cap' must be a power-of-two and must not exceed
 *      OBJQ_MAXCAP.
 */
int
sarry_init_objq(struct sarry_objq *qp, size_t cap)
{
    uint8_t e;

    /*
     * Power-of-two sizes are good for block
     * based processing
     */
    if ((OBJQ_MAXCAP & 1) != 0) {
        fprintf(stderr, "OBJQ_MAXCAP is not a power of two\n");
        return -EINVAL;
    }

    /* Ensure `cap' doesn't exceed the limit */
    if (cap > OBJQ_MAXCAP) {
        e = log2(OBJQ_MAXCAP);
        fprintf(stderr, "cap exceeds OBJQ_MAXCAP, rejecting...\n");
        fprintf(stderr, "cap must be <= 2^%d (%d) entries\n", e, cap);
        return -EINVAL;
    }

    TAILQ_INIT(&qp->q);
    qp->count = 0;
    qp->cap = cap;
}

/*
 * Insert an object into an object queue.
 *
 * @qp: Object queue pointer.
 * @op: Object pointer.
 */
int
sarry_objq_in(struct sarry_objq *qp, struct sarry_obj *op)
{
    size_t new_count;

    new_count = (qp->count + 1);
    if (new_count > OBJQ_MAXCAP) {
        fprintf(stderr, "sarry_insert: object queue full\n");
        return -ENOSPC;
    }

    TAILQ_INSERT_TAIL(&qp->q, op, link);
    ++qp->count;
    return 0;
}
