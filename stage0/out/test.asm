; Robert Burnett, Matthew Barton    Wed Nov 15 16:06:10 2023

%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program test
_start:                                 
        Exit    {0}                     
SECTION .data                           
I0      dd      1                       ; a
I1      dd      10                      ; b
B0      dd      false                   ; c
B1      dd      true                    ; d
SECTION .bss                            
I2      resd                            ; foo
