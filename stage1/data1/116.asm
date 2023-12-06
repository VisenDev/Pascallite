; YOUR NAME(S)       Fri Nov 18 15:45:25 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage1no116

_start:                                 
        mov     eax,[TRUE]              ; AReg = true
        mov     [B3],eax                ; b = AReg
        mov     eax,[B0]                ; AReg = yes
        and     eax,[TRUE]              ; AReg = yes and true
        mov     [T0],eax                ; deassign AReg
        mov     eax,[B1]                ; AReg = no
        or      eax,[FALSE]             ; AReg = no or false
        and     eax,[T0]                ; AReg = T1 and T0
        mov     [B7],eax                ; d = AReg
        mov     eax,[B3]                ; AReg = b
        not     eax                     ; AReg = !AReg
        and     eax,[B0]                ; AReg = T0 and yes
        mov     [B5],eax                ; w = AReg
        mov     eax,[B7]                ; load d in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[B5]                ; load w in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        Exit    {0}                     

SECTION .data                           
FALSE   dd      0                       ; false
B1      dd      0                       ; no
TRUE    dd      -1                      ; true
B0      dd      -1                      ; yes

SECTION .bss                            
T0      resd    1                       ; T0
B2      resd    1                       ; a
B3      resd    1                       ; b
B4      resd    1                       ; c
B7      resd    1                       ; d
B8      resd    1                       ; e
B5      resd    1                       ; w
B6      resd    1                       ; z
