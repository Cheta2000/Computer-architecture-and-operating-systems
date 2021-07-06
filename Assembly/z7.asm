global main
extern printf
extern scanf

section .text

main:
	finit

	fldln2
	fld dword [x]
	fyl2x
	sub esp,8
	fstp qword [esp]
	push printa
	call printf
	add esp,12

	fld dword [x]
	fldl2e
	fmulp
	fld1
	fscale
	fxch
	fld1
	fxch
	fprem
	f2xm1
	faddp
	fmulp
	sub esp,8
	fst qword [esp]
	push printb
	call printf
	add esp,12

	fld st0
	fld1
	fdiv st0,st1
	fsubp
	fld1
	fld1
	faddp
	fdivr st0,st1
	sub esp,8
	fstp qword [esp]
	push printc
	call printf
	add esp,12

	fld dword [x]
	fld st0
	fmulp
	fld1
	faddp
	fsqrt
	fld dword [x]
	faddp
	fldln2
	fxch
	fyl2x
	sub esp,8
	fstp qword[esp]
	push printd
	call printf
	add esp,12

	ret




section .data
	x: dd 8.94
	scan: db '%f',0
	printa: db 'a) %f',10,0
	printb: db 'b) %f',10,0
	printc: db 'c) %f',10,0
	printd: db 'd) %f',10,0
