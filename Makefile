PRGNAME     = fatresize
CC          = /opt/retrostone-toolchain/bin/arm-buildroot-linux-musleabihf-gcc

INCLUDES	= -I. -Iinclude

CFLAGS		= -mcpu=cortex-a7 -mfpu=neon -O2 -fsingle-precision-constant -fno-PIC -flto -fno-common -Wall $(INCLUDES)
LDFLAGS     = -Wl,--start-group -lc -lgcc -lm -luuid -lparted -lparted-fs-resize  -flto -Wl,--end-group -s 

# Files to be compiled
SRCDIR 		=  ./src ./src/fat ./src/hfs
VPATH		= $(SRCDIR)
SRC_C		= $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.c))
OBJ_C		= $(notdir $(patsubst %.c, %.o, $(SRC_C)))
OBJS		= $(OBJ_C) 

# Rules to make executable
$(PRGNAME): $(OBJS)  
	$(CC) $(CFLAGS) -std=gnu99 -o $(PRGNAME) $^ $(LDFLAGS)
	
clean:
	rm -f $(PRGNAME)$(EXESUFFIX) *.o
