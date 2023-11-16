; Robert Burnett, Matthew Barton    Wed Nov 15 20:38:52 2023
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage0no051

_start:                                 
        Exit    {0}                     
                                        
SECTION .data                           
I2      dd      1                       ; big
I3      dd      +2336                   ; biggeryet
B6      dd      0                       ; fal_se
I4      dd      +2336                   ; large
B2      dd      0                       ; maybe
I6      dd      -1234                   ; minus
B1      dd      0                       ; no
B5      dd      0                       ; off
B4      dd      -1                      ; on
B3      dd      -1                      ; possible
I0      dd      0                       ; small
I1      dd      -1234                   ; smalleryet
B7      dd      -1                      ; tru_e
B0      dd      -1                      ; yes
I5      dd      0                       ; zero
                                        
SECTION .bss                            
I8      resd    1                       ; many
B8      resd    1                       ; right
I7      resd    1                       ; some
B9      resd    1                       ; wrong
