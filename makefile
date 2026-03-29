CC = gcc
DEBUG = -g
CFLAGS = -Wall $(DEBUG)

TARGET = lab3
OBJS = lab3.o

.SUFFIXES: .o .c

%.o: %.c
	$(CC) $(CFLAGS) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

clean:
	rm -f *.o $(TARGET)

