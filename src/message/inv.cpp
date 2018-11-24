#include "messages.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace message{
	networkable::Inv_vect::ressource_type Inv::\
		getRessourceType() const{
			if(data.empty())
				return networkable::Inv_vect::\
					invalidRes;
			return data[0].type;
		}
	
	std::vector<std::string> Inv::getRessourceHashes() const{
		std::vector<std::string> out;
		std::transform(data.begin(), data.end(), 
				std::back_inserter(out),
				[](networkable::Inv_vect i){
				return i.hash;
				});
		return out;
	}

	Inv::Inv(std::vector<networkable::Inv_vect> invData):
		Message(inv), data(invData) {}
	
	Inv::Inv(NetworkBuffer* networkBuffer) : 
		Message(inv) {
			auto size = networkBuffer->readVar_uint()\
				    	.getValue();
			for(uint64_t i=0; i < size; ++i){
				data.push_back(networkBuffer->\
						readInv_vect());
			}
		}

	std::string Inv::payload() const{
		std::string bytes = networkable::Var_uint(
					data.size()).byteRepr();
		for(auto& iv : data){
			bytes += iv.byteRepr();
		}
		return bytes;
	}
} // namespace message

