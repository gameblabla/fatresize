PRGNAME     = fatresize_hc
CC          = /opt/bittboy-toolchain/bin/arm-buildroot-linux-musleabi-gcc

INCLUDES	= -I. -Iinclude

CFLAGS		= -O2 -fsingle-precision-constant -fno-PIC -flto -fno-common -Wall $(INCLUDES)
LDFLAGS     = -Wl,--start-group -lc -lgcc -lm -luuid -lparted -lparted-fs-resize  -flto -Wl,--end-group -s 

# Files to be compiled
SRCDIR 		=  ./src ./src/fat ./src/hfs
#SRCDIR 		=  ./src
VPATH		= $(SRCDIR)
SRC_C		= $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.c))
OBJ_C		= $(notdir $(patsubst %.c, %.o, $(SRC_C)))
OBJS		= $(OBJ_C) 

# Rules to make executable
$(PRGNAME): $(OBJS)  
	$(CC) $(CFLAGS) -std=gnu99 -o $(PRGNAME) $^ $(LDFLAGS)
	
clean:
	rm -f $(PRGNAME)$(EXESUFFIX) *.o
