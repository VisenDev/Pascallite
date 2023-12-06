; YOUR NAME(S)       Fri Nov 18 15:45:25 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage1no106

_start:                                 
        call    ReadInt                 ; read int; value placed in eax
        mov     [I1],eax                ; store eax at b
        call    ReadInt                 ; read int; value placed in eax
        mov     [I2],eax                ; store eax at c
        mov     eax,[I4]                ; AReg = 3
        mov     [I0],eax                ; a = AReg
        mov     eax,[I1]                ; AReg = b
        imul    dword [I2]              ; AReg = b * c
        add     eax,[I0]                ; AReg = T0 + a
        mov     [I3],eax                ; d = AReg
        mov     eax,[I0]                ; AReg = a
        imul    dword [I1]              ; AReg = a * b
        add     eax,[I2]                ; AReg = T0 + c
        mov     [I1],eax                ; b = AReg
        mov     eax,[I3]                ; load d in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[I1]                ; load b in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        Exit    {0}                     

SECTION .data                           
I4      dd      3                       ; 3

SECTION .bss                            
I0      resd    1                       ; a
I1      resd    1                       ; b
I2      resd    1                       ; c
I3      resd    1                       ; d
