global main
extern printf


	section .text
main:
	push dword [num2]
	push dword [num1]
	push print0
	call printf
	add esp,12
	mov eax,0
        mov ebx, dword [num1]
        mov ecx,0
        mov edx, dword [num2]
mult:
	shr edx,1
	jc increase
	shl ebx,1
	inc ecx
	cmp ecx,16
	jne mult
	jmp print1
increase:
	add eax,ebx
	inc ecx
	shl ebx,1
	cmp ecx,16
	jne mult
	jmp print1
print1:
	push eax
	push printa
	call printf
	add esp,8

start:
	mov eax,0
	mov ebx, dword [num1]
	mov ecx,0
	mov edx, dword [num2]
set:
	cmp ebx,edx
	jl check
	shl edx,1
	inc ecx
	jmp set
div:
	shl eax,1
	cmp ebx, edx
	jge more
	jmp less
more:
	sub ebx, edx
	inc eax
	cmp edx, dword [num2]
	je print2
	shr edx,1
	jmp div
less:
	cmp edx, dword [num2]
	je print2
	shr edx,1
	jmp div
check:
	cmp ecx,0
	jne div
	mov eax,0
print2:
	push ebx
	push eax
	push printb
	call printf
	add esp,12
	ret

	section .data
	print0: db 'Numbers: %d %d',10,0
	printa: db 'Multiply in dec: %lu',10,0
	printb: db 'Division in dec: %d. Rest: %d',10,0
	num1: dd 10101111011111000b
	num2: dd 110011011110b

