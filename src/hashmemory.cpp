#include <map>
#include <string>

#include "hashmemory.hpp"

template <class H>
bool HashMemory<H>::exists(std::string elementHash){
	return memory.count(elementHash) == 1;
}

template <class H>
bool HashMemory<H>::add(H element){
	if ( hasBlock(element.hash()) )
		return false;
	memory[element.hash()] = element;
	return true;
}

template <class H>
H HashMemory<H>::get(std::string elementHash){
	return memory.at(elementHash);
}
