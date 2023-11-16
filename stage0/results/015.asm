; Robert Burnett, Matthew Barton    Wed Nov 15 20:38:52 2023
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage0no015

_start:                                 
        Exit    {0}                     
                                        
SECTION .data                           
I2      dd      1                       ; big
I3      dd      2                       ; biggeryet
B1      dd      0                       ; no
I4      dd      1                       ; same
I0      dd      0                       ; small
I1      dd      -1                      ; smalleryet
B0      dd      -1                      ; yes
                                        
SECTION .bss                            
B2      resd    1                       ; right
I5      resd    1                       ; some
