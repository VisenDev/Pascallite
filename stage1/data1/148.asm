; YOUR NAME(S)       Fri Nov 18 15:45:27 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage1no148

_start:                                 
        call    ReadInt                 ; read int; value placed in eax
        mov     [I1],eax                ; store eax at a
        call    ReadInt                 ; read int; value placed in eax
        mov     [I2],eax                ; store eax at b
        call    ReadInt                 ; read int; value placed in eax
        mov     [I3],eax                ; store eax at c
        mov     eax,[I1]                ; AReg = a
        cmp     eax,[I0]                ; compare a and zero
        je      .L0                     ; if a = zero then jump to set eax to TRUE
        mov     eax,[FALSE]             ; else set eax to FALSE
        jmp     .L1                     ; unconditionally jump
.L0:                                    
        mov     eax,[TRUE]              ; set eax to TRUE
.L1:                                    
        mov     [B0],eax                ; d = AReg
        mov     eax,[I2]                ; AReg = b
        cmp     eax,[I3]                ; compare b and c
        jl      .L2                     ; if b < c then jump to set eax to TRUE
        mov     eax,[FALSE]             ; else set eax to FALSE
        jmp     .L3                     ; unconditionally jump
.L2:                                    
        mov     eax,[TRUE]              ; set eax to TRUE
.L3:                                    
        mov     [B1],eax                ; e = AReg
        mov     eax,[I1]                ; AReg = a
        cmp     eax,[I2]                ; compare a and b
        jg      .L4                     ; if a > b then jump to set eax to TRUE
        mov     eax,[FALSE]             ; else set eax to FALSE
        jmp     .L5                     ; unconditionally jump
.L4:                                    
        mov     eax,[TRUE]              ; set eax to TRUE
.L5:                                    
        mov     [B2],eax                ; f = AReg
        mov     eax,[I1]                ; load a in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[I2]                ; load b in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[I3]                ; load c in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[B0]                ; load d in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[B1]                ; load e in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[B2]                ; load f in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        Exit    {0}                     

SECTION .data                           
FALSE   dd      0                       ; false
TRUE    dd      -1                      ; true
I0      dd      0                       ; zero

SECTION .bss                            
I1      resd    1                       ; a
I2      resd    1                       ; b
I3      resd    1                       ; c
B0      resd    1                       ; d
B1      resd    1                       ; e
B2      resd    1                       ; f
