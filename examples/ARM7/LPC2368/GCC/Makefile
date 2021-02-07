#
# Makefile for TNKernel 'example_ARM' project
# Processor: LPC2368
#
# Compiler: gcc  (Sourcery_CodeBench_Lite_for_ARM_EABI)
#
#_________________________________________________________________________________________
#

FORMAT   = ihex
IMAGE    = example_ARM_LPC2368
           
PRJ_DIR   = c:/Yuri/projects/New_TNKernel/tnkernel_3/examples/ARM7/LPC2368/GCC
TOOLSROOT = c:\Sourcery_CodeBench_Lite_for_ARM_EABI

TOOLS           = $(TOOLSROOT)/bin/arm-none-eabi-
OBJ_DIR         = $(PRJ_DIR)/Obj
LSTPATH         = $(PRJ_DIR)/Lst

# includes dirs

INCLUDES = -I ./ -I $(PRJ_DIR)/.. \
           -I $(PRJ_DIR)/../.. \
           -I $(PRJ_DIR)/../../../../os\
           -I $(PRJ_DIR)/../../../../common_arm/inc\
           -I $(PRJ_DIR)/../../../../common\
           -I $(PRJ_DIR)/../../../../tests/test_common

# source dirs

PRJ_SRC_DIR         = $(PRJ_DIR)/..
COMMON_SRC_DIR      = $(PRJ_DIR)/../..
OS_SRC_DIR          = $(PRJ_DIR)/../../../../os
OS_PORT_SRC_DIR     = $(PRJ_DIR)/../../../../os/ARM
COMMON_ARM_DIR      = $(PRJ_DIR)/../../../../common_arm\src
COMMON_SRC_DIR1     = $(PRJ_DIR)/../../../../common
TEST_COMMON_SRC_DIR = $(PRJ_DIR)/../../../../tests/test_common 

LDSCRIPT = $(COMMON_ARM_DIR)/lpc2368_gcc.ld

VPATH = $(PRJ_DIR) $(PRJ_SRC_DIR) $(COMMON_SRC_DIR) $(OS_PORT_SRC_DIR)\
        $(OS_SRC_DIR) $(COMMON_ARM_DIR) $(COMMON_SRC_DIR1) $(TEST_COMMON_SRC_DIR)

#_________________________________________________________________________________________
#
# List of objects
#

OBJECTS = \
  $(OBJ_DIR)/tnkernel_startup_gcc_LPC23xx.o\
  $(OBJ_DIR)/startup_hardware_init_LPC236x.o\
  $(OBJ_DIR)/system_lpc236x.o\
  $(OBJ_DIR)/lpc236x_uart.o\
  $(OBJ_DIR)/prj_shell_func.o\
  $(OBJ_DIR)/project_main.o\
  $(OBJ_DIR)/shell.o\
  $(OBJ_DIR)/tn.o\
  $(OBJ_DIR)/tn_alloc.o\
  $(OBJ_DIR)/tn_dqueue.o\
  $(OBJ_DIR)/tn_event.o\
  $(OBJ_DIR)/tn_mailbox.o\
  $(OBJ_DIR)/tn_mem.o\
  $(OBJ_DIR)/tn_mutex.o\
  $(OBJ_DIR)/tn_port_arm.o\
  $(OBJ_DIR)/tn_port_arm_asm.o\
  $(OBJ_DIR)/tn_sem.o\
  $(OBJ_DIR)/tn_sys.o\
  $(OBJ_DIR)/tn_task.o\
  $(OBJ_DIR)/tn_timer.o\
  $(OBJ_DIR)/tn_user.o\
  $(OBJ_DIR)/tn_utils.o


DEP_FILE =  $(OBJ_DIR)/$(notdir $(basename $@).d)

#_________________________________________________________________________________________
#
#  Tools, tools options(flags)
#

CC      = $(TOOLS)gcc.exe
AS      = $(TOOLS)as.exe
CP	     = $(TOOLS)cpp.exe
CPP     = $(TOOLS)g++.exe
LD      = $(TOOLS)ld.exe
OBJCOPY = $(TOOLS)objcopy.exe
OBJDUMP = $(TOOLS)objdump.exe
SIZE    = $(TOOLS)size.exe
NM      = $(TOOLS)nm.exe
REMOVE  = rm
COPY    = copy


#-- Assembler flags

ASMOPTS = --traditional-format -march=armv4t -EL
ASMLST  = -alhms=$(LSTPATH)/$(notdir $(basename $@).lst)

#-- Compiler  flags

OPTFLAGS = -mcpu=arm7tdmi

CCOPTS  = $(OPTFLAGS) $(INCLUDES) -c \
    -Wall -Wpointer-arith -Wswitch \
    -Wreturn-type -Wshadow -Wstrict-prototypes -Wunused \
    -Os  -std=c99  -march=armv4t -mlittle-endian -fno-builtin \
    -D TNKERNEL_PORT_ARM
# -Wredundant-decls -Wnested-externs

#-- Linker flags

LDFLAGS =  -nostartfiles -Wl,-Map=$(OBJ_DIR)/$(IMAGE).map,--cref  -lc \
           $(NEWLIBLPC) -lm -lc -lgcc -T$(LDSCRIPT)

#--- Compiling/Assembling ---

$(OBJ_DIR)/%.o: %.s
	$(AS) $(ASMOPTS) $< $(ASMLST) -o $@
	
$(OBJ_DIR)/%.o:  %.c 
	$(CC) $(CCOPTS) $< -MD -MF $(DEP_FILE) -o $@

$(OBJ_DIR)/%.o:  %.S 
	$(CC) $(CCOPTS) $< -MD -MF $(DEP_FILE) -o $@

include  $(wildcard   $(OBJ_DIR)/$(notdir $(basename *.*).d))

#-- Build & Linking ---

all: $(PRJ_DIR)/$(IMAGE).elf

$(PRJ_DIR)/$(IMAGE).elf: $(OBJECTS)

	$(CC) $(OBJECTS) $(LDFLAGS) --output $(OBJ_DIR)/$(notdir $@)
	$(OBJCOPY) -O$(FORMAT)    $(OBJ_DIR)/$(IMAGE).elf  $(OBJ_DIR)/$(IMAGE).hex

#--- Display segment's size

	$(SIZE) -A $(OBJ_DIR)/$(IMAGE).elf

# ===========================================================================

.PHONY:   clean

clean:

	$(REMOVE) -f $(LSTPATH)/*.lst 
	$(REMOVE) -f $(OBJ_DIR)/*.*

# ===========================================================================
# ===========================================================================
# ===========================================================================


