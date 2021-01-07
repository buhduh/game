$(CFLAGS) := $(CFLAGS)
$(LDFLAGS) := $(LDFLAGS)

SRC = $(wildcard */*.cpp)
OBJ = $(SRC:%.cpp=build/%.o)
TLIBS = $(sort $(dir $(OBJ)))
LIBS = $(TLIBS:build/%/=lib/lib%.a)
BUILD_DIRS = lib build $(TLIBS)

all: $(BUILD_DIRS) $(OBJ) $(LIBS)

$(LIBS): lib/lib%.a: %
	ar crf $@ build/$</*.o

$(OBJ): build/%.o: %.cpp
	g++ -g $(CFLAGS) -I$(dir $<) -o $@ -c $<

$(BUILD_DIRS):
	@mkdir -p $@

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIRS)
