#TODO Really need to sort these flags out

DIR = $(shell pwd)
export CFLAGS = -std=c++17 -I$(VULKAN_SDK)/include -I$(DIR)/include -I$(HOME)/include -DDEBUG
export LDFLAGS = -L$(VULKAN_SDK)/lib `pkg-config --static --libs glfw3` -lvulkan -lstdc++fs

ALL_PLATFORM_SRC = $(shell find src/platform -name "*.cpp")

#this probably won't work for windows...
ifeq ($(shell uname), Linux)
PLATFORM_SRC = src/platform/linux_platform.cpp
endif

#main is a special snowflake, mostly for testing and the main function weirdness
TEMP_ALL_SRC = $(shell find src -name "*.cpp")
#probably a better way to do this, but meh
ALL_SRC = $(filter-out $(ALL_PLATFORM_SRC), $(TEMP_ALL_SRC)) $(PLATFORM_SRC)
SRC = $(filter-out src/main.cpp, $(ALL_SRC))
ALL_OBJ = $(patsubst src/%.cpp, build/%.o, $(ALL_SRC))
OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))

TEST_SRC = $(shell find test -name "*.cpp")
TEST_OBJ = $(patsubst test/%.cpp, build/test_%.o, $(TEST_SRC))
TEST_BIN = bin/main_test_package

ASSETS_DIR = assets

WAVEFRONT_SRC = $(wildcard models/wavefront/*.obj)
WAVEFRONT_ASSETS = $(patsubst models/wavefront/%.obj, assets/meshes/%.bin, $(WAVEFRONT_SRC))
WAVEFRONT_TOOL = tools/bin/wavefrontparser
WAVEFRONT_BLD = build/wavefrontparser

SHADER_TYPES = frag vert
SHADER_SRC = $(foreach type, $(SHADER_TYPES), $(wildcard shaders/*.$(type)))
SHADER_SPV = $(patsubst shaders/%, assets/shaders/%.spv, $(SHADER_SRC))

SPIKE_SRC = $(wildcard spike/*.cpp)
SPIKE_BIN = $(patsubst spike/%.cpp, bin/spike_%, $(SPIKE_SRC))

BUILD_DIRS = $(sort $(dir $(ALL_OBJ) $(WAVEFRONT_BIN) $(SHADER_SPV) $(WAVEFRONT_ASSETS) $(ASSETS_DIR)) bin)

EXE = bin/game

all: depend $(BUILD_DIRS) $(EXE) $(WAVEFRONT_ASSETS) shaders

depend: .depend 
	@echo > /dev/null

.depend: $(ALL_SRC) $(SPIKE_SRC) $(wildcard include/*.hpp)
	@rm -f ./.depend
	g++ $(CFLAGS) -MM $^ >> ./.depend;
	@sed -i -r 's/(.+)\.o/build\/\1.o/' .depend

include .depend

test: $(BUILD_DIRS) $(TEST_BIN)
	$(TEST_BIN)

$(TEST_BIN): $(OBJ) $(TEST_OBJ)
	g++ -g $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_OBJ): build/test_%.o: test/%.cpp
	g++ -g $(CFLAGS) -o $@ -c $^ $(LDFLAGS)

shaders: $(BUILD_DIRS) $(SHADER_SPV)	

$(SHADER_SPV): assets/shaders/%.spv: shaders/%
	$(VULKAN_SDK)/bin/glslangValidator -V -o $@ $^

$(EXE): $(ALL_OBJ)
	g++ -g $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(ALL_OBJ): build/%.o: src/%.cpp
	g++ -g $(CFLAGS) -o $@ -c $< $(LDFLAGS)

spike: $(BUILD_DIRS) $(OBJ) $(SPIKE_BIN)

bin/spike_%: spike/%.cpp
	g++ -g $(CFLAGS) -o $@ $^ $(OBJ) $(LDFLAGS)

$(BUILD_DIRS):
	@mkdir -p $@

$(WAVEFRONT_ASSETS): assets/meshes/%.bin: models/wavefront/%.obj $(WAVEFRONT_TOOL)
	$(WAVEFRONT_TOOL) $(filter-out $(WAVEFRONT_TOOL), $^) $@
	@touch $(WAVEFRONT_BLD)

wavefront: $(WAVEFRONT_ASSETS)

$(WAVEFRONT_TOOL): $(WAVEFRONT_BLD)
	$(MAKE) -C tools wavefrontparser

$(WAVEFRONT_BLD):

clean:
	@rm -rf $(filter-out ./, $(BUILD_DIRS))
	@rm -f .depend
	@$(MAKE) -C tools clean

clean_spike:
	@rm -f bin/spike_*

.PHONY: all wavefront shaders spike depend
