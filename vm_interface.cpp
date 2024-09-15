#include <stdlib.h>
#include <cstdio>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <chrono>
#include "vm_interface.hpp"

using namespace std::chrono;

VM* create_vm(VM* vm){
	std::cout << "Created VM with " << sizeof(VM) << " bytes" << std::endl; 
	vm = (VM *)(malloc(sizeof(VM)));
	vm->stack_size = 0;
	return vm;
}

StackObject* create_object(ObjectType t) {
	std::cout << "Created StackObject with " << sizeof(StackObject) << " bytes" << std::endl; 
	StackObject* obj = (StackObject *)(malloc(sizeof(StackObject)));
	obj->type = t;
	system_clock::time_point tp = system_clock::now();
  	system_clock::duration dtn = tp.time_since_epoch();
	std::cout << std::endl;
	obj->head = new StackObject(
					ObjectType::INT_PAIR,
					new StackObject(ObjectType::INT, int(dtn.count() * system_clock::period::num / system_clock::period::den)),
					new StackObject(ObjectType::INT, int(dtn.count() * system_clock::period::num / system_clock::period::den))
				);
	
	return obj;
}

void push(StackObject* obj, VM* vm) {
	if (vm == 0){ 
		vm = create_vm(vm);
	}
	vm->stack[vm->stack_size] = obj;
	vm->stack_size  = std::max(vm->stack_size + 1, STACK_SIZE);
	mark(obj);
}

StackObject* pop(VM* vm) {
	vm->stack_size -= 1;
	return vm->stack[vm->stack_size];
}

//mark-sweep allocator code

void mark(StackObject* obj) {
	if (obj == nullptr) {
		return;
	}
	if (obj->marked){
		return;
	}
	obj->marked = 1;
	if (obj->type == INT_PAIR){
		std::cout << "Running mark on StackObject containing " << obj->head << " , " << obj->head << std::endl;
		mark(obj->head);
		mark(obj->tail);
	}
}

void sweep(StackObject* seed) {
	StackObject* nxt_ptr = seed->next;
	while (nxt_ptr){
		if (seed->marked == 0){
			free(seed);
		}
		seed = seed->next;
	}
}


void spawn_objects(VM* vm, int num_objects){
    for (int i = 0; i < num_objects; i++){
        int value = rand() % 2;
        switch(value){
            case 0:
				std::cout << "Spawning INT" << std::endl;
                push(create_object(ObjectType::INT), vm);
				break;      
		    case 1:
				std::cout << "Spawning INT_PAIR" << std::endl;
                push(create_object(ObjectType::INT_PAIR), vm);
				break;
        }
    }
}

void run_unithreaded_gc(VM* vm){
    mark(vm->stack[0]);
    sweep(vm->stack[0]);
}