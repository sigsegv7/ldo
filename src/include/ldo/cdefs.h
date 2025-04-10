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

#ifndef CDEFS_H_
#define CDEFS_H_

#if defined(_HYRA_SOURCE)
#include <sys/cdefs.h>
#else
#undef __weak
#define __weak          __attribute__((__weak__))
#undef __always_inline
#define __always_inline __attribute__((__always_inline__))
#undef __packed
#define __packed        __attribute__((__packed__))
#undef __dead
#define __dead          __attribute__((__noreturn__))
#undef __cold
#define __cold          __attribute__((__cold__))
#undef __dead_cold
#define __dead_cold     __attribute__((__noreturn__, __cold__))
#undef __unused
#define __unused        __attribute__((__unused__))
#undef __nothing
#define __nothing       ((void)0)
#undef __likely
#define __likely(exp)   __builtin_expect(((exp) != 0), 1)
#undef __unlikely
#define __unlikely(exp) __builtin_expect(((exp) != 0), 0)
#undef __static_assert
#define __static_assert _Static_assert
#endif  /* _HYRA_SOURCE */

#endif  /* !CDEFS_H_ */
