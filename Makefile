# Memtrace helye (mindkét build használja)
MEMTRACE = lib/memtrace_gtest/memtrace.cpp

CXX = g++

########### Végleges build ###########

SRCS = $(wildcard src/*.cpp) $(MEMTRACE)
OBJ  = $(patsubst %.cpp, build/obj/%.o, $(SRCS))
TARGET = build/genMatrix

CPPFLAGS = -DMEMTRACE # A MEMTRACE makró kell hogy működjön
CXXFLAGS = -std=c++17 -O3 -Wall -Werror -Wextra -pedantic # nem engedünk minden csodát lefordítani

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
TESTCXXFLAGS = -g -O2 -std=c++17 -Wall -Wextra -pthread

######################################

####### OS, arch vektorizálás ########

ifeq ($(shell uname -s),Darwin)
    ifeq ($(shell uname -m),arm64)
        CXXFLAGS += -mcpu=native
		TESTCXXFLAGS += -mcpu=native
    endif
else
    CXXFLAGS += -mavx2 -mfma
	TESTCXXFLAGS += -mavx2 -mfma
endif

######################################

vpath %.cpp src test lib/memtrace_gtest

all: prod_build test_build

prod_build: $(TARGET)
test_build: $(TESTTARGET)

$(TARGET): $(OBJ)
	mkdir -p build
	$(CXX) $(OBJ) -o $(TARGET) $(CXXFLAGS) $(CPPFLAGS)

$(GTEST_ARCHIVE):
		mkdir -p lib/googletest/build; \
		cd lib/googletest/build && cmake .. && make -j4;

$(TESTTARGET): $(TESTOBJ) $(GTEST_ARCHIVE)
	mkdir -p build
	$(CXX) $(TESTOBJ) -o $(TESTTARGET) $(TESTCXXFLAGS) $(TESTCPPFLAGS) $(GTEST_ARCHIVE) $(GTEST_BUILD_DIR)/libgtest_main.a


build/obj/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(CPPFLAGS)

build/testobj/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $< -o $@ $(TESTCXXFLAGS) $(TESTCPPFLAGS)

clean:
	rm -rf build
	rm -rf lib/googletest/build