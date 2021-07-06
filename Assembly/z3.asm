global main
extern scanf
extern printf

section .text
main:
	push n
	push scan
	call scanf
	add esp,8
	mov eax,1
	mov ebx,[n]
	mov ecx,0
loop:
	inc ecx
	mul ecx
	cmp ecx,ebx
	jl loop
	push eax
	push print
	call printf
	add esp,8
	ret

section .data
	scan: db '%d',0
	print: db '%lu',10,0
	n: dd 0

