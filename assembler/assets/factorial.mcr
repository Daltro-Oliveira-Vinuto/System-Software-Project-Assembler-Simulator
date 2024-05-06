secao text

input n
load n
store aux
store fat

label_fat:
load aux
sub one
jmpz end
store aux
mul fat
store fat
jmp label_fat
end: output fat
stop

secao data
n: space
fat: space
aux: space
one: const 1
