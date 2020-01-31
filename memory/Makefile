CFLAGS = -std=c++17 -Iinclude -I$(HOME)/include -Itest -DDEBUG 
TEST_CFLAGS = $(CFLAGS) -DTEST
LDFLAGS = -Llib -lmemory

TEST_MAIN = test/test.cpp
TEST_SRC = $(filter-out $(TEST_MAIN), $(wildcard test/*.cpp))
SRC = $(wildcard src/*.cpp)
ALL_SRC = $(TEST_MAIN) $(TEST_SRC) $(SRC)

LIB_OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))
TEST_OBJ = $(patsubst test/%.cpp, build/%.o, $(TEST_MAIN) $(TEST_SRC))
OBJ = $(LIB_OBJ) $(TEST_OBJ)

SRC = $(wildcard src/*.cpp)

BUILD_DIRS = build bin lib

TEST_EXE = bin/test

LIB = lib/libmemory.a

all: .depend $(BUILD_DIRS) $(OBJ) $(LIB)

depend: .depend 
	@echo > /dev/null

.depend: $(SRC) $(TEST_SRC)
	@g++ $(CFLAGS) -MM $^ > ./.depend;
	@sed -i -r 's/(.+)\.o/build\/\1.o/' .depend

include .depend

$(LIB): $(LIB_OBJ)
	ar crf $@ $^

$(BUILD_DIRS):
	@mkdir -p $@

test: $(BUILD_DIRS) $(TEST_EXE)
	@$(TEST_EXE)

$(TEST_EXE): $(TEST_OBJ) $(LIB)
	g++ -g -Wall $(CFLAGS) -o bin/test $^

#I should be able to merge the OBJ targets, no?
$(LIB_OBJ): build/%.o: src/%.cpp
	g++ -g -Wall $(CFLAGS) -o $@ -c $<

$(TEST_OBJ): build/%.o: test/%.cpp
	g++ -g -Wall $(CFLAGS) -o $@ -c $<

clean:
	@rm -rf $(BUILD_DIRS) .depend

.PHONY: depend test clean
