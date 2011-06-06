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

PPU_OPTIMIZE_LV := -O2
PPU_CPPFLAGS	:= -DPSGL

PPU_INCDIRS= -Iinclude -I$(CELL_INC_DIR) -I$(CELL_SDK)/target/ppu/include/sysutil -I$(CELL_SDK)/target/ppu/include -I/psl1ght/include
PPU_LDLIBS += -l./libftp 
PPU_LDLIBS += -lpthread -lm -lgcm_sys_stub -lio_stub -lsysmodule_stub -lsysutil_stub -lfs_stub -lrtc_stub \

all : $(PPU_TARGET)

PPU_CFLAGS  += -g

VPSHADER_SRCS = vpshader.cg vpshader2.cg
FPSHADER_SRCS = fpshader.cg fpshader2.cg

VPSHADER_PPU_OBJS = $(patsubst %.cg, $(OBJS_DIR)/$(MM)%.ppu.o, $(VPSHADER_SRCS))
FPSHADER_PPU_OBJS = $(patsubst %.cg, $(OBJS_DIR)/$(MM)%.ppu.o, $(FPSHADER_SRCS))

include $(CELL_MK_DIR)/sdk.target.mk

$(VPSHADER_PPU_OBJS): $(OBJS_DIR)/$(MM)%.ppu.o : %.vpo
	@mkdir -p $(dir $(@))

$(FPSHADER_PPU_OBJS): $(OBJS_DIR)/$(MM)%.ppu.o : %.fpo
	@mkdir -p $(dir $(@))


mm : $(PPU_TARGET)
	@mkdir -p $(BIN)
	@$(PPU_STRIP) -s $< -o $(OBJS_DIR)/$(PPU_TARGET)

	@$(MAKE_SELF_NPDRM) ./objs/$(MM_REL)_BOOT.elf $(RELEASE)$(NPDRM)/USRDIR/EBOOT.BIN $(CONTENT_ID) > nul
	@rm $(PPU_TARGET)
	$(MAKE) -f makefile.multiman mm