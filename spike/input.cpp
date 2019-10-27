#include "artemis_input.hpp"

int main() {
	for(int i = 0; i < MAX_CONTEXT_HANDLES; i++) {
		if(!input::handles[i]) STD_LOG("false");
		else STD_LOG("true");
	}
}
