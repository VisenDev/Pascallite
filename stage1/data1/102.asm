; YOUR NAME(S)       Fri Nov 18 15:45:25 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage1no102

_start:                                 
        call    ReadInt                 ; read int; value placed in eax
        mov     [I2],eax                ; store eax at a
        call    ReadInt                 ; read int; value placed in eax
        mov     [I3],eax                ; store eax at b
        mov     eax,[I7]                ; AReg = -5
        cdq                             ; sign extend dividend from eax to edx:eax
        idiv    dword [I2]              ; AReg = -5 div a
        xchg    eax,edx                 ; exchange quotient and remainder
        add     eax,[I3]                ; AReg = T0 + b
        mov     [I6],eax                ; z = AReg
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        add     eax,[I3]                ; AReg = z + b
        mov     [T0],eax                ; deassign AReg
        mov     eax,[I2]                ; AReg = a
        sub     eax,[I3]                ; AReg = a - b
        imul    dword [T0]              ; AReg = T1 * T0
        mov     [I4],eax                ; c = AReg
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        Exit    {0}                     

SECTION .data                           
I7      dd      -5                      ; -5
I1      dd      5                       ; five
I0      dd      0                       ; zero

SECTION .bss                            
T0      resd    1                       ; T0
I2      resd    1                       ; a
I3      resd    1                       ; b
I4      resd    1                       ; c
I5      resd    1                       ; w
I6      resd    1                       ; z
