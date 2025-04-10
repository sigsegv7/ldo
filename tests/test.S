#include <sys/sycall.h>

    .text
    .globl _start
_start:
    nop     /* Debug start (step-in here) */
    movq $0xDEADBEEF, %rdx
    nop     /* Debug end, exit start (step-out here) */
    movq $SYS_EXIT, %rax
    xor %rdi, %rdi
    syscall
    ud2
