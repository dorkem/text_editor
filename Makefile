CC=gcc
CFLAGS=-I"C:\Users\cjh\Desktop\20233144"
LDFLAGS=-L"C:\Users\cjh\Desktop\20233144" -lpdcurses

TARGET=20233144
SRC=20233144.c
OBJ=$(SRC:.c=.o)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	del $(OBJ) $(TARGET)