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

#include <stdio.h>
#include <unistd.h>
#include <ldo/file.h>
#include <ldo/ldo.h>

static ldo_flags_t flags = 0;

static void
usage(const char *argv0)
{
    fprintf(stderr, "Usage: %s <*.oo>\n", argv0);
}

/*
 * Get linker runtime flags
 */
ldo_flags_t
ldo_rtflags(void)
{
    return flags;
}

int
main(int argc, char **argv)
{
    char c;
    int i;

    if (argc < 2) {
        usage(argv[0]);
        return -1;
    }

    while ((c = getopt(argc, argv, "hv")) >= 0) {
        switch (c) {
        case 'h':
            usage(argv[0]);
            return 0;
        case 'v':
            flags |= LDO_F_VERBOSE;
            break;
        case '?':
            fprintf(stderr, "Bad argument: -%c\n", optopt);
            break;
        }
    }

    ldo_init();

    /* Load object files */
    if (optind < argc) {
        for (i = optind; i < argc; ++i) {
            ldo_load(argv[i]);
        }
    }
    return 0;
}
