; YOUR NAME(S)       Fri Nov 18 15:45:27 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage1no146

_start:                                 
        mov     eax,[I4]                ; AReg = 3
        add     eax,[I5]                ; AReg = 3 + 34
        imul    dword [I0]              ; AReg = T0 * five
        mov     [I1],eax                ; a = AReg
        mov     eax,[I6]                ; AReg = -5
        cdq                             ; sign extend dividend from eax to edx:eax
        idiv    dword [I1]              ; AReg = -5 div a
        xchg    eax,edx                 ; exchange quotient and remainder
        add     eax,[I1]                ; AReg = T0 + a
        mov     [I2],eax                ; b = AReg
        add     eax,[I1]                ; AReg = b + a
        mov     [T0],eax                ; deassign AReg
        mov     eax,[I2]                ; AReg = b
        sub     eax,[I1]                ; AReg = b - a
        imul    dword [T0]              ; AReg = T1 * T0
        mov     [I3],eax                ; c = AReg
        mov     eax,[I1]                ; load a in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[I2]                ; load b in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[I3]                ; load c in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        Exit    {0}                     

SECTION .data                           
I6      dd      -5                      ; -5
I4      dd      3                       ; 3
I5      dd      34                      ; 34
I0      dd      5                       ; five

SECTION .bss                            
T0      resd    1                       ; T0
I1      resd    1                       ; a
I2      resd    1                       ; b
I3      resd    1                       ; c
