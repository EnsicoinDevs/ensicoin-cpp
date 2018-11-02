#ifndef HASHMEMORY_HPP
#define HASHMEMORY_HPP

#include <map>
#include <string>

#include "crypto.hpp"

class HashMemory{
	private:
		std::map<std::string, Hashable::pointer > memory;
	public:
		bool add(Hashable::pointer element);
		bool exists(std::string elementHash);
		Hashable::pointer get(std::string elementHash);
};

#endif /* HASHMEMORY_HPP */
