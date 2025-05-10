CPPPARAMS = -m32 -Iinclude\
            -nostdlib -nostdinc -static -ffreestanding \
            -fno-builtin -fno-rtti -fno-exceptions -fno-use-cxa-atexit \
            -fno-leading-underscore -fno-omit-frame-pointer \
            -fpermissive -Wall -Wextra -Werror \
            -Wno-error=unused-variable -Wno-error=unused-parameter -Wno-write-strings \
            -mno-red-zone -mno-sse -mno-sse2 -mno-mmx -mno-80387\
			-fno-pie -no-pie -fno-pic -g -O0\
    

ASPARAMS = --32
LDPARAMS = -melf_i386

build_dir = build
objects = $(build_dir)/loader.o \
          $(build_dir)/kernel.o \
          $(build_dir)/drivers/driver.o \
          $(build_dir)/drivers/keyboard.o \
          $(build_dir)/drivers/mouse.o \
          $(build_dir)/gdt.o \
          $(build_dir)/hardwareCommunication/port.o \
          $(build_dir)/hardwareCommunication/interrupts.o \
		  $(build_dir)/hardwareCommunication/pci.o \
          $(build_dir)/hardwareCommunication/interruptstubs.o


$(build_dir):
	mkdir -p $(build_dir)

$(build_dir)/%.o: src/%.cpp | $(build_dir)
	g++ $(CPPPARAMS) -o $@ -c $<

$(build_dir)/%.o: src/%.s | $(build_dir)
	as $(ASPARAMS) -o $@ $<

$(build_dir)/common/%.o: src/common/%.cpp | $(build_dir)
	mkdir -p $(dir $@)
	g++ $(CPPPARAMS) -o $@ -c $<

$(build_dir)/drivers/%.o: src/drivers/%.cpp | $(build_dir)
	mkdir -p $(dir $@)
	g++ $(CPPPARAMS) -o $@ -c $<

$(build_dir)/hardwareCommunication/%.o: src/hardwareCommunication/%.cpp | $(build_dir)
	mkdir -p $(dir $@)
	g++ $(CPPPARAMS) -o $@ -c $<

$(build_dir)/hardwareCommunication/%.o: src/hardwareCommunication/%.s | $(build_dir)
	mkdir -p $(dir $@)
	as $(ASPARAMS) -o $@ $<

$(build_dir)/kernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)


install: $(build_dir)/kernel.bin
	sudo cp $< /boot/kernel.bin

$(build_dir)/kernel.iso: $(build_dir)/kernel.bin
	mkdir -p iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "DysOS" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/kernel.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg

	grub-mkrescue --output=$@ iso 
	rm -rf iso 

run: $(build_dir)/kernel.iso
	VBoxManage controlvm "DysOS" poweroff || true
	sleep 2 
	VBoxManage startvm "DysOS" --type gui

clean:
	find $(build_dir) -type f \( -name '*.o' -o -name '*.bin' -o -name '*.iso' \) -delete
	rm -f *.iso
	rm -rf iso