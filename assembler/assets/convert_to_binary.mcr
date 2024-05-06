secao text

input n
load n
store a

conv:
	load a
	div dois
	store q

	mul dois
	store aux
	load a
	sub aux
	store bit
	output bit

	load q
	jmpz final

	store a
	jmp conv

final:
	stop

secao data

a: space
n: space
dois: const 2
q: space
aux: space
bit: space

