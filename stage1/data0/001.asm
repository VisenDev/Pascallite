; YOUR NAME(S)       Mon Nov  7 15:02:52 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage0no001

_start:                                 
        Exit    {0}                     

SECTION .data                           
I2      dd      1                       ; big
I3      dd      2                       ; biggeryet
I4      dd      2                       ; large
B2      dd      0                       ; maybe
B1      dd      0                       ; no
I0      dd      0                       ; small
I1      dd      -1                      ; smalleryet
B0      dd      -1                      ; yes

SECTION .bss                            
I6      resd    1                       ; many
B3      resd    1                       ; right
I5      resd    1                       ; some
B4      resd    1                       ; wrong
