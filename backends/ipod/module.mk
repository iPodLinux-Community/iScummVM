MODULE := backends/ipod

MODULE_OBJS := \
	events.o \
	graphics.o \
	sdl.o \
	cop.o

MODULE_DIRS += \
	backends/ipod/

# We don't use the rules.mk here on purpose
OBJS := $(addprefix $(MODULE)/, $(MODULE_OBJS)) $(OBJS)
