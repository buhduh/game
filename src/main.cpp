#include "platform.hpp"
#include "artemis_game.hpp"
#include "artemis_memory.hpp"
#include "artemis_asset.hpp"
#include "renderer.hpp"
#include "artemis_input.hpp"

static const uint8_t upBit    = 1 << 0;
static const uint8_t leftBit  = 1 << 1;
static const uint8_t downBit  = 1 << 2;
static const uint8_t rightBit = 1 << 3;
volatile static uint8_t keyState = 0;

renderer::UniformBufferObject* constructUBO(IArena* arena) {
	renderer::UniformBufferObject ubo = {};
	ubo.model = glm::mat4(1.0f);
	ubo.view = glm::lookAt(
		glm::vec3(2.0f, 2.0f, 2.0f), 
		glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
	renderer::UniformBufferObject* toRet = 
		(renderer::UniformBufferObject*) arena->allocate(sizeof(ubo));
	*toRet = ubo;
	return toRet;
}

void cbAction(input::Context* context, input::key_t key, input::event_t event) {
	switch(key) {
	case INPUT_KEY_W:
		if(event == input::KEY_PRESSED)
			keyState |= upBit;	
		else
			keyState &= ~upBit;	
		break;
	case INPUT_KEY_A:
		if(event == input::KEY_PRESSED)
			keyState |= leftBit;	
		else
			keyState &= ~leftBit;	
		break;
	case INPUT_KEY_S:
		if(event == input::KEY_PRESSED)
			keyState |= downBit;	
		else
			keyState &= ~downBit;	
		break;
	case INPUT_KEY_D:
		if(event == input::KEY_PRESSED)
			keyState |= rightBit;
		else
			keyState &= ~rightBit;
		break;
	default:
		assert(false);
	}
}

void initializeInput(input::Context* context) {
	auto res = input::addCallbackToContext(
		context, input::KEY_PRESSED, INPUT_KEY_W, cbAction
	);
	assert(res);
	res = input::addCallbackToContext(
		context, input::KEY_PRESSED, INPUT_KEY_A, cbAction
	);
	assert(res);
	res = input::addCallbackToContext(
		context, input::KEY_PRESSED, INPUT_KEY_S, cbAction
	);
	assert(res);
	res = input::addCallbackToContext(
		context, input::KEY_PRESSED, INPUT_KEY_D, cbAction
	);
	assert(res);
	res = input::addCallbackToContext(
		context, input::KEY_RELEASED, INPUT_KEY_W, cbAction
	);
	assert(res);
	res = input::addCallbackToContext(
		context, input::KEY_RELEASED, INPUT_KEY_A, cbAction
	);
	assert(res);
	res = input::addCallbackToContext(
		context, input::KEY_RELEASED, INPUT_KEY_S, cbAction
	);
	assert(res);
	res = input::addCallbackToContext(
		context, input::KEY_RELEASED, INPUT_KEY_D, cbAction
	);
	assert(res);
}

int main(void) {
	GameMemory* mainMemory = new GameMemory(gigabytes(1));
	StackArena* stackArena = 
		mainMemory->newStackArena(kilobytes(1));

	//most this crap shouldn't be on the stack
	platform::Window* window = platform::createWindow();
	platform::initializeInput(window);
	IArena* arena = new StupidArena();
	MeshOrganizer mOrganizer(arena);
	Mesh mesh1 = getMeshAsset("Cube", &mOrganizer);
	renderer::Vulkan vulkan = renderer::Vulkan(window, 1, &mesh1);

	input::initializeInputSystem(arena, window);
	input::Context* context = input::requestNewContext();
	input::enableContext(context);
	initializeInput(context);

	while(!platform::shouldCloseWindow(window)) {
		stackArena->reset();	
		auto ubo = constructUBO(stackArena);	
		platform::pollEvents();
		vulkan.drawFrame(ubo);	
	}
	platform::destroyWindow(window);
}
