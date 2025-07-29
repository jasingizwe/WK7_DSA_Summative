section .data
    filename    db "names.txt", 0
    msg         db "Line count: ", 0
    newline     db 10, 0

section .bss
    buffer      resb 4096
    bytes_read  resd 1
    digits_buf  resb 16
    fd          resd 1

section .text
    global _start

_start:
    mov eax, 5
    mov ebx, filename
    mov ecx, 0
    int 0x80
    cmp eax, 0
    js open_failed
    mov [fd], eax

    mov eax, 3
    mov ebx, [fd]
    mov ecx, buffer
    mov edx, 4096
    int 0x80
    cmp eax, 0
    jle read_failed
    mov [bytes_read], eax

    mov ecx, eax
    mov esi, buffer
    xor edi, edi

count_loop:
    lodsb
    cmp al, 10
    jne next_char
    inc edi

next_char:
    loop count_loop

    mov eax, [bytes_read]
    dec eax
    cmp byte [buffer + eax], 10
    je skip_add
    inc edi

skip_add:

    mov eax, 4
    mov ebx, 1
    mov ecx, msg
    mov edx, 11
    int 0x80

    mov eax, edi
    mov edi, digits_buf
    add edi, 16
    xor ecx, ecx

    cmp eax, 0
    jne conv_loop
    mov byte [edi - 1], '0'
    mov ecx, 1
    dec edi
    jmp print_digits

conv_loop:
    mov esi, 10
    xor edx, edx         ; **must clear edx here**
    div esi
    add dl, '0'
    dec edi
    mov [edi], dl
    inc ecx
    test eax, eax
    jnz conv_loop

print_digits:
    mov eax, 4
    mov ebx, 1
    mov ecx, edi
    mov edx, ecx
    sub edx, digits_buf
    neg edx
    int 0x80

    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80

    mov eax, 6
    mov ebx, [fd]
    int 0x80

    mov eax, 1
    xor ebx, ebx
    int 0x80

open_failed:
    mov eax, 1
    mov ebx, 1
    int 0x80

read_failed:
    mov eax, 1
    mov ebx, 2
    int 0x80

