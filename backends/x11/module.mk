MODULE := backends/x11

MODULE_OBJS := \
	x11.o

MODULE_DIRS += \
	backends/x11/

# We don't use the rules.mk here on purpose
OBJS := $(addprefix $(MODULE)/, $(MODULE_OBJS)) $(OBJS)