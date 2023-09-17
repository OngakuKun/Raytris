.PHONY: all clean echoes

PROJECT_NAME           ?=Raytris
RAYLIB_VERSION         ?=4.5.0
RAYLIB_PATH            ?=C:/raylib/raylib
COMPILER_PATH          ?=C:/raylib/w64devkit/bin
BUILD_MODE             ?=DEBUG
EXT                    ?=.exe
PLATFORM               ?=PLATFORM_DESKTOP

DESTDIR                ?=/usr/local
RAYLIB_INSTALL_PATH    ?=$(DESTDIR)/lib
RAYLIB_H_INSTALL_PATH  ?=$(DESTDIR)/include
RAYLIB_LIBTYPE         ?=STATIC
USE_EXTERNAL_GLFW      ?=FALSE
USE_WAYLAND_DISPLAY    ?=FALSE

RAYLIB_RELEASE_PATH    ?=$(RAYLIB_PATH)/src
EXAMPLE_RUNTIME_PATH   ?=$(RAYLIB_RELEASE_PATH)

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
	ifeq ($(OS),Windows_NT)
		PLATFORM_OS=WINDOWS
		export PATH := $(COMPILER_PATH):$(PATH)

		CC             =gcc
		MAKE           =make

		CFLAGS        +=-Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces

		LDFLAGS        =-L. -L$(RAYLIB_RELEASE_PATH) -L$(RAYLIB_PATH)/src
		LDFLAGS       +=$(RAYLIB_PATH)/src/raylib.rc.data

		ifeq ($(BUILD_MODE),RELEASE)
			# -Wl,--subsystem,windows hides the console window
			LDFLAGS   +=-Wl,--subsystem,windows
		endif

		ifeq ($(BUILD_MODE),DEBUG)
			CFLAGS    += -g -O0
		else
			CFLAGS    += -s -O1
		endif

		LDLIBS = -lraylib -lopengl32 -lgdi32 -lwinmm

		INCLUDE_PATHS  = -I./src -I$(RAYLIB_PATH)/src -I$(RAYLIB_PATH)/src/external
	else
		UNAMEOS=$(shell uname)
		ifeq ($(UNAMEOS),Linux)
			PLATFORM_OS          =LINUX

			RAYLIB_PREFIX ?= ..
			RAYLIB_PATH    = $(realpath $(RAYLIB_PREFIX))

			CC                   =gcc
			MAKE                 =make
			EXT                  =

			CFLAGS              +=-Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces
			
			LDFLAGS              = -L. -L/usr/local/lib
			LDFLAGS = -L. -L$(RAYLIB_INSTALL_PATH) -L$(RAYLIB_RELEASE_PATH)

			ifeq ($(BUILD_MODE),DEBUG)
				CFLAGS          += -g -O0
			else
				CFLAGS          += -s -O1
			endif

			LDLIBS               = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

			INCLUDE_PATHS        =-I./src -I/usr/local/include/raylib

		endif
	endif
endif

SRC_DIR = src
OBJ_DIR = bin/obj

SRC = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*.h)
OBJS = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

ifeq ($(PLATFORM),PLATFORM_ANDROID)
	MAKEFILE_PARAMS =-f Makefile.Android
	export PROJECT_NAME
	export SRC_DIR
else
	MAKEFILE_PARAMS =$(PROJECT_NAME)
endif

all:
	$(MAKE) $(MAKEFILE_PARAMS)

$(PROJECT_NAME): $(OBJS)
	$(CC) -o bin/$(PROJECT_NAME)$(EXT) $(OBJS) $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D$(BUILD_MODE) -D$(PLATFORM)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM)

clean:
	$(RM) -r bin/$(PROJECT_NAME)$(EXT) bin/
	@echo Cleaning done

echoes:
	@printf "================================================================================\n"
	@printf "Build $(PROJECT_NAME)$(EXT) :\n-> $(PLATFORM)-$(BUILD_MODE)\n"
	@printf "SRC :\n-> $(SRC)\n"
	@printf "OBJS :\n-> $(OBJS)\n"
	@printf "INCLUDE_PATHS :\n-> $(INCLUDE_PATHS)\n"
	@printf "================================================================================"
