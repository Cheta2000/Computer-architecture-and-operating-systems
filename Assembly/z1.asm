	global _start
	section .text

_start:
	mov ecx,0
loop:
	inc ecx
	mov al,[number]
	cmp ecx,1
	je first
	shl al,4
	shr al,4
	cmp al,9
	jg more
	add al,"0"
	jmp print
first:
	shr al,4
	cmp al,9
	jg more
	add al,"0"
	jmp print

more:
	sub al,10
	add al,"A"
	jmp print

print:
	push ecx
	mov [sign],al
	mov eax,4
	mov ebx,1
	mov ecx,sign
	mov edx,1
	int 80h
	pop ecx
	cmp ecx,2
	jne loop
	mov eax,1
	mov ebx,0
	int 80h

section .data
sign db 0
number db 0xF8

