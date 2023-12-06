; YOUR NAME(S)       Fri Nov 18 15:45:25 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage1no115

_start:                                 
        mov     eax,[B0]                ; AReg = yes
        and     eax,[B1]                ; AReg = yes and no
        mov     [B7],eax                ; d = AReg
        mov     eax,[B0]                ; AReg = yes
        mov     [B2],eax                ; a = AReg
        mov     eax,[B0]                ; AReg = yes
        or      eax,[B1]                ; AReg = yes or no
        mov     [B3],eax                ; b = AReg
        not     eax                     ; AReg = !AReg
        mov     [B4],eax                ; c = AReg
        mov     eax,[B7]                ; load d in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[B2]                ; load a in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[B3]                ; load b in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        mov     eax,[B4]                ; load c in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        Exit    {0}                     

SECTION .data                           
B1      dd      0                       ; no
B0      dd      -1                      ; yes

SECTION .bss                            
B2      resd    1                       ; a
B3      resd    1                       ; b
B4      resd    1                       ; c
B7      resd    1                       ; d
B8      resd    1                       ; e
B5      resd    1                       ; w
B6      resd    1                       ; z
