; YOUR NAME(S)       Fri Nov 18 15:45:25 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage1no101

_start:                                 
        call    ReadInt                 ; read int; value placed in eax
        mov     [I2],eax                ; store eax at a
        call    ReadInt                 ; read int; value placed in eax
        mov     [I3],eax                ; store eax at b
        call    ReadInt                 ; read int; value placed in eax
        mov     [I4],eax                ; store eax at c
        mov     eax,[I2]                ; load a in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[I3]                ; load b in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[I5]                ; AReg = 3
        add     eax,[I6]                ; AReg = 3 + 34
        imul    dword [I1]              ; AReg = T0 * five
        mov     [I2],eax                ; a = AReg
        add     eax,[I2]                ; AReg = a + a
        mov     [I3],eax                ; b = AReg
        mov     eax,[I1]                ; load five in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[I2]                ; load a in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[I3]                ; load b in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[I4]                ; load c in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[I1]                ; load five in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[I0]                ; load zero in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        Exit    {0}                     

SECTION .data                           
I5      dd      3                       ; 3
I6      dd      34                      ; 34
I1      dd      5                       ; five
I0      dd      0                       ; zero

SECTION .bss                            
I2      resd    1                       ; a
I3      resd    1                       ; b
I4      resd    1                       ; c
