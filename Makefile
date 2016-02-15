CC=gcc -g
LIBS = -lz -l:libmagic.so.1.0.0
OBJECTS= unzip.o unzip_utility.o


CFLAGS=-Wall -I./include/
DEMO=extractArch

all: $(DEMO)

clean:
	$(RM) *.o $(DEMO) $(TEST)

$(DEMO): unzip_utility.o unzip.o type.o extractArch.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@ -L/usr/lib/x86_64-linux-gnu/

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@