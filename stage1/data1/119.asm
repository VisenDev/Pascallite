; YOUR NAME(S)       Fri Nov 18 15:45:25 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage1no119

_start:                                 
        call    ReadInt                 ; read int; value placed in eax
        mov     [I0],eax                ; store eax at f
        call    ReadInt                 ; read int; value placed in eax
        mov     [I1],eax                ; store eax at g
        call    ReadInt                 ; read int; value placed in eax
        mov     [I2],eax                ; store eax at h
        mov     eax,[I0]                ; AReg = f
        cmp     eax,[I1]                ; compare f and g
        jle     .L0                     ; if f <= g then jump to set eax to TRUE
        mov     eax,[FALSE]             ; else set eax to FALSE
        jmp     .L1                     ; unconditionally jump
.L0:                                    
        mov     eax,[TRUE]              ; set eax to TRUE
.L1:                                    
        mov     [B2],eax                ; c = AReg
        mov     eax,[B0]                ; AReg = a
        cmp     eax,[B1]                ; compare a and b
        je      .L2                     ; if a = b then jump to set eax to TRUE
        mov     eax,[FALSE]             ; else set eax to FALSE
        jmp     .L3                     ; unconditionally jump
.L2:                                    
        mov     eax,[TRUE]              ; set eax to TRUE
.L3:                                    
        mov     [B3],eax                ; d = AReg
        mov     eax,[I0]                ; AReg = f
        cmp     eax,[I2]                ; compare f and h
        je      .L4                     ; if f = h then jump to set eax to TRUE
        mov     eax,[FALSE]             ; else set eax to FALSE
        jmp     .L5                     ; unconditionally jump
.L4:                                    
        mov     eax,[TRUE]              ; set eax to TRUE
.L5:                                    
        mov     [B4],eax                ; e = AReg
        mov     eax,[B2]                ; load c in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[B3]                ; load d in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[B4]                ; load e in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        Exit    {0}                     

SECTION .data                           
B0      dd      -1                      ; a
B1      dd      -1                      ; b
FALSE   dd      0                       ; false
TRUE    dd      -1                      ; true

SECTION .bss                            
B2      resd    1                       ; c
B3      resd    1                       ; d
B4      resd    1                       ; e
I0      resd    1                       ; f
I1      resd    1                       ; g
I2      resd    1                       ; h
