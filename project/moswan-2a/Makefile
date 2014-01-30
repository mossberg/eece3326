CC = g++
FLAGS = -Wall -g
TARGET = program
HEADER = $(wildcard *.h)


all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(FLAGS) $(TARGET).cpp -o program

clean:
	rm -v $(TARGET)
	rm -rfv $(TARGET).dSYM
