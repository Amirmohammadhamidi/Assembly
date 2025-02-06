section .note.GNU-stack

section .text

global fill_array
global line_handler

fill_array:
    ;xmm0 : percision
    ;xmm1 : x0 (initial x)

    xor r8, r8 ; use r8 as index and set it to 0
    shufps xmm0 , xmm0 , 0 ; set all xmm0 elements to percision

    shufps xmm1 , xmm1 , 0; set all xmm1 elements to x0

    xorps xmm3 , xmm3 ; set xmm3 to zero
    mov rax, 0x3f800000 ; Bit representation of 1.0
    pinsrd xmm3 , eax , 1
    mov rax,0x40000000 
    pinsrd xmm3 , eax , 2 ; Bit representation of 2.0
    mov rax, 0x40400000
    pinsrd xmm3 , eax , 3 ; Bit representation of 3.0

    
    mulps xmm3 , xmm0 ; xmm3 : {0.0 * percision , ... , 3.0 * percision}

    addps xmm1 , xmm3 ; xmm1 : {x0 , x0 + 1.0 * percision , ..}

    mov rax, 0x40800000  ; Bit representation of 4.0
    movd xmm2 , eax
    shufps xmm2 , xmm2 , 0 ; xmm2 : {4.0 , 4.0 , 4.0 , 4,0}
    mulps xmm2 , xmm0 ; xmm2 : {4.0 * percision , 4.0 * percision , 4.0 * percision , 4.0 * percision}

    xorps xmm0 , xmm0

.loop:
    cmp r8, rsi         ; Compare index with size
    je .done           ; if index is equal to size of array

    movaps xmm0, xmm1   ; xmm0 <- xmm1
    addps xmm1, xmm2    ; xmm1 <- xmm1 + xmm2;

    movaps [rdi + 4 * r8], xmm0  ; Store the result (r8*4 for float indexing)

    add r8 , 4              ; Increment the index by 1 (for the next float)

    jmp .loop           ; Repeat the loop

.done:
    ret

line_handler:
    ;rdi : size
    ;rsi : float * x_array
    ;rdx : float * y_array
    ;xmm0 : a , xmm1 : b
    shufps xmm0 , xmm0 , 0 ; xmm0 = {a , a , a , a}
    shufps xmm1 , xmm1 , 0 ; xmm1 = {b , b , b , b}
    xorps xmm3 , xmm3 ; clear xmm3
    xor r8 , r8 ; clear r8
.loop:
    cmp r8, rdi         ; Compare index with size
    je .done           ; if index is equal to size of array

    movaps xmm3 , [rsi + 4 * r8] ; load 4 float number to xmm3
    mulps xmm3 , xmm0
    addps xmm3 , xmm1

    movaps [rdx + 4 * r8], xmm3  ; Store the result (4 * r8 for float indexing)

    add r8 , 4              ; Increment the index by 1 (for the next float)

    jmp .loop           ; Repeat the loop

.done:
    ret

