SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp,build/%.o,$(SRC))
TEST_SRC = $(wildcard test/*.cpp)
TEST_OBJ = $(patsubst test/%.cpp,build/test/%.o,$(TEST_SRC))
HEADERS = $(wildcard include/*.hpp)

DIR = $(shell pwd)

CFLAGS = -std=c++17 -I$(DIR)/include -I$(HOME)/include -Itest -DDEBUG 
TEST_CFLAGS = $(CFLAGS) -DTEST
LDFLAGS = -Llib -lmemory

BUILD_DIRS = build bin lib build/test
LIB = lib/libmemory.a

TEST_EXE = bin/test

all: $(BUILD_DIRS) $(LIB)

depend: .depend 
	@echo > /dev/null

.depend: $(SRC) $(TEST_SRC) $(HEADERS)
	g++ $(CFLAGS) -MM $^ > ./.depend;
	sed -i -r 's/(.+)\.o/build\/\1.o/' .depend

test: $(BUILD_DIRS) $(TEST_EXE)
	@$(TEST_EXE)

$(TEST_EXE): $(TEST_OBJ) $(LIB)
	g++ -g -Wall $(CFLAGS) -o bin/test $^

$(BUILD_DIRS):
	@mkdir -p $@

$(LIB): $(OBJ)
	ar crf $@ $^

$(OBJ): build/%.o: src/%.cpp
	g++ -g -Wall $(CFLAGS) -o $@ -c $^

$(TEST_OBJ): build/test/%.o: test/%.cpp
	g++ -g -Wall $(TEST_CFLAGS) -o $@ -c $^

clean:
	@rm -rf $(BUILD_DIRS)

.PHONY: test clean all depend
