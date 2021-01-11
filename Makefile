#TODO Really need to sort these flags out

DIR = $(shell pwd)
#NOTE, don't forget to add PKG_CONFIG_PATH PKG_CONFIG_PATH := ${VULKAN_SDK}/lib/pkgconfig to env
#LDLIBS = $(shell pkg-config --static --libs glfw3) $(shell pkg-config --libs vulkan) $(shell pkg-config --libs vulkan)
LDLIBS = $(shell pkg-config --static --libs glfw3 vulkan)
export CFLAGS := -std=c++17 -I$(VULKAN_SDK)/include -I$(DIR)/include -I$(HOME)/include -I$(DIR)/third_party/ImGui -DDEBUG
export LDFLAGS := -Llib -lImGui $(LDLIBS) -lstdc++fs -lpthread

ALL_PLATFORM_SRC = $(shell find src/platform -name "*.cpp")

#this probably won't work for windows...
ifeq ($(shell uname), Linux)
PLATFORM_SRC = src/platform/linux_platform.cpp
endif

EXE = bin/game

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

MESH_ASSETS_DIR = assets/meshes
SHADERS_DIR = assets/shaders

WAVEFRONT_SRC = $(wildcard models/wavefront/*.obj)
WAVEFRONT_ASSETS = $(patsubst models/wavefront/%.obj, %, $(WAVEFRONT_SRC))
WAVEFRONT_TOOL = tools/bin/wavefrontparser
WAVEFRONT_BLD = build/wavefrontparser

SHADER_TYPES = frag vert
SHADER_SRC = $(foreach type, $(SHADER_TYPES), $(wildcard shaders/*.$(type)))
SHADER_SPV = $(patsubst shaders/%, assets/shaders/%.spv, $(SHADER_SRC))

SPIKE_SRC = $(wildcard spike/*.cpp)
SPIKE_BIN = $(patsubst spike/%.cpp, bin/spike_%, $(SPIKE_SRC))

LIB = lib/libArtemis.a

THIRD_PARTY = $(wildcard third_party/*/*.cpp)
UNIQUE_THIRD_PARTY = $(sort $(dir $(THIRD_PARTY)))
THIRD_PARTY_LIBS = $(UNIQUE_THIRD_PARTY:third_party/%/=lib/lib%.a)
#LIB depends on third party, must be built later
LIBS = $(THIRD_PARTY_LIBS) $(LIB)

BUILD_DIRS = $(sort $(dir $(ALL_OBJ) $(WAVEFRONT_BIN) $(LIBS)) bin $(MESH_ASSETS_DIR) $(SHADERS_DIR))

.PHONY: all
all: depend $(BUILD_DIRS) $(LIBS) $(EXE) $(WAVEFRONT_ASSETS) shaders

.PHONY:third_party
third_party: $(THIRD_PARTY_LIBS)

$(THIRD_PARTY_LIBS): $(BUILD_DIRS)
	@$(MAKE) -C third_party
	@cp third_party/lib/* lib/

$(LIB): $(OBJ)
	@ar crf $@ $^

depend: .depend 
	@echo > /dev/null

#TODO this depend could really be cleaned up
.PHONY:depend
.depend: $(ALL_SRC) $(SPIKE_SRC) $(wildcard include/*.hpp)
	@rm -f ./.depend
	@g++ $(CFLAGS) -MM $^ >> ./.depend;
	@sed -i -r 's/(.+)\.o/build\/\1.o/' .depend
	@ctags -R *

include .depend

.PHONY: test
test: $(BUILD_DIRS) $(TEST_BIN)
	$(TEST_BIN)

$(TEST_BIN): $(OBJ) $(TEST_OBJ)
	@g++ -g $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_OBJ): build/test_%.o: test/%.cpp
	@g++ -g $(CFLAGS) -o $@ -c $^ $(LDFLAGS)

.PHONY: shaders
shaders: $(BUILD_DIRS) $(SHADER_SPV)	

$(SHADER_SPV): assets/shaders/%.spv: shaders/%
	$(VULKAN_SDK)/bin/glslangValidator -V -o $@ $^

$(EXE): $(ALL_OBJ)
	g++ -g $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(ALL_OBJ): build/%.o: src/%.cpp
	g++ -g $(CFLAGS) -o $@ -c $< $(LDFLAGS)

.PHONY: spike
spike: $(BUILD_DIRS) $(OBJ) $(SPIKE_BIN)

bin/spike_%: spike/%.cpp
	g++ -g $(CFLAGS) -o $@ $^ $(OBJ) $(LDFLAGS)

$(BUILD_DIRS):
	@mkdir -p $@

$(WAVEFRONT_ASSETS): %: models/wavefront/%.obj $(WAVEFRONT_TOOL)
	@$(WAVEFRONT_TOOL) $(filter-out $(WAVEFRONT_TOOL), $^) $@
	@touch $(WAVEFRONT_BLD)

wavefront: $(WAVEFRONT_ASSETS)

$(WAVEFRONT_TOOL): $(WAVEFRONT_BLD)
	$(MAKE) -C tools wavefrontparser

$(WAVEFRONT_BLD):

.PHONY: clean
clean:
	rm -rf $(filter-out ./, $(BUILD_DIRS))

.PHONY: clean_spike
clean_spike:
	@rm -f bin/spike_*

.PHONY: clean_third_party
clean_third_party:
	@rm -f $(THIRD_PARTY_LIBS)
	@$(MAKE) -C third_party clean

.PHONY: purge
purge: clean clean_third_party
	@$(MAKE) -C tools clean
	@rm -f .depend
