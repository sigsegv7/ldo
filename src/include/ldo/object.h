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

#ifndef OBJECT_H_
#define OBJECT_H_

#include <sys/queue.h>
#include <stddef.h>

/* Pin it here, can be adjusted (XXX: careful!) */
#define OBJQ_MAXCAP 1024

/* Default cap */
#define OBJQ_CAP 512

/*
 * Represents "static array" objects to be
 * queued up before being injected into its
 * resulting file's .static_array section.
 *
 * @pathname: Object file pathname.
 * @cdata: Compressed data buffer.
 * @size: Size of compressed data.
 * @real_size: Size of data when decompressed.
 */
struct sarry_obj {
    const char *name;
    const char *cdata;
    size_t size;
    size_t real_size;
    TAILQ_ENTRY(sarry_obj) link;
};

/*
 * Represents a "static array" object queue
 * which holds one or more "static array"
 * objects. Once filled, all compressed objects
 * are injected into a final ELF.
 *
 * @q: TAILQ head.
 * @count: Number of objects.
 * @cap: Object queue capacity.
 */
struct sarry_objq {
    TAILQ_HEAD(, sarry_obj) q;
    size_t count;
    size_t cap;
};

int sarry_init_objq(struct sarry_objq *qp, size_t cap);
int sarry_objq_in(struct sarry_objq *qp, struct sarry_obj *op);
int sarry_objq_flush(struct sarry_objq *qp, struct sarry_obj *op);

#endif  /* !OBJECT_H_ */
