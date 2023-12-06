; YOUR NAME(S)       Fri Nov 18 15:45:25 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage1no104

_start:                                 
        call    ReadInt                 ; read int; value placed in eax
        mov     [I1],eax                ; store eax at a
        add     eax,[I4]                ; AReg = a + 2
        add     eax,[I0]                ; AReg = T0 + five
        mov     [I1],eax                ; a = AReg
        sub     eax,[I1]                ; AReg = a - a
        mov     [I2],eax                ; b = AReg
        cdq                             ; sign extend dividend from eax to edx:eax
        idiv    dword [I1]              ; AReg = b div a
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
I4      dd      2                       ; 2
I0      dd      5                       ; five

SECTION .bss                            
I1      resd    1                       ; a
I2      resd    1                       ; b
I3      resd    1                       ; c
