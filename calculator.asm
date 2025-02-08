section .note.GNU-stack

section .data
    float_value dd 1.0

section .text

global fill_array
global line_handler
global concave_handler
global sin_handler

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
    xor r8 , r8 ; clear r8
    xorps xmm2 , xmm2 ; clear xmm2

    shufps xmm0 , xmm0 , 0 ; xmm0 = {a , a , a , a}
    shufps xmm1 , xmm1 , 0 ; xmm1 = {b , b , b , b}
    
.loop:
    cmp r8, rdi         ; Compare index with size
    je .done           ; if index is equal to size of array

    movaps xmm2 , [rsi + 4 * r8] ; load 4 float number to xmm2
    mulps xmm2 , xmm0
    addps xmm2 , xmm1

    movaps [rdx + 4 * r8], xmm2  ; Store the result (4 * r8 for float indexing)

    add r8 , 4              ; Increment the index by 1 (for the next float)

    jmp .loop           ; Repeat the loop

.done:
    ret

concave_handler:
    ;rdi : size , rsi : x_array , rdx : y_array
    ;xmm0 : a , xmm1 : b , xmm2 : c
    xor r8 , r8 ; clear r8
    xorps xmm3 , xmm3 ; clear xmm3 and use it as result
    xorps xmm4 , xmm4 ; clear xmm4

    shufps xmm0 , xmm0 , 0 ; xmm0 = {a , a , a , a}
    shufps xmm1 , xmm1 , 0 ; xmm1 = {b , b , b , b}
    shufps xmm2 , xmm2 , 0 ; xmm2 = {c , c , c , c}

.loop:
    cmp r8, rdi         ; Compare index with size
    je .done           ; if index is equal to size of array

    movaps xmm3 , [rsi + 4 * r8] ; load 4 float number to xmm3
    movaps xmm4 , xmm3 ; xmm4 <- xmm3

    mulps xmm3 , xmm3
    mulps xmm3 , xmm0 ; xmm3 = {ax^2}

    mulps xmm4 , xmm1 ; xmm4 = {bx}

    addps xmm3 , xmm4
    addps xmm3 , xmm2 ; xmm3 = {ax^2 + bx + c}

    movaps [rdx + 4 * r8], xmm3  ; Store the result (4 * r8 for float indexing)

    add r8 , 4              ; Increment the index by 1 (for the next float)

    jmp .loop           ; Repeat the loop

.done:
    ret

sin_handler:
    ; rdi : size (number of floats)
    ; rsi : x_array (input float array)
    ; rdx : y_array (output float array)
    ; xmm0 : x0 , xmm1 : y0 , xmm2 : w_sin , xmm3 : w_cos

    xor r8, r8                ; r8 = 0 (index counter)

    shufps xmm0, xmm0, 0      ; xmm0 = {x0, x0, x0, x0}
    shufps xmm1, xmm1, 0      ; xmm1 = {y0, y0, y0, y0}
    shufps xmm2, xmm2, 0      ; xmm2 = {w_sin, w_sin, w_sin, w_sin}
    shufps xmm3, xmm3, 0      ; xmm3 = {w_cos, w_cos, w_cos, w_cos}

.loop:
    cmp r8, rdi               ; Check if r8 >= size
    jae .done                 ; Exit if done

    movaps xmm4, [rsi + 4 * r8]  ; Load 4 floats (use movups for unaligned access)
    subps xmm4, xmm0             ; xmm4 = x - x0
    divps xmm4, xmm3             ; xmm4 = (x - x0) / cos(w)
    mulps xmm4, xmm2             ; xmm5 = w_sin * (x - x0) / cos(w)
    addps xmm4 , xmm1

    movaps [rdx + 4 * r8], xmm4  ; Store the result (unaligned access)

    add r8, 4                    ; Move to next 4 floats
    jmp .loop                    ; Repeat

.done:
    ret