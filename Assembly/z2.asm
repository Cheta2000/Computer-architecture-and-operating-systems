	global main
	extern printf

	section .text
main:
	mov eax,0
	mov ecx,-1
	mov edx,0
done:
	mov ebx,1
	inc ecx
check:
	cmp ebx,ecx
	jl increase
	mov ebx,1
	add eax,edx
	mov edx,0
	cmp ecx,4
	jne loop
	jmp print
increase:
	imul edx,10
	inc ebx
	jmp check
loop:
	shr dword [number],1
	jc multiply
	imul ebx,2
	cmp ebx,16
	jne loop
	jmp done
multiply:
	add edx,ebx
	imul ebx,2
	cmp ebx,16
	je done
	jmp loop
print:
	push eax
	push message
	call printf
	add esp,8
	ret

	section .data

number: dq 0111100110000010b
message: db '%d',10,0
