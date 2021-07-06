	global main
	extern printf

	section .text
main:
	mov dx,[number]
	mov ecx,16
loop:
	shl dx,1
	mov al,[bcd]
	adc al,al
	daa
	mov [bcd],al
	mov al,[bcd+1]
	adc al,al
	daa
	mov [bcd+1],al
	mov al,[bcd+2]
	adc al,al
	daa
	mov [bcd+2],al
	dec ecx
	jnz loop
end:
	push dword[bcd]
	push message
	call printf
	add esp,8
	ret


	section .data
number: dd 12349
message: db "%x",10,0
bcd: db 0

