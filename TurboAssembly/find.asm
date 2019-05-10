.286
JUMPS
MASM
model small
.stack 256
.data
	wrd db 255, 255, 0, 10 dup (00h); a word to find with size of 4
	wrd_size db 0
    file_name db 'test.txt'
	file_size dw 0
    msg_er db 'An error occurred with code $' ; messages
	msg_find db 'find>$'
	msg_filename db 'filename>$'
	msg_part_1 db ' sentences were found, $'
	msg_part_2 db ' questuons, $'
	msg_part3 db ' exclamations and $'
	msg_part_4 db ' commas.$'
    msg_curr_args db 'Currect Number Of Args'
    buffer db 1000 dup (?); a buffer for reading
	file_handle dw (?); file handle returned by 3dh
	questuons dw 0
	excl dw 0
	commas dw 0
	sentence dw 0
	tire dw 0
	row dw 1; row of current word
	col dw 1; col or current word
.code

; using macros for readability
open_file macro ; opens a file for reading, saves the handle in file_handle AND in bx
	mov ah, 3dh; function for opening a file
	lea dx, file_name;
	mov cx, 0; file mode read
	int 21h
	jc er
	mov file_handle, ax;
	mov bx, file_handle
endm

output_09 macro msg
	mov ah, 09h; function for std out
	mov dx, offset msg
	int 21h
endm

output_msg macro msg1, sz
	mov ah, 06h; direct console output
	mov si, 0;
	mov cx, sz;
go:
	mov dl, msg1 + si
	int 21h
	inc si;
	loop go
endm

input macro msg_buffer
	mov dx, offset msg_buffer
	mov ah, 0Ah;
	int 21h
endm

close_file macro
	mov ah, 3eh; function for closing a file
	int 21h
	jc er;
endm

read_file macro; reads buffer_size bytes from file to buffer. When done ax bytes have been read, if ax = 0 => EOF
    mov ah, 3fh; function for reading cx byte
	mov cx, 1000
	lea dx, buffer
	int 21h
	jc er
	mov file_size, ax;
endm

print_file macro ; prints file buffer on std out
	mov	cx, file_size
	mov	si, 0
	mov ah, 02h
print_loop:
	mov dl, buffer[si]
	int	21h
	inc si
	loop print_loop
endm

find_print_word_in_file macro
    mov	cx, file_size
	mov	si, 0
	mov di, 0
	cld; scan in forward direction
trav_loop:
	mov dl, buffer[si]
	cmp dl, 0Ah
	jne not_found_new_line
	inc row
	mov col, 1
	jmp mismatch
not_found_new_line:
	cmp dl, 3Fh
	jne not_found_question
	inc questuons
	jmp mismatch
not_found_question:
	cmp dl, 2Ch
	jne not_found_comma
	inc commas
	jmp mismatch
not_found_comma:
	cmp dl, 20h
	jne not_found_space
	inc col
	jmp mismatch
not_found_space:
	cmp dl, 21h
	jne not_found_excl
	inc excl
	jmp mismatch
not_found_excl:
	cmp dl, 2Eh
	jne not_found_sentence
	inc sentence
	jmp mismatch
not_found_sentence:
	cmp dl, 2Dh
	jne not_found_tire
	inc tire
mismatch:
	jmp misss
not_found_tire:
	
	pusha
	mov ax, si
	lea si, buffer
	add si, ax
	lea di, wrd + 2; text to search in
	mov cl, wrd_size
	xor ch, ch
	repe cmpsb
	jne misss1
	call print_nl
	push row
	call print_dw
	call print_sp
	push col
	call print_dw
	misss1:
	popa
misss:
	inc si;
	loop trav_loop
	
	call print_nl
	push sentence
	call print_dw
	output_09 msg_part_1
	
	push questuons
	call print_dw
	output_09 msg_part_2
	
	push excl
	call print_dw
	output_09 msg_part3
	
	push commas
	call print_dw
	output_09 msg_part_4
endm

main:
    mov ax,@data
    mov ds,ax
    xor ax,ax
	
    output_09 msg_find
	input wrd
	mov al, wrd + 1
	mov wrd_size, al; size of read characters
	xor ax, ax
	
	open_file; opens a file for reading, saves the handle in file_handle AND in bx
	read_file; reads buffer_size bytes from file to buffer
	close_file
	jmp skip_er
er:
    mov dx, offset msg_er
    mov ah, 09h; write string to std out
    int 21h
	mov ah, 02h
	mov dl, al
	add dl, 30h; char to int
	int 21h
    jmp exit
skip_er:
	print_file
	
	find_print_word_in_file
	
	;call find_all_matches
exit:
    mov ax, 4c00h; return 0
    int 21h

; ---------------------------
try_match_word PROC
	push bp
	mov bp, sp
	mov ax,[bp+4]	
	
	lea si, buffer
	add si, ax
	lea di, wrd + 2; text to search in
	mov cl, wrd_size
	xor ch, ch
	repe cmpsb
	jne miss
	
	call print_nl
	push row
	call print_dw
	call print_sp
	push col
	call print_dw
miss:
	;pop bp
	ret
try_match_word ENDP

; ---------------------------
print_sp PROC; print a space
	mov dl, 20h; space
	mov ah, 02h
	int 21h
	ret
print_sp ENDP 
; ---------------------------
print_nl PROC; print new line
	mov dl, 0Ah; new line
	mov ah, 02h
	int 21h
	ret
print_nl ENDP
; ---------------------------
print_dw PROC
	push bp
	mov bp, sp	
	mov ax, [BP + 4]
	mov cl, 10
	mov si, 0
	div cl; al - result, ah - mod
	add ah, 30h; parse to int
	add al, 30h; parse to int
	push ax
	mov ah, 02h; print one char
	pop dx
	int 21h
	xchg dl, dh
	int 21h
	pop bp
	ret
print_dw ENDP
; ---------------------------
end main