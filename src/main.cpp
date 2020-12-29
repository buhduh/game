#include <chrono>

#include "platform.hpp"
#include "artemis_game.hpp"
#include "artemis_memory.hpp"
#include "artemis_asset.hpp"
#include "renderer.hpp"
#include "artemis_input.hpp"
#include "artemis_camera.hpp"

static bool run = true;

//translations
static const uint8_t T_UP_BIT    =  1 << 0;
static const uint8_t T_LEFT_BIT  =  1 << 1;
static const uint8_t T_DOWN_BIT  =  1 << 2;
static const uint8_t T_RIGHT_BIT =  1 << 3;
static const uint8_t T_IN_BIT    =  1 << 4;
static const uint8_t T_OUT_BIT   =  1 << 5;
volatile static uint8_t translationState = 0;

//rotations
static const uint8_t R_CCW_BIT  = 1 << 0;
static const uint8_t R_CW_BIT   = 1 << 1;
static const uint8_t R_UP_BIT   = 1 << 2;
static const uint8_t R_DOWN_BIT = 1 << 3;
static const uint8_t R_IN_BIT   = 1 << 4;
static const uint8_t R_OUT_BIT =  1 << 5;
volatile static uint8_t rotationState = 0;


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
	if(translationState & T_LEFT_BIT && !(translationState & T_RIGHT_BIT)) {
		camera->panLeft(time);
	} else if(translationState & T_RIGHT_BIT && !(translationState & T_LEFT_BIT)) {
		camera->panRight(time);
	}
	if(translationState & T_UP_BIT && !(translationState & T_DOWN_BIT)) {
		camera->panUp(time);
	} else if(translationState & T_DOWN_BIT && !(translationState & T_UP_BIT)) {
		camera->panDown(time);
	}
	if(rotationState & R_IN_BIT && !(rotationState & R_OUT_BIT)) {
		camera->rotateIn(ANGULAR_VEL * time);
	} else if(rotationState & R_OUT_BIT && !(rotationState & R_IN_BIT)) {
		camera->rotateOut(ANGULAR_VEL * time);
	}
	if(translationState & T_IN_BIT && !(translationState & T_OUT_BIT)) {
		camera->panIn(time);
	} else if(translationState & T_OUT_BIT && !(translationState & T_IN_BIT)) {
		camera->panOut(time);
	}
}

void cbAction(input::Context* context, input::key_t key, input::event_t event) {
	switch(key) {
	case INPUT_KEY_W:
		if(event == input::KEY_PRESSED)
			translationState |= T_UP_BIT;	
		else
			translationState &= ~T_UP_BIT;	
		break;
	case INPUT_KEY_A:
		if(event == input::KEY_PRESSED)
			translationState |= T_LEFT_BIT;	
		else
			translationState &= ~T_LEFT_BIT;	
		break;
	case INPUT_KEY_S:
		if(event == input::KEY_PRESSED)
			translationState |= T_DOWN_BIT;	
		else
			translationState &= ~T_DOWN_BIT;	
		break;
	case INPUT_KEY_D:
		if(event == input::KEY_PRESSED)
			translationState |= T_RIGHT_BIT;
		else
			translationState &= ~T_RIGHT_BIT;
		break;
	case INPUT_KEY_Q:
		if(event == input::KEY_PRESSED)
			rotationState |= R_OUT_BIT;
		else
			rotationState &= ~R_OUT_BIT;
		break;
	case INPUT_KEY_E:
		if(event == input::KEY_PRESSED)
			rotationState |= R_IN_BIT;
		else
			rotationState &= ~R_IN_BIT;
		break;
	case INPUT_KEY_L_CTRL:
		if(event == input::KEY_PRESSED)
			translationState |= T_OUT_BIT;
		else
			translationState &= ~T_OUT_BIT;
		break;
	case INPUT_KEY_L_SHFT:
		if(event == input::KEY_PRESSED)
			translationState |= T_IN_BIT;
		else
			translationState &= ~T_IN_BIT;
		break;
	default:
		assert(false);
	}
}

void initializeInput(input::Context* context, Camera* camera) {
	//so lambdas can't create closures around local variables and behave as
	//function pointers.  Like what is the fucking point of them?
	static Camera* fuckingStupid = camera;
	//translations
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
		context, input::KEY_PRESSED, INPUT_KEY_R,
		[](input::Context*, input::key_t, input::event_t) {
			fuckingStupid->snapToOrigin();	
		}
	);
	assert(res);
	res = input::addCallbackToContext(
		context, input::KEY_PRESSED, INPUT_KEY_ESC,
		[](input::Context*, input::key_t, input::event_t) {
			run = false;
		}
	);
	assert(res);

	//rotations
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
	GameMemory* mainMemory = new GameMemory(GIGABYTES(1));
	StackArena* stackArena = 
		mainMemory->newStackArena(KILOBYTES(1));

	//most this crap shouldn't be on the stack
	platform::Window* window = platform::createWindow();
	platform::initializeInput(window);
	//MeshMemoryManager mManager(mainMemory);
	//Mesh* mesh = loadMeshFromFile(&mManager, std::string("sphere"));
	renderer::Vulkan vulkan = renderer::Vulkan(window);

	IArena* arena = new StupidArena();

	Camera* camera = Camera::newCamera(
		arena, 
		glm::vec3(2.0f, 2.0f, 2.0f), 
		glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3(0.0f, 0.0f, 1.0f)
	);

	/*
	//stbi_load() is breaking this, and i have no idea why...
	input::initializeInputSystem(arena, window);
	input::Context* context = input::requestNewContext();
	input::enableContext(context);
	initializeInput(context, camera);
	*/

	auto lastTime = std::chrono::high_resolution_clock::now();

	while(!platform::shouldCloseWindow(window) && run) {
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
