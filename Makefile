CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = myPreprocessor
SRC = preprocessor.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) inputfile.i helpfile.i

run: $(TARGET)
	./$(TARGET) test.c

.PHONY: all clean run
