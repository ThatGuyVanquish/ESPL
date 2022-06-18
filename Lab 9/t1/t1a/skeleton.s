%macro	syscall1 2
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro	syscall3 4
	mov	edx, %4
	mov	ecx, %3
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro  exit 1
	syscall1 1, %1
%endmacro

%macro  write 3
	syscall3 4, %1, %2, %3
%endmacro

%macro  read 3
	syscall3 3, %1, %2, %3
%endmacro

%macro  open 3
	syscall3 5, %1, %2, %3
%endmacro

%macro  lseek 3
	syscall3 19, %1, %2, %3
%endmacro

%macro  close 1
	syscall1 6, %1
%endmacro

%define	STK_RES	200
%define	RDWR	2
%define	SEEK_END 2
%define SEEK_SET 0

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8
%define ELFHDR_size 52
%define ELFHDR_phoff	28

%define FD dword [ebp - 4]
%define ELF_header ebp - 56 ; Subbing 56 because -4 for pc increment and 52 for elf header size 

	global _start

	section .text
_start:	
	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage
	;CODE START
	call get_my_loc
	add ebx, FileName			; Opening file in FileName
	open ebx, RDWR, 0x777
	mov FD, eax

	; Checking if file is an elf file
	lea ecx, [ELF_header]		; Grabbing the effective address of elf header
	read FD, ecx, ELFHDR_size
	cmp dword [ELF_header], 0x464C457F
	jne not_elf

	; Printing OutStr
	call get_my_loc
	add ebx, OutStr
	write 1, ebx, 32
	close FD
	jmp VirusExit

not_elf:
	close FD
	call get_my_loc
	add ebx, Failstr
	write 1, ebx, 12
	exit -1

VirusExit:
       exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)
	
FileName:	db "ELFexec", 0 ; default was with 1 but you gave us the file without it
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
Failstr:    db "perhaps not", 10 , 0


get_my_loc:              ; puts location in ebx
	call next_i
next_i:
	pop ebx				; changed to ebx because this doesn't work with ecx 
	sub ebx, next_i		; need to add sub because it doesn't subtract the address which screws things up
	ret	
PreviousEntryPoint: dd VirusExit
virus_end:


