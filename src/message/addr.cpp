#include "messages.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <vector>

namespace message{

	Addr::Addr(std::vector<networkable::Address> addressList) :
		Message(addr), addresses(addressList) {}
	Addr::Addr(NetworkBuffer* networkBuffer) :
		Message(addr), 
		addresses(networkable::Var_Array<networkable::Address>(networkBuffer).getValue()) {}
	std::string Addr::payload() const{
		return networkable::Var_Array<networkable::Address>(addresses).byteRepr();
	}

	GetAddr::GetAddr() : Message(getaddr) {}

	std::string GetAddr::payload() const{
		return "";
	}

} // namespace message
