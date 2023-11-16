; Robert Burnett, Matthew Barton    Wed Nov 15 20:38:52 2023
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage0no018

_start:                                 
        Exit    {0}                     
                                        
SECTION .data                           
I1      dd      5                       ; five
I0      dd      0                       ; zero
                                        
SECTION .bss                            
I2      resd    1                       ; aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
I3      resd    1                       ; bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb
I4      resd    1                       ; ccccccccccccccccccccccccccccccc
B0      resd    1                       ; d
I5      resd    1                       ; ddddddddddddddddddddddddddddddd
I6      resd    1                       ; eeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
I7      resd    1                       ; fffffffffffffffffffffffffffffff
I8      resd    1                       ; ggggggggggggggggggggggggggggggg
I9      resd    1                       ; hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
I:      resd    1                       ; iiiiiiiiiiiiiiiiiiiiiiiiiiiiiii
I;      resd    1                       ; jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj
I<      resd    1                       ; kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk
I=      resd    1                       ; lllllllllllllllllllllllllllllll
I>      resd    1                       ; mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
I?      resd    1                       ; nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn
I@      resd    1                       ; ooooooooooooooooooooooooooooooo
IA      resd    1                       ; ppppppppppppppppppppppppppppppp
IB      resd    1                       ; qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
IC      resd    1                       ; rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
ID      resd    1                       ; sssssssssssssssssssssssssssssss
IE      resd    1                       ; ttttttttttttttttttttttttttttttt
IF      resd    1                       ; uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu
IG      resd    1                       ; vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
IH      resd    1                       ; wwwwwwwwwwwwwwwwwwwwwwwwwwwwwww
II      resd    1                       ; xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
IJ      resd    1                       ; yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy
IK      resd    1                       ; zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
