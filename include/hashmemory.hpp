#ifndef HAHSMEMORY_HPP
#define HASHMEMORY_HPP

#include <map>
#include <string>

#include "crypto.hpp"

class HashMemory{
	private:
		std::map<std::string, Hashable* > memory;
	public:
		bool add(Hashable* element);
		bool exists(std::string elementHash);
		Hashable* get(std::string elementHash);
};

#endif /* HASHMEMORY_HPP */
