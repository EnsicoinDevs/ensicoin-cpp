#ifndef HASHMEMORY_HPP
#define HASHMEMORY_HPP

#include <unordered_map>
#include <memory> 
#include <string>

template<class T>
class HashMemory{
	public:
		using pointer = std::shared_ptr<T>;
		bool exists(std::string elemHash) const {
			return memory.count(elemHash) == 1;
		}
		bool exists(pointer elem) const{
			return exists(elem->hash());
		}
		bool add(pointer element){
			if(exists(element)){
				return false;
			}
			memory[element->hash()] = element;
			return true;
		}
		pointer get(std::string elemHash) const {
			return memory.at(elemHash);
		}
		void erase(std::string elemHash){
			memory.erase(elemHash);
		}
	private:
		std::unordered_map<std::string, pointer > memory;
};

#endif /* HASHMEMORY_HPP */
