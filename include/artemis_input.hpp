#ifndef ARTEMIS_INPUT_HPP
#define ARTEMIS_INPUT_HPP

#include "artemis_game.hpp"
#include "artemis_memory.hpp"
#include "platform.hpp"

//this won't work for more complex input like combos and such,
//or a state machine, but I can deal with that later
namespace input {

	//NOTE statically defining the various lists for now
	//asserts will blow up when I break it, at which point
	//I either dynamically adjust at run time or make them bigger

	//will need to tune this i think
	const uint8_t MAX_CALLBACKS = UINT8_MAX;
	const uint8_t DEFAULT_CB_COUNT = 32; 

	typedef uint8_t event_t;
	typedef platform::input_key_t key_t;
	typedef void ACTION_CALLBACK(struct Context*, key_t, event_t);

	//may need to map these in the platform layer and write some kind of converter
	//to the index values
	const event_t NUM_EVENTS = 2;
	const event_t KEY_PRESSED = 1;
	const event_t KEY_RELEASED = 0;

	//will need to tune this i think
	const uint8_t MAX_KEYS = UINT8_MAX;
	const uint8_t DEFAULT_KEY_COUNT = 16;
	struct _key {
		key_t value;
		ACTION_CALLBACK* actions[NUM_EVENTS][DEFAULT_CB_COUNT];
	};

/*
	Context:
	"key" -> action -> cb
	cb***
	this is all packed tightly in memory, cbs directly after actions directly after keys
	hopefully this will all fit on l1 cache.
	keys are unique to context
	all contexts share same callback list
*/
	struct Context {
		uint8_t numKeys;
		//technically numCallbacks should go in the _key struct
		//but changing the callback size for individual keys
		//sounds really messy
		uint8_t numCallbacks;
		_key keys[DEFAULT_KEY_COUNT];
	};

	void initializeInputSystem(IArena*, platform::Window*);
	Context* requestNewContext();
	bool addCallbackToContext(Context*, event_t, key_t, ACTION_CALLBACK*);
	bool removeCallbackFromContext(Context*, event_t, key_t, ACTION_CALLBACK*);
	void enableContext(Context*);
	Context* getActiveContext();

	//if activeContext is nullptr do nothing
	void internalInputCallback(platform::input_key_t, platform::input_event_t);
};

#endif
