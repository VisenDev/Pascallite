; YOUR NAME(S)       Fri Nov 18 15:45:25 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage1no105

_start:                                 
        call    ReadInt                 ; read int; value placed in eax
        mov     [I2],eax                ; store eax at a
        call    ReadInt                 ; read int; value placed in eax
        mov     [I3],eax                ; store eax at b
        call    ReadInt                 ; read int; value placed in eax
        mov     [I4],eax                ; store eax at c
        call    ReadInt                 ; read int; value placed in eax
        mov     [I6],eax                ; store eax at z
        mov     eax,[I3]                ; AReg = b
        cdq                             ; sign extend dividend from eax to edx:eax
        idiv    dword [I2]              ; AReg = b div a
        xchg    eax,edx                 ; exchange quotient and remainder
        mov     [I6],eax                ; z = AReg
        imul    dword [I4]              ; AReg = z * c
        mov     [I5],eax                ; w = AReg
        mov     eax,[I6]                ; AReg = z
        neg     eax                     ; AReg = -AReg
        mov     [I2],eax                ; a = AReg
        mov     eax,[I4]                ; AReg = c
        mov     [I3],eax                ; b = AReg
        mov     eax,[I6]                ; load z in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[I5]                ; load w in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[I2]                ; load a in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[I3]                ; load b in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        Exit    {0}                     

SECTION .data                           
I1      dd      5                       ; five
I0      dd      0                       ; zero

SECTION .bss                            
I2      resd    1                       ; a
I3      resd    1                       ; b
I4      resd    1                       ; c
I5      resd    1                       ; w
I6      resd    1                       ; z
