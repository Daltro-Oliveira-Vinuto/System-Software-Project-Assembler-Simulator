
secao text ; ok

input n
load n


; aaaa
fat:    sub 			one
jmpz 	   exit
store n+1   ; old aux
mul n 
store      n
load n+1 ; old aux
jmp fat
exit: 	
output n

stop

secao data

one: 
const 1
n: space 2
; aux: space not necessary anymore because of the line above
