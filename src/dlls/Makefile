export TOPDIR		:=	$(CURDIR)

export ELF2DLL		:=	../../../tools/elf2dll
export LIBDINO		:=	$(TOPDIR)/libdino
export DINOHACK		:=	$(TOPDIR)/lib_dinohack
export IDO			:=	../../../tools/ido_recomp/linux/5.3
export N64CRC		:=	tools/n64crc
export PYTHON		:=	python3
export XDELTA3		:=	xdelta3
export ARMIPS		:=	tools/armips

16: $(ELF2DLL)
	$(MAKE) -B -C 16

365: $(ELF2DLL)
	$(MAKE) -B -C 365

$(ELF2DLL):
	#$(MAKE) -C elf2dll

$(N64CRC):
	gcc tools/n64crc.c -o $(N64CRC)

run: build/DINO.z64
	$(PROJECT64) build/DINO.z64 | true

stock:
	$(PROJECT64) tools/DINO.z64 | true

patch: build/DINO.z64
	$(XDELTA3) -S djw -f -e -s tools/DINO.z64 build/DINO.z64 build/DINO.xdelta

rel:
	mkdir -p release

	$(MAKE) clean dinomod rom patch
	cp build/DINO.xdelta release/dinomod.xdelta

	$(MAKE) clean dinopatch rom patch
	cp build/DINO.xdelta release/dinopatch.xdelta
