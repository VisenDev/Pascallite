; Robert Burnett, Matthew Barton    Wed Nov 15 20:38:52 2023
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage0no049

_start:                                 
        Exit    {0}                     
                                        
SECTION .data                           
I2      dd      1                       ; big
I3      dd      2                       ; bigger__yet
B2      dd      0                       ; maybe
B1      dd      0                       ; no
I0      dd      0                       ; small
I1      dd      -1                      ; smaller_yet
B0      dd      -1                      ; yes
                                        
SECTION .bss                            
I5      resd    1                       ; many
B3      resd    1                       ; right
I4      resd    1                       ; some
B4      resd    1                       ; wrong
