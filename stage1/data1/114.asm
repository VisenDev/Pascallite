; YOUR NAME(S)       Fri Nov 18 15:45:25 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage1no114

_start:                                 
        mov     eax,[I1]                ; AReg = five
        imul    dword [I2]              ; AReg = five * a
        mov     [I6],eax                ; d = AReg
