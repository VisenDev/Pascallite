; YOUR NAME(S)       Fri Nov 18 15:45:25 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage1no118

_start:                                 
        mov     eax,[I0]                ; AReg = w
        cmp     eax,[I1]                ; compare w and z
        je      .L0                     ; if w = z then jump to set eax to TRUE
        mov     eax,[FALSE]             ; else set eax to FALSE
        jmp     .L1                     ; unconditionally jump
.L0:                                    
        mov     eax,[TRUE]              ; set eax to TRUE
.L1:                                    
        mov     [T0],eax                ; deassign AReg
        mov     eax,[B0]                ; AReg = a
        cmp     eax,[B1]                ; compare a and b
        jne     .L2                     ; if a <> b then jump to set eax to TRUE
        mov     eax,[FALSE]             ; else set eax to FALSE
        jmp     .L3                     ; unconditionally jump
.L2:                                    
        mov     eax,[TRUE]              ; set eax to TRUE
.L3:                                    
        and     eax,[T0]                ; AReg = T1 and T0
        mov     [B2],eax                ; c = AReg
