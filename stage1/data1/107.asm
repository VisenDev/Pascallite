; YOUR NAME(S)       Fri Nov 18 15:45:25 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage1no107

_start:                                 
        call    ReadInt                 ; read int; value placed in eax
        mov     [I3],eax                ; store eax at a
        call    ReadInt                 ; read int; value placed in eax
        mov     [I4],eax                ; store eax at b
        call    ReadInt                 ; read int; value placed in eax
        mov     [I5],eax                ; store eax at c
        mov     eax,[I3]                ; AReg = a
        add     eax,[I4]                ; AReg = a + b
        add     eax,[I5]                ; AReg = T0 + c
        mov     [I3],eax                ; a = AReg
        imul    dword [I4]              ; AReg = a * b
        imul    dword [I5]              ; AReg = T0 * c
        mov     [I6],eax                ; d = AReg
        mov     eax,[I3]                ; AReg = a
        add     eax,[I4]                ; AReg = a + b
        mov     [I6],eax                ; d = AReg
        mov     eax,[I3]                ; load a in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[I6]                ; load d in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        Exit    {0}                     

SECTION .data                           
I1      dd      5                       ; five
B0      dd      0                       ; right
I2      dd      45                      ; t0
B1      dd      -1                      ; wrong
I0      dd      0                       ; zero

SECTION .bss                            
I3      resd    1                       ; a
I4      resd    1                       ; b
I5      resd    1                       ; c
I6      resd    1                       ; d
