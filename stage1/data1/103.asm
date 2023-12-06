; YOUR NAME(S)       Fri Nov 18 15:45:25 2022
%INCLUDE "Along32.inc"
%INCLUDE "Macros_Along.inc"

SECTION .text                           
global  _start                          ; program stage1no103

_start:                                 
        call    ReadInt                 ; read int; value placed in eax
        mov     [I5],eax                ; store eax at z
        cmp     eax,[I0]                ; compare z and zero
        jg      .L0                     ; if z > zero then jump to set eax to TRUE
        mov     eax,[FALSE]             ; else set eax to FALSE
        jmp     .L1                     ; unconditionally jump
.L0:                                    
        mov     eax,[TRUE]              ; set eax to TRUE
.L1:                                    
        mov     [B0],eax                ; d = AReg
        mov     eax,[I5]                ; load z in eax
        call    WriteInt                ; write int in eax to standard out
        call    Crlf                    ; write \r\n to standard out
        Exit    {0}                     

SECTION .data                           
FALSE   dd      0                       ; false
I1      dd      5                       ; five
TRUE    dd      -1                      ; true
I0      dd      0                       ; zero

SECTION .bss                            
I2      resd    1                       ; a
I3      resd    1                       ; b
I4      resd    1                       ; c
B0      resd    1                       ; d
I5      resd    1                       ; z
