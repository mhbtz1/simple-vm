#include "vm_interface.hpp"
#include <cstdlib>
#include <cstdio>
#include <cstdint>


uint8_t execute_instr(VM* vm, char* instruction) {
	// execute instruction using stack
}

int main(int argv, char** argc){
	VM* vm;
    spawn_objects(create_vm(vm), 10);
}
