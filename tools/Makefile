#Do your best to only need includes here
#TODO I should probably fix the debugging flags...

LIB_DIR = ../lib
LIBS = $(LIB_DIR)/libArtemis.a

CFLAGS := $(CFLAGS) -Iinclude
LDFLAGS :=  -L$(LIB_DIR) -lArtemis $(LDFLAGS)

BIN_DIR = bin

WAVEFRONTPARSER_BIN = bin/wavefrontparser
WAVEFRONTPARSER_BLD = build/wavefrontparser
WAVEFRONTPARSER_SRC = $(wildcard src/wavefrontparser/*.cpp)
WAVEFRONTPARSER_OBJ = $(patsubst src/wavefrontparser/%.cpp,build/wavefrontparser/%.o,$(WAVEFRONTPARSER_SRC))
WAVEFRONTPARSER_TEST_SRC = $(wildcard tests/src/wavefrontparser/*.cpp)
WAVEFRONTPARSER_TEST_OBJ = $(patsubst tests/src/wavefrontparser/%.cpp,build/wavefrontparser/%.o,$(WAVEFRONTPARSER_TEST_SRC))
WAVEFRONTPARSER_INC = -Iinclude/wavefrontparser

wavefrontparser: $(BIN_DIR) $(WAVEFRONTPARSER_BLD) $(WAVEFRONTPARSER_BIN)

$(BIN_DIR):
	mkdir $@

$(WAVEFRONTPARSER_BLD):
	@mkdir -p $@

$(WAVEFRONTPARSER_BIN): $(WAVEFRONTPARSER_OBJ) $(LIBS)
	g++ -g -Wall $(CFLAGS) -o $@ $^ $(LDFLAGS) 

$(WAVEFRONTPARSER_OBJ): build/wavefrontparser/%.o: src/wavefrontparser/%.cpp
	g++ -g -Wall $(CFLAGS) $(WAVEFRONTPARSER_INC) -o $@ -c $^

clean:
	@rm -rf build
	@rm -rf bin

test: $(WAVEFRONTPARSER_OBJ) $(WAVEFRONTPARSER_TEST_OBJ)
	g++ -g -Wall $(CFLAGS) -o bin/wavefront_test $^

$(WAVEFRONTPARSER_TEST_OBJ): build/wavefrontparser/%.o: tests/src/wavefrontparser/%.cpp
	g++ -g -Wall $(CFLAGS) -o $@ -c $^

.PHONY: clean test
