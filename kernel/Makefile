
OSNAME = SquidOS

GNUEFI = ../gnu-efi
OVMFDIR = ../OVMFbin
LDS = Linker.ld
CC = gcc
ASMC = nasm
LD = ld

CFLAGS = -ffreestanding -fshort-wchar -mno-red-zone
ASMFLAGS =
LDFLAGS = -T $(LDS) -static -Bsymbolic -nostdlib

SRCDIR := src
OBJDIR := lib
BUILDDIR := bin
FONTDIR := fonts
BOOTEFI := $(GNUEFI)/x86_64/bootloader/main.efi

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.cpp)
ASMSRC = $(call rwildcard,$(SRCDIR),*.asm)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
OBJS += $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%_asm.o, $(ASMSRC))

clearobj:
	@echo !--- Clearing $(OBJDIR) Folder ---!
	@rm -r $(OBJDIR)
	@mkdir -p $(OBJDIR)

clearbin:
	@echo !--- Clearing $(BUILDDIR) Folder ---!
	@rm -r $(BUILDDIR)
	@mkdir -p $(BUILDDIR)

kernel: $(OBJS) link
	@echo !--- Compiled and Linked ---!

$(OBJDIR)/Interrupts/Interrupts.o: $(SRCDIR)/Interrupts/Interrupts.cpp
	@echo !--- Compiling $^ ---!
	@mkdir -p $(@D)
	@$(CC) -mno-red-zone -mgeneral-regs-only -ffreestanding -c $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo !--- Compiling $^ ---!
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/%_asm.o: $(SRCDIR)/%.asm
	@echo !--- Compiling $^ ---!
	@mkdir -p $(@D)
	@$(ASMC) $(ASMFLAGS) $^ -f elf64 -o $@

link:
	@echo !--- Linking $^ ---!
	@$(LD) $(LDFLAGS) -o $(BUILDDIR)/Kernel.elf $(OBJS)

setup:
	@mkdir $(SRCDIR)
	@mkdir $(OBJDIR)
	@mkdir $(BUILDDIR)

buildimg:
	@echo !--- Building Image ---!
	@dd if=/dev/zero of=$(BUILDDIR)/$(OSNAME).img bs=512 count=93750
	@mformat -i $(BUILDDIR)/$(OSNAME).img -f 1440 ::
	@mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI
	@mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI/BOOT
	@mcopy -i $(BUILDDIR)/$(OSNAME).img $(BOOTEFI) ::/EFI/BOOT
	@mcopy -i $(BUILDDIR)/$(OSNAME).img startup.nsh ::
	@mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/Kernel.elf ::
	@mcopy -i $(BUILDDIR)/$(OSNAME).img $(FONTDIR)/zap-light16.psf ::

run:
	@echo !--- Running ---!
	@qemu-system-x86_64 -machine q35 -drive format=raw,file=$(BUILDDIR)/$(OSNAME).img -m 256M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none -soundhw pcspk
