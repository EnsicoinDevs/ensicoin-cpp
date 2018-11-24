#include "messages.hpp"

namespace message{
	GetMempool::GetMempool() : Message(getmempool) {}
	std::string GetMempool::payload() const{
		return "";
	}
} // namespace message
