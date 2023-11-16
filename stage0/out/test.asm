; Robert Burnett, Matthew Barton    Wed Nov 15 18:42:00 2023

%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           ; 
global  _start                          ; ; program test

_start:                                 ; 
SECTION .data                           ; 
I0      dd      1                       ; ; a
I1      dd      10                      ; ; b
B0      dd      false                   ; ; c
B1      dd      true                    ; ; d
SECTION .bss                            ; 
I2      resd                            ; ; foo
        Exit    {0}                     ; 
