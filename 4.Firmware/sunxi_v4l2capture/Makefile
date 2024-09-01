APP_VERSION = 1.0.0
# common
CURDIR = $(shell pwd)
export ROOT_PATH = $(CURDIR)

#misc
BUILD_COMPLETE_STRING ?= $(shell date "+%a, %d %b %Y %T %z")
UID := $(shell id -u)
GID := $(shell id -g)


OUTPUT_DIR= $(CURDIR)/output

# cross host
CROSS_COMPELTE_HOST = 
export $(CROSS_COMPELTE_HOST)


CROSS_COMPILE=$(CROSS_COMPELTE_HOST)
include $(ROOT_PATH)/linux.mk

INC_PATH = $(CURDIR)/inc
SRC_PATH = $(CURDIR)/src
LIB_PATH = $(CURDIR)/lib
OBJDIR =  $(CURDIR)/obj

LD_C_FLAGS   += -fno-aggressive-loop-optimizations  -ffunction-sections -fdata-sections -ftree-vectorize -Wall -Wno-unused-function -Wno-unused-variable -Wno-unused-but-set-variable -O2 -fPIC -g -Wl,-gc-sections
LD_CPP_FLAGS +=  -ldl -lm -lpthread -lrt  -lstdc++ 


.PHONY: all clean

BIN = sunxi_capture

checkenv:
	@if [ ! -e $(OUTPUT_DIR)/ ]; then \
		mkdir $(OUTPUT_DIR); \
	fi






SRC_CPP += $(foreach dir, $(SRC_PATH), $(wildcard $(dir)/*.cpp))   
SRC_C += $(foreach dir, $(SRC_PATH), $(wildcard $(dir)/*.c))

OBJS_CPP:= $(patsubst %.cpp, %.o, $(SRC_CPP))
OBJS_C  := $(patsubst %.c, %.o,  $(SRC_C))

CPP_DEPS  := $(OBJS_CPP:.o=.d)
C_DEPS  := $(OBJS_C:.o=.d)

all: $(BIN)

$(BIN) : $(OBJS_C) $(OBJS_CPP) checkenv
	$(Q)echo -e $@
	$(CC) -o $(OUTPUT_DIR)/$(BIN) ${OBJDIR}/*.o  ${LD_C_FLAGS} -L ${LIB_PATH} -I ${INC_PATH}

$(OBJS_CPP): %.o : %.cpp
	$(Q)@test -d $(OBJDIR) | mkdir -p $(OBJDIR)
	$(Q)echo -e "\033[35m CREATE $(notdir $@)\033[0m"
	$(Q)$(CXX) -c $(INC_PATH) $(C_FLAGS) $(LD_CPP_FLAGS) $(<) -o $@ 
	$(Q)mv $*.o $(OBJDIR) 

#c后缀文件编译.o
$(OBJS_C): %.o : %.c
	$(Q)@test -d $(OBJDIR) | mkdir -p $(OBJDIR)
	$(Q)echo -e "\033[35m CREATE $(notdir $@)\033[0m"
	$(Q)$(CC) -c -I $(INC_PATH) $(C_FLAGS) $(LD_C_FLAGS) $(<) -o $@ 
	$(Q)mv $@ $(OBJDIR)


clean:
	@$(ECHO) "RM  $(OUTPUT_DIR) $(OBJDIR)"
	@$(RM)  $(OUTPUT_DIR)
	@$(RM)  $(OBJDIR)
