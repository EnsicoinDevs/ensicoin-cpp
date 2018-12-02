#include "messages.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <string>

namespace message{
	NotFound::NotFound(std::vector<networkable::Inv_vect> invData):
		Message(notfound), data(invData) {}
	
	NotFound::NotFound(NetworkBuffer* networkBuffer) : 
		Message(notfound),
		data(networkable::Inv_Array(networkBuffer).getValue()) {}

	std::string NotFound::payload() const{
		return networkable::Inv_Array(data).byteRepr();
	}
} // namespace message
