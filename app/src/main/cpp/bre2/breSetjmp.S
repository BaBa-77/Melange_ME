.section .text, "x"
.arm

.global breSetjmp
.global breLongjmp

breSetjmp:
    stmea r0!, {r4-r10, fp, ip, sp, lr}
    mov r0, #0
    bx lr

breLongjmp:
    ldmfd r0!, {r4-r10, fp, ip, sp, lr}
    movs r0, r1
    moveq r0, #1
    bx lr
