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

#include <stdlib.h>
#include <ldo/buffer.h>

/*
 * Allocate a new zero-filled LDO buffer
 *
 * @len: Length of buffer
 */
struct ldo_buffer *
ldo_allocz(size_t len)
{
    struct ldo_buffer *bp;

    /* Verify arguments */
    if (len == 0)
        return NULL;
    if ((bp = malloc(sizeof(*bp))) == NULL)
        return NULL;

    bp->data = calloc(len, 1);
    if (bp->data == NULL) {
        free(bp);
        return NULL;
    }
    bp->len = len;
    return bp;
}

/*
 * Free an LDO buffer.
 *
 * @bp: Pointer to LDO buffer.
 */
void
ldo_free(struct ldo_buffer *bp)
{
    /* Bad pointers? */
    if (bp == NULL)
        return;
    if (bp->data == NULL)
        return;

    free(bp->data);
    free(bp);
}

/*
 * Resize an LDO buffer.
 *
 * @bp: Pointer to LDO buffer.
 * @new_len: New length to set.
 */
void
ldo_realloc(struct ldo_buffer *bp, size_t new_len)
{
    /* Bad pointers? */
    if (bp == NULL)
        return;
    if (bp->data == NULL)
        return;

    /* Length cannot be zero */
    if (new_len == 0)
        return;

    bp->data = realloc(bp->data, new_len);
    bp->len = new_len;
}
