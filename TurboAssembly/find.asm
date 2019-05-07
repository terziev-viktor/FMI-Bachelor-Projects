MASM
model small
.stack 256
.data
    msg_er db 'An error occurred with code $' ; messages
    msg_curr_args db 'Currect Number Of Args'
    buffer db 1000 dup (?); a buffer for reading
	file_size dw 0
    file_name db 'test.txt'
	file_handle dw (?); file handle returned by 3dh
	wrd db 'test'; a word to find with size of 4
	row db 1; row of current word
	col db 1; col or current word
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
    
endm

main:
    mov ax,@data
    mov ds,ax
    xor ax,ax
    
	open_file
	read_file
	print_file
	find_print_word_in_file
	close_file
	jmp exit
er:
    mov dx, offset msg_er
    mov ah, 09h; write string to std out
    int 21h
	mov ah, 02h
	mov dl, al
	add dl, 30h; char to int
	int 21h
    jmp exit
exit:
    mov ax, 4c00h; return 0
    int 21h
end main