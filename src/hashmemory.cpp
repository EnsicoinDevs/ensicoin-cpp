#include <map>
#include <string>

#include "hashmemory.hpp"

bool HashMemory::exists(std::string elementHash){
	return memory.count(elementHash) == 1;
}

bool HashMemory::add(Hashable::pointer element){
	if ( exists(element->hash()) )
		return false;
	memory[element->hash()] = element;
	return true;
}

Hashable::pointer HashMemory::get(std::string elementHash){
	return memory.at(elementHash);
}
