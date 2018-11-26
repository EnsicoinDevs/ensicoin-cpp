#include "transaction.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <rapidjson/document.h>
#include <sstream>

namespace ressources{

	rapidjson::Value OutputTransaction::json(rapidjson::Document*
			document) const {
		rapidjson::Value output(rapidjson::kObjectType);
		output.AddMember("value", value, document->GetAllocator());

		output.AddMember("script", "nique ton json lit les bytes",
				document->GetAllocator());

		return output;
	}
	
	std::string OutputTransaction::byteRepr() const{
		return networkable::Uint64(value).byteRepr() +
				script.byteRepr();
	}

	OutputTransaction::OutputTransaction(NetworkBuffer* 
			networkBuffer) : 
		value(networkBuffer->readUint64().getValue()),
		script(networkBuffer) {}

} //namespace ressources
