#include "artemis_input.hpp"

namespace input {
	static uint8_t numCallbacks = 0;
	static Context* activeContext = nullptr;
	static IArena* arena = nullptr;

	size_t getContextSize(Context context) {
		return sizeof(_key) * context.numKeys;
	}

	void initializeInputSystem(IArena* _arena, platform::Window* window) {
		assert(_arena);
		if(arena) return;
		arena = _arena;
	}

	//This is stupid easier....
	Context* requestNewContext() {
		assert(arena);
		if(!arena) return nullptr;
		Context* context = (Context*) arena->allocate(sizeof(Context));
		context->numCallbacks = DEFAULT_CB_COUNT;
		context->numKeys = DEFAULT_KEY_COUNT;
		for(int i = 0; i < context->numKeys; i++) {
			context->keys[i].value = platform::KEY_UNASSIGNED;
		}
		return context;
	}

	//the problem here is that comparing pointers will remove ALL actions at that address
	//in this system.  Only want to remove events at a particular
	bool removeCallbackFromContext(
		Context* context, event_t event, key_t key, ACTION_CALLBACK* action
	) 
	{
		assert(context);
		if(!context) return false;
		if(!action) return false;
		_key* foundKey = nullptr;
		for(int i = 0; i < context->numKeys; i++) {
			if(context->keys[i].value == key) {
				foundKey = &context->keys[i];
				break;
			}
		}
		assert(foundKey);
		if(!foundKey) return false;
		bool removed = false;
		for(int i = 0; i < context->numCallbacks; i++) {
			if(foundKey->actions[event][i] == action) {
				foundKey->actions[event][i] = nullptr;
				removed = true;
			}
		}
		return removed;
	}

	//makes no attempt to keep callbacks unique
	bool addCallbackToContext(
		Context* context, event_t event, 
		key_t key, ACTION_CALLBACK* action
	) 
	{
		assert(context);
		if(!context) return false;
		if(!action) return false;
		_key* foundKey = nullptr;
		bool canAdd = false;
		int keyIndex;
		for(int i = 0; i < context->numKeys; i++) {
			if(context->keys[i].value == key) {
				foundKey = &context->keys[i];
				break;
			//do not believe c++ has lazy evaluation
			} else if(!canAdd && context->keys[i].value == platform::KEY_UNASSIGNED) {
				canAdd = true;	
				keyIndex = i;
			}
		}
		if(foundKey) {
			for(int i = 0; i < context->numCallbacks; i++) {
				if(!foundKey->actions[event][i]) {
					foundKey->actions[event][i] = action;
					return true;
				}
			}
			//most likely due to running out of space for actions
			assert(false);
			return false;
		}
		//no more space left for keys...
		assert(canAdd);
		if(!canAdd) return false;
		context->keys[keyIndex].value = key;
		//should be safe to just put it at 0...., i think....
		context->keys[keyIndex].actions[event][0] = action;
		return true;
	}

	void enableContext(Context* context) {
		assert(context);
		activeContext = context;
	}

	void internalInputCallback(platform::input_key_t key, platform::input_event_t event) {
		if(!activeContext) return;
		//this might not work for other platforms, may need to move this to the platform layer
		//glfw calls a "repeat" event whose value is 2.
		if(event >= NUM_EVENTS) return;
		for(int i = 0; i < activeContext->numKeys; i++) {
			if(key == activeContext->keys[i].value) {
				for(int j = 0; j < activeContext->numCallbacks; j++) {
					if(activeContext->keys[i].actions[event][j]) {
						activeContext->keys[i].actions[event][j](
							activeContext, key, event
						);
					}
				}
				break;
			}
		}
	}
};
