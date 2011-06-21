.PHONY: mm

CELL_MK_DIR = $(CELL_SDK)/samples/mk
include $(CELL_MK_DIR)/sdk.makedef.mk
CELL_INC_DIR = $(CELL_SDK)/target/include

MM	= source/
SHADERS = shaders/
RELEASE = ./release
BIN	= bin/
NPDRM	= /NPDRM_RELEASE

MM_REL	= multiMAN
APPID	= BLES80608

CONTENT_ID=MM4PS3-$(APPID)_00-MULTIMANAGER0200

MAKE_SELF_NPDRM = make_self_npdrm

PPU_SRCS = $(MM)main.cpp
PPU_TARGET = $(MM_REL)_BOOT.elf
PPU_OPTIMIZE_LV := -O2 -fno-exceptions

PPU_INCDIRS= -Iinclude -I$(CELL_INC_DIR) -I$(CELL_SDK)/target/ppu/include/sysutil -I$(CELL_SDK)/target/ppu/include
PPU_LDLIBS += -lpthread -lm -lio_stub -lsysmodule_stub -lsysutil_stub -lfs_stub\

all : $(PPU_TARGET)

PPU_CFLAGS  += -g -O2 -fno-exceptions 

include $(CELL_MK_DIR)/sdk.target.mk

mm : $(PPU_TARGET)
	@mkdir -p $(BIN)
	@$(PPU_STRIP) -s $< -o $(OBJS_DIR)/$(PPU_TARGET)

	@$(MAKE_SELF_NPDRM) ./objs/$(MM_REL)_BOOT.elf $(RELEASE)$(NPDRM)/USRDIR/EBOOT.BIN $(CONTENT_ID) > nul
	@rm $(PPU_TARGET)
	$(MAKE) -f makefile.multiman mm
