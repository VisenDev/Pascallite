; YOUR NAME(S)       Fri Nov 18 15:45:26 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage1no123

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
        call    ReadInt                 ; read int; value placed in eax
        mov     [I15],eax               ; store eax at o
        call    ReadInt                 ; read int; value placed in eax
        mov     [I16],eax               ; store eax at p
        call    ReadInt                 ; read int; value placed in eax
        mov     [I17],eax               ; store eax at q
        call    ReadInt                 ; read int; value placed in eax
        mov     [I18],eax               ; store eax at r
        call    ReadInt                 ; read int; value placed in eax
        mov     [I19],eax               ; store eax at s
        call    ReadInt                 ; read int; value placed in eax
        mov     [I20],eax               ; store eax at t
        call    ReadInt                 ; read int; value placed in eax
        mov     [I21],eax               ; store eax at u
        call    ReadInt                 ; read int; value placed in eax
        mov     [I22],eax               ; store eax at v
        call    ReadInt                 ; read int; value placed in eax
        mov     [I23],eax               ; store eax at w
        call    ReadInt                 ; read int; value placed in eax
        mov     [I24],eax               ; store eax at x
        call    ReadInt                 ; read int; value placed in eax
        mov     [I25],eax               ; store eax at y
        call    ReadInt                 ; read int; value placed in eax
        mov     [I26],eax               ; store eax at z
        mov     eax,[I1]                ; AReg = a
        sub     eax,[I2]                ; AReg = a - b
        mov     [T0],eax                ; deassign AReg
        mov     eax,[I3]                ; AReg = c
        sub     eax,[I4]                ; AReg = c - d
        mov     [T1],eax                ; deassign AReg
        mov     eax,[I5]                ; AReg = e
        sub     eax,[I6]                ; AReg = e - f
        mov     [T2],eax                ; deassign AReg
        mov     eax,[I7]                ; AReg = g
        sub     eax,[I8]                ; AReg = g - h
        mov     [T3],eax                ; deassign AReg
        mov     eax,[I9]                ; AReg = i
        sub     eax,[I10]               ; AReg = i - j
        mov     [T4],eax                ; deassign AReg
        mov     eax,[I11]               ; AReg = k
        sub     eax,[I12]               ; AReg = k - l
        mov     [T5],eax                ; deassign AReg
        mov     eax,[I13]               ; AReg = m
        sub     eax,[I14]               ; AReg = m - n
        mov     [T6],eax                ; deassign AReg
        mov     eax,[I15]               ; AReg = o
        sub     eax,[I16]               ; AReg = o - p
        mov     [T7],eax                ; deassign AReg
        mov     eax,[I17]               ; AReg = q
        sub     eax,[I18]               ; AReg = q - r
        mov     [T8],eax                ; deassign AReg
        mov     eax,[I19]               ; AReg = s
        sub     eax,[I20]               ; AReg = s - t
        mov     [T9],eax                ; deassign AReg
        mov     eax,[I21]               ; AReg = u
        sub     eax,[I22]               ; AReg = u - v
        mov     [T10],eax               ; deassign AReg
        mov     eax,[I23]               ; AReg = w
        sub     eax,[I24]               ; AReg = w - x
        mov     [T11],eax               ; deassign AReg
        mov     eax,[I25]               ; AReg = y
        sub     eax,[I26]               ; AReg = y - z
        mov     [T12],eax               ; deassign AReg
        mov     eax,[T11]               ; AReg = T11
        sub     eax,[T12]               ; AReg = T11 - T12
        mov     [T11],eax               ; deassign AReg
        mov     eax,[T10]               ; AReg = T10
        sub     eax,[T11]               ; AReg = T10 - T11
        mov     [T10],eax               ; deassign AReg
        mov     eax,[T9]                ; AReg = T9
        sub     eax,[T10]               ; AReg = T9 - T10
        mov     [T9],eax                ; deassign AReg
        mov     eax,[T8]                ; AReg = T8
        sub     eax,[T9]                ; AReg = T8 - T9
        mov     [T8],eax                ; deassign AReg
        mov     eax,[T7]                ; AReg = T7
        sub     eax,[T8]                ; AReg = T7 - T8
        mov     [T7],eax                ; deassign AReg
        mov     eax,[T6]                ; AReg = T6
        sub     eax,[T7]                ; AReg = T6 - T7
        mov     [T6],eax                ; deassign AReg
        mov     eax,[T5]                ; AReg = T5
        sub     eax,[T6]                ; AReg = T5 - T6
        mov     [T5],eax                ; deassign AReg
        mov     eax,[T4]                ; AReg = T4
        sub     eax,[T5]                ; AReg = T4 - T5
        mov     [T4],eax                ; deassign AReg
        mov     eax,[T3]                ; AReg = T3
        sub     eax,[T4]                ; AReg = T3 - T4
        mov     [T3],eax                ; deassign AReg
        mov     eax,[T2]                ; AReg = T2
        sub     eax,[T3]                ; AReg = T2 - T3
        mov     [T2],eax                ; deassign AReg
        mov     eax,[T1]                ; AReg = T1
        sub     eax,[T2]                ; AReg = T1 - T2
        mov     [T1],eax                ; deassign AReg
        mov     eax,[T0]                ; AReg = T0
        sub     eax,[T1]                ; AReg = T0 - T1
        mov     [I0],eax                ; sum = AReg
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        Exit    {0}                     

SECTION .data                           

SECTION .bss                            
T0      resd    1                       ; T0
T1      resd    1                       ; T1
T10     resd    1                       ; T10
T11     resd    1                       ; T11
T12     resd    1                       ; T12
T2      resd    1                       ; T2
T3      resd    1                       ; T3
T4      resd    1                       ; T4
T5      resd    1                       ; T5
T6      resd    1                       ; T6
T7      resd    1                       ; T7
T8      resd    1                       ; T8
T9      resd    1                       ; T9
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
I15     resd    1                       ; o
I16     resd    1                       ; p
I17     resd    1                       ; q
I18     resd    1                       ; r
I19     resd    1                       ; s
I0      resd    1                       ; sum
I20     resd    1                       ; t
I21     resd    1                       ; u
I22     resd    1                       ; v
I23     resd    1                       ; w
I24     resd    1                       ; x
I25     resd    1                       ; y
I26     resd    1                       ; z
