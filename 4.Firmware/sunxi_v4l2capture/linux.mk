#平台是否支持C++
#Makefile全局参数
MAKEFLAGS += -rR --no-print-directory
#TOOL_CHAIN_HOST目标平台编译链
ifdef CROSS_COMPILE
TOOL_CHAIN = $(CROSS_COMPILE)-
endif

#makfile编译相关变量
export CC      := $(TOOL_CHAIN)gcc
export CXX	   := $(TOOL_CHAIN)g++
export LD      := $(TOOL_CHAIN)ld
export NM      := $(TOOL_CHAIN)nm
export AR      := $(TOOL_CHAIN)ar
export RM      := rm
export Q       := @
export MAKE    := make
export MKDIR   := mkdir
export CP	   := cp
export OBJCOPY := $(TOOL_CHAIN)objcopy
export STRIP   := $(TOOL_CHAIN)strip
export RM      := rm -rf
export ECHO    := echo
export INSTALL := /usr/bin/install

#全局C_FLAGS
DBG_CFLAGS := -g -fstack-protector -fstack-protector-all  -feliminate-unused-debug-types -Wno-int-to-pointer-cast

C_FLAGS += -O2 $(DBG_CFLAGS) -D_REENTRANT -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE -Wall \
		-pipe

#c版本
C_FLAGS += -ldl -lm -lpthread -lrt