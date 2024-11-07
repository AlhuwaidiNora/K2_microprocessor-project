; Fibonacci sequence generator
RA = 0      ; Initialize first number
RB = 1      ; Initialize second number
R0 = RA     ; Output first number
RB = RA+RB  ; Calculate next number
JC = 0      ; Check for carry
RA = RA+RB  ; Update current number
RB = RA-RB  ; Swap numbers
RA = RA-RB  ; Complete swap
J = 2       ; Jump back to output
