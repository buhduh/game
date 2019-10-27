#include "artemis_input.hpp"

namespace input {
	
	void initializeInputSystem(IArena* _arena, platform::Window* window) {
		assert(!initialized);
		if(initialized) return;
		arena = _arena;	
		initialized = true;
	}

	Context makeEmptyContext() {
	}

	Context* requestContext() {
		assert(initialized);
		assert(arena);

		_key keys[DEFAULT_KEY_COUNT];
		Context context = Context{
			numKeys: DEFAULT_KEY_COUNT,
			numCallbacks: DEFAULT_CB_COUNT,
			keys: keys
		};

		size_t cSize(sizeof(context));

		_action action = {};
		size_t aSize(sizeof(action) * DEFAULT_KEY_COUNT);

		ACTION_CALLBACK* callbacks[DEFAULT_CB_COUNT];
		size_t cbSize(sizeof(callbacks));

		Context* toRet = (Context*) arena->allocate(cSize + aSize + cbSize);
		assert(toRet);
		return toRet;
		/*
		*toRet = context;

		uintptr_t actionIPtr = reinterpret_cast<uintptr_t>(toRet) + cSize;
		_action* actionPtr = reinterpret_cast<_action*>(actionIPtr);
		*actionPtr = action;

		uintptr_t cbIPtr = reinterpret_cast<uintptr_t>(actionPtr) + aSize;
		ACTION_CALLBACK** cbPtr = reinterpret_cast<ACTION_CALLBACK**>(cbIPtr);
		//cbPtr
		return nullptr;
		*/
	}

	//TODO resize context based on key size
	size_t getSizeOfContext(Context* c) {
		size_t nKS = sizeof(c->numKeys);
		size_t nCb = sizeof(c->numCallbacks);
		size_t sK = sizeof(c->keys);
		return nKS + nCb + sK;
	}

	ACTION_CALLBACK* getCallbackLocation(Context* context) {
		size_t sizeOfC = getSizeOfContext(context);
		uintptr_t where = reinterpret_cast<uintptr_t>(context) + sizeOfC;

	}

	//not going to resize this until i have to
	bool addCallbackToContext(
		Context* context, action_t action, 
		key_t key, ACTION_CALLBACK* callback)
	{
		assert(context->numCallbacks + 1 < MAX_CALLBACKS);
		_key* foundKey = nullptr;
		for(uint8_t i = 0; i < context->numKeys; ++i) {
			if(context->keys[i].value == key) {
				context->keys[i] = foundKey;
				break;
			}
		}
		if(!foundKey) {
			//need to add a key
		}
		//foundKey->actions[action][context->numCallbacks] = callback;
		return true;
	}


	//not intended to be called from game code
	//key is unique, not just the keyboard int, eg scan code from glfw
	void internalInputCallback(
		platform::input_key_t key, 
		platform::input_action_t action
	) {
		/*
		switch(key) {
		case A:
			STD_LOG("wtf?");
			break;
		};
		switch(action) {
		case PRESSED:
			STD_LOG("pressed");
			break;
		case RELEASED:
			STD_LOG("released");
			break;
		};
		*/
	}

	//TODO
	bool removeCallbackFromContext(
		Context* handle, 
		ACTION_CALLBACK* callback) 
	{
		return false;
	}

	//TODO
	void enableContext(Context* context) {
		assert(initialized);
		activeContext = context;
	}

	//TODO
	Context* getActiveContext() {
		return activeContext;
	}

};
