secao text
input n+2
load n+2
fat: sub one
jmpz exit
store aux
mul n+2
store n+2
load aux
jmp fat
exit: output n+2
stop
secao data
one: const 1
n: space
aux: space
res: space