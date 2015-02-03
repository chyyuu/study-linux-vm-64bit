SRCS = $(wildcard *.c)
        OBJS = $(SRCS:.c = .o)
        TARGET = $(shell pwd | rev | awk -F \/ '{print $$1}' | rev)
        CC = gcc
        CCFLAGS = -g -Wall -m32

all : $(OBJS)
	$(CC) $^  -o $(TARGET) $(CCFLAGS)

install:
	cp -f $(TARGET) ../../output/

%.o : %.c
	$(CC) -c $< $(CCFLAGS)
clean:
	rm $(TARGET)
