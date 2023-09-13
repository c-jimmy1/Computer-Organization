CC = gcc
CFLAGS = -Wall
SRC = main.c alloc.c mult.c print.c read.c free.c
TARGET = main.exe

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) *.o
