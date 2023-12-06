; YOUR NAME(S)       Fri Nov 18 15:45:27 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage1no149

_start:                                 
        call    ReadInt                 ; read int; value placed in eax
        mov     [I0],eax                ; store eax at a
        call    ReadInt                 ; read int; value placed in eax
        mov     [I1],eax                ; store eax at b
        call    ReadInt                 ; read int; value placed in eax
        mov     [I2],eax                ; store eax at c
        call    ReadInt                 ; read int; value placed in eax
        mov     [I3],eax                ; store eax at d
        call    ReadInt                 ; read int; value placed in eax
        mov     [I4],eax                ; store eax at e
        call    ReadInt                 ; read int; value placed in eax
        mov     [I5],eax                ; store eax at f
        mov     eax,[I0]                ; AReg = a
        add     eax,[I1]                ; AReg = a + b
        mov     [T0],eax                ; deassign AReg
        mov     eax,[I2]                ; AReg = c
        add     eax,[I3]                ; AReg = c + d
        mov     [T1],eax                ; deassign AReg
        mov     eax,[I4]                ; AReg = e
        add     eax,[I5]                ; AReg = e + f
        add     eax,[T1]                ; AReg = T2 + T1
        add     eax,[T0]                ; AReg = T1 + T0
        mov     [I0],eax                ; a = AReg
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        Exit    {0}                     

SECTION .data                           

SECTION .bss                            
T0      resd    1                       ; T0
T1      resd    1                       ; T1
I0      resd    1                       ; a
I1      resd    1                       ; b
I2      resd    1                       ; c
I3      resd    1                       ; d
I4      resd    1                       ; e
I5      resd    1                       ; f
