#TODO Really need to sort these flags out

CFLAGS = -std=c++17 -I$(VULKAN_SDK_PATH)/include -Iinclude -I$(HOME)/include -DDEBUG
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan

#main is a special snowflake, mostly for testing and the main function weirdness
ALL_SRC = $(shell find src -name "*.cpp")
SRC = $(filter-out src/main.cpp, $(ALL_SRC))
ALL_OBJ = $(patsubst src/%.cpp, build/%.o, $(ALL_SRC))
OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))

TEST_SRC = $(shell find test -name "*.cpp")
TEST_OBJ = $(patsubst test/%.cpp, build/test_%.o, $(TEST_SRC))
TEST_BIN = bin/main_test_package

WAVEFRONT_SRC = $(wildcard models/wavefront/*.obj)
WAVEFRONT_BIN = $(patsubst models/wavefront/%.obj, assets/models/%.bin, $(WAVEFRONT_SRC))
WAVEFRONT_TOOL = tools/bin/wavefrontparser
WAVEFRONT_BLD = build/wavefrontparser
WAVEFRONT_PARSER = tools/bin/wavefrontparser

SHADER_TYPES = frag vert
SHADER_SRC = $(foreach type, $(SHADER_TYPES), $(wildcard shaders/*.$(type)))
SHADER_SPV = $(patsubst shaders/%, assets/shaders/%.spv, $(SHADER_SRC))

SPIKE_SRC = $(wildcard spike/*.cpp)
SPIKE_BIN = $(patsubst spike/%.cpp, bin/spike_%, $(SPIKE_SRC))

BUILD_DIRS = $(sort $(dir $(ALL_OBJ) $(WAVEFRONT_BIN) $(SHADER_SPV)) bin)

EXE = bin/game

all: $(BUILD_DIRS) $(EXE) $(WAVEFRONT_BIN) shaders

test: $(BUILD_DIRS) $(TEST_BIN)
	$(TEST_BIN)

$(TEST_BIN): $(OBJ) $(TEST_OBJ)
	g++ -g $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_OBJ): build/test_%.o: test/%.cpp
	g++ -g $(CFLAGS) -o $@ -c $^ $(LDFLAGS)

shaders: $(BUILD_DIRS) $(SHADER_SPV)	

$(SHADER_SPV): assets/shaders/%.spv: shaders/%
	$(VULKAN_SDK_PATH)/bin/glslangValidator -V -o $@ $^

$(EXE): $(ALL_OBJ)
	g++ -g $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(ALL_OBJ): build/%.o: src/%.cpp
	g++ -g $(CFLAGS) -o $@ -c $^ $(LDFLAGS)

spike: $(BUILD_DIRS) $(OBJ) $(SPIKE_BIN)

bin/spike_%: spike/%.cpp
	g++ -g $(CFLAGS) -o $@ $^ $(OBJ) $(LDFLAGS)

$(BUILD_DIRS):
	@mkdir -p $@

wavefront: $(WAVEFRONT_BIN)

$(WAVEFRONT_BIN): assets/models/%.bin: models/wavefront/%.obj $(WAVEFRONT_TOOL)
	$(WAVEFRONT_PARSER) $(filter-out $(WAVEFRONT_TOOL), $^) $@
	@touch $(WAVEFRONT_BLD)

$(WAVEFRONT_TOOL): $(WAVEFRONT_BLD)
	$(MAKE) -C tools wavefrontparser

$(WAVEFRONT_BLD):

clean:
	rm -rf $(BUILD_DIRS)
	@$(MAKE) -C tools clean

.PHONY: all wavefront shaders spike
