#include "messages.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace message{
	Inv::Inv(std::vector<networkable::Inv_vect> invData):
		Message(inv), data(invData) {}
	
	Inv::Inv(NetworkBuffer* networkBuffer) : 
		Message(inv),
		data(networkable::Inv_Array(networkBuffer).getValue()) {}

	std::string Inv::payload() const{
		return networkable::Inv_Array(data).byteRepr();
	}
} // namespace message

