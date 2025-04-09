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

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ldo/file.h>

/*
 * Open a file and return an LDO file
 * handle.
 *
 * @filename: Path of file.
 * @flags: O_*
 */
struct ldo_file *
ldo_open(const char *filename, int flags)
{
    struct stat sb;
    struct ldo_file *lfp = NULL;
    struct ldo_buffer *bp;
    size_t file_size;
    int retval;

    if (lstat(filename, &sb) < 0) {
        fprintf(stderr, "failed to stat '%s'\n", filename);
        perror("lstat");
        return NULL;
    }

    if ((lfp = malloc(sizeof(*lfp))) == NULL) {
        fprintf(stderr, "lfp malloc failure (open %s)\n", filename);
        return NULL;
    }

    if ((retval = open(filename, flags)) < 0) {
        fprintf(stderr, "failed to open %s\n", filename);
        perror("open");
        free(lfp);
        return NULL;
    }

    lfp->fd = retval;
    lfp->file_size = sb.st_size;
    lfp->data = ldo_allocz(lfp->file_size);

    if (lfp->data == NULL) {
        close(lfp->fd);
        free(lfp);
        return NULL;

    }

    bp = lfp->data;
    read(lfp->fd, bp->data, lfp->file_size);
    return lfp;
}

void
ldo_close(struct ldo_file *lfp)
{
    close(lfp->fd);
    ldo_free(lfp->data);
    free(lfp);
}
