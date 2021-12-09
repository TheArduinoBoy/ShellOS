[org 0x7C00]
mov [BOOT_DISK], dl
mov bp, 0x7C00
mov sp, 0x7C00
mov bx, 0x8000
mov ah, 0x02
mov al, 16
mov dl, [BOOT_DISK]
mov ch, 0
mov dh, 0
mov cl, 0x02
int 0x13
jc diskerror
jmp 0x8000
BOOT_DISK: db 0
diskerror:
	mov ah, 0x0E
	mov al, "D"
	int 0x10
	mov al, "S"
	int 0x10
	mov al, "E"
	int 0x10
	jmp $
times 510-($-$$) db 0
dw 0xAA55
