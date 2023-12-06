; YOUR NAME(S)       Fri Nov 18 15:45:26 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage1no124

_start:                                 
        call    ReadInt                 ; read int; value placed in eax
        mov     [I1],eax                ; store eax at a
        call    ReadInt                 ; read int; value placed in eax
        mov     [I2],eax                ; store eax at b
        call    ReadInt                 ; read int; value placed in eax
        mov     [I3],eax                ; store eax at c
        call    ReadInt                 ; read int; value placed in eax
        mov     [I4],eax                ; store eax at d
        call    ReadInt                 ; read int; value placed in eax
        mov     [I5],eax                ; store eax at e
        call    ReadInt                 ; read int; value placed in eax
        mov     [I6],eax                ; store eax at f
        call    ReadInt                 ; read int; value placed in eax
        mov     [I7],eax                ; store eax at g
        call    ReadInt                 ; read int; value placed in eax
        mov     [I8],eax                ; store eax at h
        call    ReadInt                 ; read int; value placed in eax
        mov     [I9],eax                ; store eax at i
        call    ReadInt                 ; read int; value placed in eax
        mov     [I10],eax               ; store eax at j
        call    ReadInt                 ; read int; value placed in eax
        mov     [I11],eax               ; store eax at k
        call    ReadInt                 ; read int; value placed in eax
        mov     [I12],eax               ; store eax at l
        call    ReadInt                 ; read int; value placed in eax
        mov     [I13],eax               ; store eax at m
        call    ReadInt                 ; read int; value placed in eax
        mov     [I14],eax               ; store eax at n
        mov     eax,[I13]               ; AReg = m
        sub     eax,[I14]               ; AReg = m - n
        mov     [T0],eax                ; deassign AReg
        mov     eax,[I12]               ; AReg = l
        sub     eax,[T0]                ; AReg = l - T0
        mov     [T0],eax                ; deassign AReg
        mov     eax,[I11]               ; AReg = k
        sub     eax,[T0]                ; AReg = k - T0
        mov     [T0],eax                ; deassign AReg
        mov     eax,[I10]               ; AReg = j
        sub     eax,[T0]                ; AReg = j - T0
        mov     [T0],eax                ; deassign AReg
        mov     eax,[I9]                ; AReg = i
        sub     eax,[T0]                ; AReg = i - T0
        mov     [T0],eax                ; deassign AReg
        mov     eax,[I8]                ; AReg = h
        sub     eax,[T0]                ; AReg = h - T0
        mov     [T0],eax                ; deassign AReg
        mov     eax,[I7]                ; AReg = g
        sub     eax,[T0]                ; AReg = g - T0
        mov     [T0],eax                ; deassign AReg
        mov     eax,[I6]                ; AReg = f
        sub     eax,[T0]                ; AReg = f - T0
        mov     [T0],eax                ; deassign AReg
        mov     eax,[I5]                ; AReg = e
        sub     eax,[T0]                ; AReg = e - T0
        mov     [T0],eax                ; deassign AReg
        mov     eax,[I4]                ; AReg = d
        sub     eax,[T0]                ; AReg = d - T0
        mov     [T0],eax                ; deassign AReg
        mov     eax,[I3]                ; AReg = c
        sub     eax,[T0]                ; AReg = c - T0
        mov     [T0],eax                ; deassign AReg
        mov     eax,[I2]                ; AReg = b
        sub     eax,[T0]                ; AReg = b - T0
        mov     [T0],eax                ; deassign AReg
        mov     eax,[I1]                ; AReg = a
        sub     eax,[T0]                ; AReg = a - T0
        mov     [I0],eax                ; sum = AReg
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        Exit    {0}                     

SECTION .data                           

SECTION .bss                            
T0      resd    1                       ; T0
I1      resd    1                       ; a
I2      resd    1                       ; b
I3      resd    1                       ; c
I4      resd    1                       ; d
I5      resd    1                       ; e
I6      resd    1                       ; f
I7      resd    1                       ; g
I8      resd    1                       ; h
I9      resd    1                       ; i
I10     resd    1                       ; j
I11     resd    1                       ; k
I12     resd    1                       ; l
I13     resd    1                       ; m
I14     resd    1                       ; n
I0      resd    1                       ; sum
