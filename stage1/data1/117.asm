; YOUR NAME(S)       Fri Nov 18 15:45:25 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage1no117

_start:                                 
        mov     eax,[I0]                ; AReg = w
        cmp     eax,[I1]                ; compare w and z
        jl      .L0                     ; if w < z then jump to set eax to TRUE
        mov     eax,[FALSE]             ; else set eax to FALSE
        jmp     .L1                     ; unconditionally jump
.L0:                                    
        mov     eax,[TRUE]              ; set eax to TRUE
.L1:                                    
        mov     [T0],eax                ; deassign AReg
        mov     eax,[B0]                ; AReg = a
        or      eax,[B1]                ; AReg = a or b
        and     eax,[T0]                ; AReg = T1 and T0
        mov     [B4],eax                ; e = AReg
        mov     eax,[I1]                ; AReg = z
        cmp     eax,[I4]                ; compare z and 3
        jge     .L2                     ; if z >= 3 then jump to set eax to TRUE
        mov     eax,[FALSE]             ; else set eax to FALSE
        jmp     .L3                     ; unconditionally jump
.L2:                                    
        mov     eax,[TRUE]              ; set eax to TRUE
.L3:                                    
        mov     [T0],eax                ; deassign AReg
        mov     eax,[I0]                ; AReg = w
        cmp     eax,[I5]                ; compare w and 4
        jl      .L4                     ; if w < 4 then jump to set eax to TRUE
        mov     eax,[FALSE]             ; else set eax to FALSE
        jmp     .L5                     ; unconditionally jump
.L4:                                    
        mov     eax,[TRUE]              ; set eax to TRUE
.L5:                                    
        or      eax,[T0]                ; AReg = T1 or T0
        mov     [B3],eax                ; d = AReg
        Exit    {0}                     

SECTION .data                           
I4      dd      3                       ; 3
I5      dd      4                       ; 4
B0      dd      -1                      ; a
B1      dd      0                       ; b
FALSE   dd      0                       ; false
TRUE    dd      -1                      ; true

SECTION .bss                            
T0      resd    1                       ; T0
B2      resd    1                       ; c
B3      resd    1                       ; d
B4      resd    1                       ; e
I0      resd    1                       ; w
I3      resd    1                       ; x
I2      resd    1                       ; y
I1      resd    1                       ; z
