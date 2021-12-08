[org 0x7E00]

; Print that we are loading the kernel
mov ah, 0x0E

mov al, 'K'
int 0x10
mov al, 'L'
int 0x10

; Start booting into 32bit protected mode before actually loading the kernel
; Enable the A20 line
in al, 0x92
or al, 2
out 0x92, al

cli ; Disable interrupts

; Load the GDT
lgdt [GDT_DESC]

; Set the CR0 register to tell that we are in 32bit mode
mov eax, cr0
or eax, 1
mov cr0, eax

jmp CODE_SEG:StartPM ; Far jump to label StartPM

[bits 32] ; Tell nasm that we are using 32bit instructions
StartPM: ; Far jumped to start executing in 32 bit protected mode
	; Updating segment registers to point to our new data segment
	mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	; Message to tell that we are in 32bit PM
	mov [0xB8000], byte '3' ; Print "3"
	mov [0xB8002], byte '2' ; Print "2"
	mov [0xB8004], byte 'S' ; Print "S"

	; Start 64bit Long Mode
	; Check is CPUID is supported
	pushfd
	pop eax
	mov ecx, eax
	xor eax, 1 << 21
	push eax
	popfd
	pushfd
	pop eax
	push ecx
	popfd
	xor eax, ecx
	jz NoCPUID      ; CPUID is not supported

	; Check if long mode if supported
	mov eax, 0x80000001
	cpuid
	test edx, 1 << 29
	jz NoLM

	; Enable paging
	mov edi, 0x1000
	mov cr3, edi
	mov dword [edi], 0x2003
	add edi, 1000
	mov dword [edi], 0x3003
	add edi, 1000
	mov dword [edi], 0x4003
	add edi, 1000
	mov ebx, 0x00000003
	mov ecx, 512
	.SetEntry:
		mov dword [edi], ebx
		add ebx, 0x1000
		add edi, 8
		loop .SetEntry
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax
	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr
	mov eax, cr0
	or eax, 1 << 30
	mov cr0, eax

	; Edit the GDT to support 64bit LM
	call EditGDT

	; Far Jump into the function that starts the C kernel
	jmp CODE_SEG:StartKernelC

NoCPUID:                ; No CPUID support
	mov [0xB8000], byte 'N'
	mov [0xB8000], byte 'C'
	mov [0xB8000], byte 'I'
	hlt
NoLM:                   ; No long mode support
	mov [0xB8000], byte 'N'
	mov [0xB8002], byte 'L'
	mov [0xB8004], byte 'M'
	hlt

; The actuall function to start the C kernel
[bits 64]
StartKernelC:
	; Clear the screen with blue
	mov edi, 0xb8000
	mov rax, 0x1f201f201f201f20
	mov ecx, 500
	rep stosq
	mov [0xB8000], byte '0'

	jmp $           ; Infinate loop

[bits 16]
; The GDT
GDT_NULL:         ; GDT NULL descriptor, has 8 bytes of 0s
	dd 0         ; 4Bytes
	dd 0         ; 4Bytes

GDT_CODEDESC:     ; GDT Code descriptor
	dw 0xFFFF    ; Limit (max)
	dw 0x0000    ; Base (start)
	db 0x0000    ; Base address (start)
	db 10011010b ; Access Byte
	db 11001111b ; Access flags & limit
	db 0x00      ; Base
GDT_DATADESC:     ; GDT Data descriptor
        dw 0xFFFF    ; Limit (max)
        dw 0x0000    ; Base (start)
        db 0x0000    ; Base address (start)
        db 10010010b ; Access Byte
        db 11001111b ; Access flags & limit
        db 0x00      ; Base
GDT_END: ; End of the GDT
GDT_DESC:        ; GDT Descriptor
	GDT_SIZE:
		dw GDT_END - GDT_NULL - 1 ; Size of the GDT
		dq GDT_NULL               ; Address of the GDT

; GDT Segments
CODE_SEG equ GDT_CODEDESC - GDT_NULL      ; GDT code segment
DATA_SEG equ GDT_DATADESC - GDT_NULL      ; GDT data segment

; Edit the GDT to support 64bit
[bits 32]
EditGDT:
	mov [GDT_CODEDESC + 6], byte 10101111b
	mov [GDT_DATADESC + 6], byte 10101111b
	ret

times 2048-($-$$) db 0
