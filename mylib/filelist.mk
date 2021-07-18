###################################################
# LIBRARY SOURCES
###################################################

# Set mylib folder path.
# IMPORTANT NOTE: change MYLIBTEMPLATE_PATH to your Driver name e.g. JOYSTICK_MYLIBPATH
# e.g. JOYSTICK_MYLIBPATH=/holme/csse3010/mylib/joystick
MYLIBTEMPLATE_PATH=$(HOME)/csse3010/mylib

# Set folder path with header files to include.
CFLAGS += -I$(MYLIBTEMPLATE_PATH)

# List all c files locations that must be included (use space as separate e.g. LIBSRCS += path_to/file1.c path_to/file2.c)
LIBSRCS += $(MYLIBTEMPLATE_PATH)/s443396_reg_iss.c
LIBSRCS += $(MYLIBTEMPLATE_PATH)/s443396_reg_lta1000g.c
LIBSRCS += $(MYLIBTEMPLATE_PATH)/s443396_reg_pb.c
LIBSRCS += $(MYLIBTEMPLATE_PATH)/s443396_reg_pantilt.c
LIBSRCS += $(MYLIBTEMPLATE_PATH)/s443396_reg_joystick.c
LIBSRCS += $(MYLIBTEMPLATE_PATH)/s443396_reg_keypad.c
LIBSRCS += $(MYLIBTEMPLATE_PATH)/s443396_reg_oled.c

LIBSRCS += $(MYLIBTEMPLATE_PATH)/s443396_lib_hamming.c

LIBSRCS += $(MYLIBTEMPLATE_PATH)/s443396_tsk_oled.c
LIBSRCS += $(MYLIBTEMPLATE_PATH)/s443396_tsk_joystick.c
LIBSRCS += $(MYLIBTEMPLATE_PATH)/s443396_tsk_pb.c
LIBSRCS += $(MYLIBTEMPLATE_PATH)/s443396_tsk_keypad.c

# project tsk files
LIBSRCS += $(MYLIBTEMPLATE_PATH)/s443396_tsk_CAG_display.c
LIBSRCS += $(MYLIBTEMPLATE_PATH)/s443396_tsk_CAG_simulator.c
LIBSRCS += $(MYLIBTEMPLATE_PATH)/s443396_tsk_CAG_keypad_grid.c
LIBSRCS += $(MYLIBTEMPLATE_PATH)/s443396_tsk_CAG_keypad_mnemonic.c
LIBSRCS += $(MYLIBTEMPLATE_PATH)/s443396_tsk_CAG_joystick.c


