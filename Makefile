CCX = g++
CPPFLAGS = -DMEMTRACE
CCXFLAGS = -g -O0 -std=c++17 -Wall

SRCS = main.cpp memtrace_gtest/memtrace.cpp
OBJ  = $(SRCS:.cpp=.o)

TARGET = build/genMatrix_test

all: $(TARGET)

$(TARGET): $(OBJ)
	mkdir -p build
	$(CCX) $^ -o $@ $(CCXFLAGS) $(CPPFLAGS)

%.o: %.cpp
	$(CCX) -c $< -o $@ $(CCXFLAGS) $(CPPFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)