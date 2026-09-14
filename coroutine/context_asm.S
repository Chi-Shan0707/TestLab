.text

.globl __ctx_save
__ctx_save:
    movq %rbx, 0(%rdi)
    movq %rbp, 8(%rdi)
    movq %r12, 16(%rdi)
    movq %r13, 24(%rdi)
    movq %r14, 32(%rdi)
    movq %r15, 40(%rdi)
    leaq 8(%rsp), %rax
    movq %rax, 48(%rdi)
    movq (%rsp), %rax
    movq %rax, 56(%rdi)
    xorl %eax, %eax
    ret

.globl __ctx_restore
__ctx_restore:
    movq 0(%rdi), %rbx
    movq 8(%rdi), %rbp
    movq 16(%rdi), %r12
    movq 24(%rdi), %r13
    movq 32(%rdi), %r14
    movq 40(%rdi), %r15
    movq 64(%rdi), %rcx
    movq 56(%rdi), %r11
    movq 48(%rdi), %rsp
    movl %esi, %eax
    movq %rcx, %rdi
    jmp *%r11
    
#ifdef __ELF__
.section .note.GNU-stack,"",@progbits
#endif
