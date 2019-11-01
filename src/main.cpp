#include <chrono>

#include "platform.hpp"
#include "artemis_game.hpp"
#include "artemis_memory.hpp"
#include "artemis_asset.hpp"
#include "renderer.hpp"
#include "artemis_input.hpp"
#include "artemis_camera.hpp"

static const uint8_t upBit    =  1 << 0;
static const uint8_t leftBit  =  1 << 1;
static const uint8_t downBit  =  1 << 2;
static const uint8_t rightBit =  1 << 3;
static const uint8_t ccwBit =    1 << 4;
static const uint8_t cwBit  =    1 << 5;
static const uint8_t inBit  =    1 << 6;
static const uint8_t outBit  =   1 << 7;
volatile static uint8_t keyState = 0;

//radians/sec
static const float ANGULAR_VEL = PI_OVER_2;

renderer::UniformBufferObject* constructUBO(IArena* arena, Camera* camera) {
	renderer::UniformBufferObject ubo = {};
	ubo.model = glm::mat4(1.0f);
	ubo.view = camera->view;
	renderer::UniformBufferObject* toRet = 
		(renderer::UniformBufferObject*) arena->allocate(sizeof(ubo));
	*toRet = ubo;
	return toRet;
}

void updateCamera(Camera* camera, float time) {
	if(keyState & leftBit && !(keyState & rightBit)) {
		camera->panLeft(time);
	} else if(keyState & rightBit && !(keyState & leftBit)) {
		camera->panRight(time);
	}
	if(keyState & upBit && !(keyState & downBit)) {
		camera->panUp(time);
	} else if(keyState & downBit && !(keyState & upBit)) {
		camera->panDown(time);
	}
	if(keyState & ccwBit && !(keyState & cwBit)) {
		camera->rotateCCW(ANGULAR_VEL * time);
	} else if(keyState & cwBit && !(keyState & ccwBit)) {
		camera->rotateCW(ANGULAR_VEL * time);
	}
	if(keyState & inBit && !(keyState & outBit)) {
		camera->panIn(time);
	} else if(keyState & outBit && !(keyState & inBit)) {
		camera->panOut(time);
	}
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
	case INPUT_KEY_Q:
		if(event == input::KEY_PRESSED)
			keyState |= ccwBit;
		else
			keyState &= ~ccwBit;
		break;
	case INPUT_KEY_E:
		if(event == input::KEY_PRESSED)
			keyState |= cwBit;
		else
			keyState &= ~cwBit;
		break;
	case INPUT_KEY_L_CTRL:
		if(event == input::KEY_PRESSED)
			keyState |= outBit;
		else
			keyState &= ~outBit;
		break;
	case INPUT_KEY_L_SHFT:
		if(event == input::KEY_PRESSED)
			keyState |= inBit;
		else
			keyState &= ~inBit;
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
		context, input::KEY_PRESSED, INPUT_KEY_Q, cbAction
	);
	assert(res);
	res = input::addCallbackToContext(
		context, input::KEY_PRESSED, INPUT_KEY_E, cbAction
	);
	assert(res);
	res = input::addCallbackToContext(
		context, input::KEY_PRESSED, INPUT_KEY_L_CTRL, cbAction
	);
	assert(res);
	res = input::addCallbackToContext(
		context, input::KEY_PRESSED, INPUT_KEY_L_SHFT, cbAction
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
	res = input::addCallbackToContext(
		context, input::KEY_RELEASED, INPUT_KEY_Q, cbAction
	);
	assert(res);
	res = input::addCallbackToContext(
		context, input::KEY_RELEASED, INPUT_KEY_E, cbAction
	);
	assert(res);
	res = input::addCallbackToContext(
		context, input::KEY_RELEASED, INPUT_KEY_L_CTRL, cbAction
	);
	assert(res);
	res = input::addCallbackToContext(
		context, input::KEY_RELEASED, INPUT_KEY_L_SHFT, cbAction
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

	Camera* camera = Camera::newCamera(
		arena, 
		glm::vec3(2.0f, 2.0f, 2.0f), 
		glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3(0.0f, 0.0f, 1.0f)
	);

	input::initializeInputSystem(arena, window);
	input::Context* context = input::requestNewContext();
	input::enableContext(context);
	initializeInput(context);

	auto lastTime = std::chrono::high_resolution_clock::now();

	while(!platform::shouldCloseWindow(window)) {
		stackArena->reset();	
		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = 
			std::chrono::duration<float, std::chrono::seconds::period>(
				lastTime - currentTime
			).count();
		lastTime = currentTime;
		platform::pollEvents();
		updateCamera(camera, time);
		auto ubo = constructUBO(stackArena, camera);	
		vulkan.drawFrame(ubo);	
	}
	platform::destroyWindow(window);
}
