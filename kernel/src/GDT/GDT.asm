[bits 64]

LoadGDT:
    lgdt [rdi] ; RDI register will contain the parameter
    mov ax, 0x10 ; Move KernelData
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    pop rdi
    mov rax, 0x08 ; Move Kernel Code
    push rax
    push rdi
    retfq
    
GLOBAL LoadGDT