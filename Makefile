CC=gcc
CFLAGS=-I./

# 기본 LDFLAGS 설정
LDFLAGS=-L./

TARGET=20233144
SRC=20233144.c

OBJ=$(SRC:.c=.o)
# 운영 체제 확인
ifeq ($(OS),Windows_NT)
    RM = del /Q
    LDFLAGS += -lpdcurses
    
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

else
    RM = rm -f
    LDFLAGS += -lncurses    
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $<

endif


clean:
	$(RM) $(OBJ) $(TARGET)