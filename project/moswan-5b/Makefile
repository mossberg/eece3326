CC = g++
FLAGS = -Wall -g
TARGET = p5b
HEADER = $(wildcard *.h)


all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) -o $(TARGET) $(FLAGS) $(TARGET).cpp

clean:
	rm -v $(TARGET)
	rm -rfv $(TARGET).dSYM
