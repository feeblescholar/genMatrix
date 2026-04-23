
# Memtrace helye (mindkét build használja)
MEMTRACE = lib/memtrace_gtest/memtrace.cpp

CCX = g++

########### Végleges build ###########

SRCS = $(wildcard src/*.cpp) $(MEMTRACE)
OBJ  = $(patsubst %.cpp, build/obj/%.o, $(SRCS))
TARGET = build/genMatrix

CPPFLAGS = -DMEMTRACE # A MEMTRACE makró kell hogy működjön
CCXFLAGS = -std=c++17 -Wall -Werror -Wextra -pedantic # nem engedünk minden csodát lefordítani

######################################

############ Teszt build #############

# GoogleTest kellékei (csak a teszthez)
GTEST_DIR = lib/googletest/googletest
GTEST_BUILD_DIR = lib/googletest/build/lib
GTEST_ARCHIVE = $(GTEST_BUILD_DIR)/libgtest.a

TESTSRCS = $(wildcard test/*.cpp) src/matrix_error.cpp $(MEMTRACE)
TESTOBJ = $(patsubst %.cpp, build/testobj/%.o, $(TESTSRCS))
TESTTARGET = build/genMatrix_gtest

TESTCPPFLAGS = -DMEMTRACE -I$(GTEST_DIR)/include
TESTCCXFLAGS = -g -O0 -std=c++17 -Wall -Wextra -pthread

######################################

vpath %.cpp src test lib/memtrace_gtest

all: prod_build test_build

prod_build: $(TARGET)
test_build: $(TESTTARGET)

$(TARGET): $(OBJ)
	mkdir -p build
	$(CCX) $(OBJ) -o $(TARGET) $(CCXFLAGS) $(CPPFLAGS)

$(GTEST_ARCHIVE):
		mkdir -p lib/googletest/build; \
		cd lib/googletest/build && cmake .. && make -j4;

$(TESTTARGET): $(TESTOBJ) $(GTEST_ARCHIVE)
	mkdir -p build
	$(CCX) $(TESTOBJ) -o $(TESTTARGET) $(TESTCCXFLAGS) $(TESTCPPFLAGS) $(GTEST_ARCHIVE) $(GTEST_BUILD_DIR)/libgtest_main.a


build/obj/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CCX) -c $< -o $@ $(CCXFLAGS) $(CPPFLAGS)

build/testobj/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CCX) -c $< -o $@ $(TESTCCXFLAGS) $(TESTCPPFLAGS)

clean:
	rm -rf build
	rm -rf lib/googletest/build