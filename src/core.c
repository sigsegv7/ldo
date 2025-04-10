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

#include <sys/errno.h>
#include <stdint.h>
#include <stdio.h>
#include <ldo/ldo.h>
#include <ldo/file.h>
#include <ldo/elf.h>
#include <ldo/object.h>
#include <ldo/cdefs.h>
#include <lz4.h>

/*
 * Too many defines for one arch, just simplify
 * it and use __PPC64__, fucking hell.
 */
#if defined(__ppc64__) || defined(_ARCH_PPC64)
#if !defined(__PPC64__)
#define __PPC64__
#endif  /* !defined(__PPC64__) */
#endif  /* defined(__ppc64__) || ... */

static struct sarry_objq objq;

/*
 * Machine string map, LDO machine defines
 * are mapped one-to-one.
 */
static const char *machstrmap[] = {
    [LDO_X86_64] = "x86_64",
    [LDO_AARCH64] = "aarch64",
    [LDO_PPC64] = "ppc64",
    [LDO_UNKNOWN] = "unknown"
};

__always_inline
static inline const char *
ldo_machstr(ldo_mach_t type)
{
    if (type > LDO_UNKNOWN)
        type = LDO_UNKNOWN;

    return machstrmap[type];
}

/*
 * Returns an LDO machine type for the
 * current platform.
 */
__always_inline static inline ldo_mach_t
getmach(void)
{
#if defined(__x86_64__) || defined(_M_X64)
    return LDO_X86_64;
#elif defined(__aarch64__) || defined(_M_ARM64)
    return LDO_AARCH64;
#elif defined(__PPC64__)
    return LDO_PPC64;
#else
    /* At this point bail the hell out */
    printf("Unknown machine... please run machchk.sh\n");
    return LDO_UNKNOWN;
#endif
}

/*
 * Returns an LDO machine type for the target
 * ELF platform.
 */
static inline ldo_mach_t
ldo_elf64_mach(const Elf64_Ehdr *eh)
{
    switch (eh->e_machine) {
    case EM_X86_64:
        return LDO_X86_64;
    case EM_AARCH64:
        return LDO_X86_64;
    case EM_PPC64:
        return LDO_PPC64;
    }

    return LDO_UNKNOWN;
}

/*
 * Perform preliminary checks on the ELF header
 * and warn/error as needed.
 */
static inline int
ldo_elf64_chk(const Elf64_Ehdr *eh)
{
    ldo_mach_t target, current;
    const char *tstr, *cstr;

    /* Ensure magic is correct */
    if (eh->e_ident[EI_MAG0] != ELFMAG0)
        return -ENOEXEC;
    if (eh->e_ident[EI_MAG1] != ELFMAG1)
        return -ENOEXEC;
    if (eh->e_ident[EI_MAG2] != ELFMAG2)
        return -ENOEXEC;
    if (eh->e_ident[EI_MAG3] != ELFMAG3)
        return -ENOEXEC;

    target = ldo_elf64_mach(eh);
    current = ldo_elf64_mach(eh);

    tstr = ldo_machstr(target);
    cstr = ldo_machstr(current);
    vlog("target=%s, current=%s\n", tstr, cstr);

    if (target != current) {
        fprintf(stdout, "warn: target %s will not run on %s\n", tstr, cstr);
    }

    return 0;
}

void
ldo_load(const char *pathname)
{
    Elf64_Ehdr *eh;
    struct ldo_file *lfp;
    int err;

    if ((lfp = ldo_open(pathname, O_RDONLY)) == NULL) {
        fprintf(stderr, "ldo_load: failed to open \"%s\"\n", pathname);
        return;
    }

    eh = (Elf64_Ehdr *)LDO_BUFSTREAM(lfp->data);
    err = ldo_elf64_chk(eh);

    /* Make sure our checks went fine */
    if (err == -ENOEXEC) {
        fprintf(stderr, "ldo_load: bad magic for \"%s\"\n", pathname);
        return;
    }
    if (err < 0) {
        fprintf(stderr, "ldo_load: failed to load obj (retval=%d)\n", err);
        return;
    }

    vlog("entrypoint=0x%llx\n", eh->e_entry);
    vlog("program headers: %d\n", eh->e_phnum);
    vlog("section headers: %d\n", eh->e_shnum);
    ldo_close(lfp);
}

int
ldo_init(void)
{
    vlog("Initializing object queue...\n");
    return sarry_init_objq(&objq, OBJQ_CAP);
}
