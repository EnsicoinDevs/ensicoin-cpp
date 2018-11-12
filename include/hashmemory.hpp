#ifndef HASHMEMORY_HPP
#define HASHMEMORY_HPP

#include <unordered_map>
#include <memory> 
#include <string>


/// \brief Wrapper around a std::unordered_map
/// \tparam T implement .hash()
template<class T>
class HashMemory{
	public:
		using pointer = std::shared_ptr<T>;
		/// \brief Check existence of hash
		/// \param elemHash hash of element
		bool exists(std::string elemHash) const {
			return memory.count(elemHash) == 1;
		}
		/// \brief Check existence of object
		/// \param elem object to check
		bool exists(pointer elem) const{
			return exists(elem->hash());
		}
		/// \brief Add element to memory
		/// \param element element to add
		/// \return returns false if element exists
		bool add(pointer element){
			if(exists(element)){
				return false;
			}
			memory[element->hash()] = element;
			return true;
		}
		/// \brief Get Element (without checking)
		/// \param elemHash hash of element
		pointer get(std::string elemHash) const {
			return memory.at(elemHash);
		}
		/// \brief Remove element (without checking)
		/// \param elemHash hash of element
		void erase(std::string elemHash){
			memory.erase(elemHash);
		}
	private:
		std::unordered_map<std::string, pointer > memory;
};

#endif /* HASHMEMORY_HPP */
