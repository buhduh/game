#ifndef ARTEMIS_INPUT_HPP
#define ARTEMIS_INPUT_HPP

#include "artemis_game.hpp"
#include "artemis_memory.hpp"
#include "platform.hpp"

//this won't work for more complex input like combos and such,
//or a state machine, but I can deal with that later
namespace input {

	//will need to tune this i think
	const uint8_t MAX_CALLBACKS = UINT8_MAX;
	const uint8_t DEFAULT_CB_COUNT = 32; 

	typedef uint8_t action_t;
	typedef void ACTION_CALLBACK(struct Context*, key_t, action_t);

	const action_t PRESSED = 0;
	const action_t RELEASED = 1;
	struct _action {
		ACTION_CALLBACK* callbacks[2][MAX_CALLBACKS];
	};

	//will need to tune this i think
	const uint8_t MAX_KEYS = UINT8_MAX;
	const uint8_t DEFAULT_KEY_COUNT = 16;
	struct _key {
		key_t value;
		_action* actions[1];
	};

/*
	Context:
	"key" -> action -> cb
	cb***
	this is all packed tightly in memory, cbs directly after actions directly after keys
	hopefully this will all fit on l1 cache.
*/
	//Contexts are statically sized for now
	struct Context {
		uint8_t numKeys;
		uint8_t numCallbacks;
		_key keys[DEFAULT_KEY_COUNT];
	};

	void initializeInputSystem(IArena*, platform::Window*);
	Context* requestContext();
	bool addCallbackToContext(Context*, action_t, key_t, ACTION_CALLBACK*);
	bool removeCallbackFromContext(Context*, ACTION_CALLBACK*);
	void enableContext(Context*);
	Context* getActiveContext();

	//if activeContext is nullptr do nothing
	void internalInputCallback(platform::input_key_t, platform::input_action_t);

	static Context* activeContext = nullptr;
	static bool initialized = false;
	static IArena* arena = nullptr;

};

#endif
