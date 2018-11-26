#include "transaction.hpp"
#include "networkbuffer.hpp"
#include "networkable.hpp"

#include <rapidjson/document.h>
#include <sstream>

namespace ressources{

	using rapidjson::Document;
	using rapidjson::Value;

	Value TransactionIdentifier::json(Document* document) const{
		Value tId(rapidjson::kObjectType);
		Value strVal;
		strVal.SetString(transactionHash.c_str(),
				transactionHash.length(),
				document->GetAllocator());

		tId.AddMember("transactionHash",
				strVal, 
				document->GetAllocator());
		tId.AddMember("index",
				index, document->GetAllocator());

		return tId;
	}
	
	TransactionIdentifier::TransactionIdentifier(NetworkBuffer*
			networkBuffer) :
		transactionHash(networkBuffer->readHash()),
		index(networkBuffer->readVar_uint().getValue()) {}

	std::string TransactionIdentifier::byteRepr() const{
		return networkable::Str(transactionHash).byteRepr() +
			networkable::Var_uint(index).byteRepr();
	}

} // namespace ressources
