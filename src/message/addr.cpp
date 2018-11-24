#include "messages.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <vector>

namespace message{

	Addr::Addr(std::vector<networkable::Address> addressList) :
		Message(addr), addresses(addressList) {}
	Addr::Addr(NetworkBuffer* networkBuffer) :
		Message(addr) {
			auto size = networkBuffer->readVar_uint()\
				    .getValue();
			for(uint64_t i = 0;i < size; ++i){
				addresses.push_back(networkBuffer\
						->readAddress());
			}
		}
	std::string Addr::payload() const{
		std::string out = networkable::Var_uint(addresses\
				.size()).byteRepr();
		for(auto& adr : addresses){
			out += adr.byteRepr();
		}
		return out;
	}

	GetAddr::GetAddr() : Message(getaddr) {}

	std::string GetAddr::payload() const{
		return "";
	}

} // namespace message
