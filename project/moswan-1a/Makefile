TARGET = program

all: $(TARGET)

$(TARGET): $(addsuffix .cpp, $(TARGET))
	g++ -o $(TARGET) -Wall -g $(addsuffix .cpp, $(TARGET))

clean:
	rm $(TARGET)

