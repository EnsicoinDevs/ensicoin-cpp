#include "messages.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <string>

namespace message{
	NotFound::NotFound(std::vector<networkable::Inv_vect> invData):
		Message(notfound), data(invData) {}
	
	GetData::GetData(NetworkBuffer* networkBuffer) : 
		Message(notfound) {
			auto size = networkBuffer->readVar_uint()\
				    	.getValue();
			for(uint64_t i=0; i < size; ++i){
				data.push_back(networkBuffer->\
						readInv_vect());
			}
		}

	std::string GetData::payload() const{
		std::string bytes = networkable::Var_uint(
					data.size()).byteRepr();
		for(auto& iv : data){
			bytes += iv.byteRepr();
		}
		return bytes;
	}
} // namespace message
