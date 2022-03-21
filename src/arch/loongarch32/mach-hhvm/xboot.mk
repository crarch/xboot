#
# Machine makefile
#

DEFINES		+=

ASFLAGS		:= -g -ggdb -Wall -D __ASSEMBLY__
CFLAGS		:= -g -ggdb -Wall
LDFLAGS		:= -T arch/$(ARCH)/$(MACH)/xboot.ld -nostdlib
# MCFLAGS		:= -march=loongarch32 -mabi=ilp32f
MCFLAGS		:= 

LIBDIRS		:=
LIBS 		:=
INCDIRS		:=
# SRCDIRS		:= arch/$(ARCH)/$(MACH)/driver/video
SRCDIRS		:= 

export arch/$(ARCH)/$(MACH)/sys-ccu-flags 		:= -fPIC
export arch/$(ARCH)/$(MACH)/sys-copyself-flags	:= -fPIC
export arch/$(ARCH)/$(MACH)/sys-dram-flags		:= -fPIC
export arch/$(ARCH)/$(MACH)/sys-spinor-flags	:= -fPIC
export arch/$(ARCH)/$(MACH)/sys-timer-flags		:= -fPIC
export arch/$(ARCH)/$(MACH)/sys-uart-flags		:= -fPIC
