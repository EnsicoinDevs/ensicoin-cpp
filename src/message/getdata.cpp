#include "messages.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <algorithm>
#include <string>
#include <vector>

namespace message{
	networkable::Inv_vect::ressource_type GetData::dataType() 
		const{
			if(data.empty())
				return networkable::Inv_vect::\
					invalidRes;
			return data[0].type;
		}

	std::vector<std::string> GetData::dataAsked() const{
		std::vector<std::string> out;
		std::transform(data.begin(), data.end(), 
				std::back_inserter(out),
				[](networkable::Inv_vect i){
					return i.hash;
				});
		return out;
	}

	GetData::GetData(std::vector<networkable::Inv_vect> invData):
		Message(getdata), data(invData) {}

	GetData::GetData(NetworkBuffer* networkBuffer) : 
		Message(getdata),
		data(networkable::Inv_Array(networkBuffer).getValue()){}

	std::string GetData::payload() const{
		return networkable::Inv_Array(data).byteRepr();
	}
} // namespace message
