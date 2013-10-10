


#DEBUG  = -g -O0
DEBUG   = -O3
CC      = gcc
INCLUDE = -I/usr/local/include
CFLAGS  = $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS = -L/usr/local/lib
LDLIBS    = -lwiringPi -lpthread -lm


SRC	=	main.c bsp.c Display_COG_Process.c Display_Controller.c Display_Hardware_Driver.c AsciiImageConverter.c

OBJ	=	$(SRC:.c=.o)

APP	=	./epaper

all:	$(APP)

$(APP):	$(OBJ)
	$(CC) -o $(APP) $(OBJ) $(LDLIBS) $(INCLUDE) $(LDFLAGS)
.c.o:
	@echo [CC] $<
	@$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
	rm -f $(OBJ) *~ core tags $(APP)

tags:	$(SRC)
	@echo [ctags]
	@ctags $(SRC)

depend:
	makedepend -Y $(SRC)

# DO NOT DELETE
