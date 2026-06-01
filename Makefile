CXX = g++
CXXFLAGS = -std=c++98 -Wall -Wextra -O2
TARGET = retailforge
SOURCES = main.cpp product.cpp supplier.cpp salestransaction.cpp inventorymanager.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
