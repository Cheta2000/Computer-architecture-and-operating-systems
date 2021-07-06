global main
extern printf
extern scanf
extern myatof
extern myftoa
section .text

main:
	mov edx,1024
	mov ecx,input1
	mov ebx,0
	mov eax,3
	int 80h
	push input1
	call myatof
	fstp qword [num1]
	add esp,4
	mov edx,1024
	mov ecx,operand
	mov ebx,0
	mov eax,3
	int 80h
	mov edx,1024
	mov ecx,input2
	mov ebx,0
	mov eax,3
	int 80h
	push input2
	call myatof
	fstp qword [num2]
	add esp,4
	finit
	fld qword [num1]
	fld qword [num2]
	mov al,byte [operand]
	cmp al,'+'
	je add
	cmp al,'-'
	je sub
	cmp al,'*'
	je mult
	cmp al,'/'
	je div
	ret
add:
	fadd st0,st1
	jmp out
sub:
	fsubr st0,st1
	jmp out
mult:
	fmul st0,st1
	jmp out
div:
	fdivr st0,st1
	jmp out
out:
	fstp qword [score]
	push buf
	push score
	call myftoa
	fstp dword [end]
	add esp,8
	;mov edx,1024
	;mov ecx,end
	;mov ebx,1
	;mov eax,4
	;int 80h
	push dword [score+4]
	push dword [score]
	push print
	call printf
	add esp,12
	ret

section .data
	print: db '%f',10,0
section .bss
	input1: resq 1
	operand: resq 1	
	input2: resq 1
	score: resq 1
	num1: resq 1
	num2: resq 1
	buf: resq 1
	end: resq 1
