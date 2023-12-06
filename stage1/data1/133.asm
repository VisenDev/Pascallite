; YOUR NAME(S)       Fri Nov 18 15:45:26 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage1no133

_start:                                 
        call    ReadInt                 ; read int; value placed in eax
        mov     [I0],eax                ; store eax at a
        call    ReadInt                 ; read int; value placed in eax
        mov     [I1],eax                ; store eax at b
        mov     eax,[I3]                ; AReg = 2
        imul    dword [I0]              ; AReg = 2 * a
        mov     [T0],eax                ; deassign AReg
        mov     eax,[I1]                ; AReg = b
        cdq                             ; sign extend dividend from eax to edx:eax
        idiv    dword [T0]              ; AReg = b div T0
        neg     eax                     ; AReg = -AReg
        mov     [I2],eax                ; r = AReg
        mov     eax,[I0]                ; load a in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[I1]                ; load b in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[I2]                ; load r in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        Exit    {0}                     

SECTION .data                           
I3      dd      2                       ; 2

SECTION .bss                            
T0      resd    1                       ; T0
I0      resd    1                       ; a
I1      resd    1                       ; b
I2      resd    1                       ; r
