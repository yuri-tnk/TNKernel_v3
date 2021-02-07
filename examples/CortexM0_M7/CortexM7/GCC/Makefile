#
# Makefile for TNKernel examples
# Processor: stm32f746
# Example:   ToDo
#
# Compiler: gcc  (GNU Tools for ARM Embedded Processors 5.3 2016q1)
#
#_________________________________________________________________________________________
#

FORMAT   = ihex
IMAGE    = test_gcc
CPU		= cortex-m7

PRJ_DIR         = c:/Yuri/Projects/tnkernel_3/examples/CortexM0_M7/CortexM7/GCC
TOOLSROOT       = c:/"Program Files (x86)/GNU Tools ARM Embedded/8 2019-q3-update"
# TOOLSROOT = c:\"Program Files"\"GNU Tools ARM Embedded"\"5.3 2016q1"

TOOLS           = $(TOOLSROOT)/bin/arm-none-eabi-
OBJ_DIR         = $(PRJ_DIR)/Obj
LSTPATH         = $(PRJ_DIR)/Lst

# includes dirs

INCLUDES = -I ./ -I $(PRJ_DIR)/.. \
           -I $(PRJ_DIR)/../.. \
           -I $(PRJ_DIR)/../../../../os\
           -I $(PRJ_DIR)/../../../../common_cortex/inc\
           -I $(PRJ_DIR)/../../../../common

# source dirs

PRJ_SRC_DIR        = $(PRJ_DIR)/..
COMMON_SRC_DIR     = $(PRJ_DIR)/../..
OS_SRC_DIR         = $(PRJ_DIR)/../../../../os
OS_PORT_SRC_DIR    = $(PRJ_DIR)/../../../../os/CortexM0_M7
COMMON_CORTEX_DIR  = $(PRJ_DIR)/../../../../common_cortex\src
COMMON_SRC_DIR1    = $(PRJ_DIR)/../../../../common

LDSCRIPT         = $(COMMON_CORTEX_DIR)/stm32f746_gcc.ld

VPATH = $(PRJ_DIR) $(PRJ_SRC_DIR) $(COMMON_SRC_DIR) $(OS_PORT_SRC_DIR)\
        $(OS_SRC_DIR) $(COMMON_CORTEX_DIR) $(COMMON_SRC_DIR1)

#_________________________________________________________________________________________
#
# List of objects
#

OBJECTS = \
  $(OBJ_DIR)/tn_stm32f746_Startup_gcc.o\
  $(OBJ_DIR)/project_main.o\
  $(OBJ_DIR)/prj_shell_func.o\
  $(OBJ_DIR)/shell.o\
  $(OBJ_DIR)/stm32f4xx_dma.o\
  $(OBJ_DIR)/stm32f4xx_gpio.o\
  $(OBJ_DIR)/stm32f7_uart.o\
  $(OBJ_DIR)/system_stm32f746.o\
  $(OBJ_DIR)/tn.o\
  $(OBJ_DIR)/tn_alloc.o\
  $(OBJ_DIR)/tn_dqueue.o\
  $(OBJ_DIR)/tn_event.o\
  $(OBJ_DIR)/tn_mailbox.o\
  $(OBJ_DIR)/tn_mem.o\
  $(OBJ_DIR)/tn_mutex.o\
  $(OBJ_DIR)/tn_port_CortexM0_M7.o\
  $(OBJ_DIR)/tn_port_CortexM0_M7_asm.o\
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

ASMOPTS = -mcpu=$(CPU) -mthumb --gdwarf-2 -mthumb-interwork
CPPOPTS = 
ASMLST = -alhms=$(LSTPATH)/$(notdir $(basename $@).lst)

#-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard

#-- Compiler  flags

CCOPTS  = $(INCLUDES) -c -mcpu=cortex-m7 -mthumb -mfpu=fpv5-sp-d16\
          -mfloat-abi=hard -Wall -Os -fdata-sections -ffunction-sections\
          -DUSE_CORTEX_M7

#-- Linker flags

LDFLAGS = -T$(LDSCRIPT) -mcpu=cortex-m7 -mthumb -mfpu=fpv5-sp-d16\
           -mfloat-abi=hard -Wl,-Map=$(OBJ_DIR)/$(IMAGE).map -nostartfiles

# -Wl,--gc-sections

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


