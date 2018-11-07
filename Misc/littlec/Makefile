CC      = gcc
CFLAGS  =-O
TARGET  = LittleC.exe
CORE    = LittleC.exe.stackdump

LittleC:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ *.c 

clean:
	rm $(TARGET) $(CORE) *~
