#include "transaction.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <rapidjson/document.h>
#include <sstream>

namespace ressources{

	InputTransaction InputTransaction::getValue() const{
		return *this;
	}

	InputTransaction::InputTransaction() {}

	rapidjson::Value InputTransaction::json(rapidjson::Document*
			document) const {
		rapidjson::Value input(rapidjson::kObjectType);
		input.AddMember("previousOutput", 
				previousOutput.json(document),
				document->GetAllocator());

		input.AddMember("script",
				"flemme tu regarde les bytes" , 
				document->GetAllocator());

		return input;
	}
	
	InputTransaction::InputTransaction(NetworkBuffer* 
			networkBuffer) : 
		previousOutput(networkBuffer),
		script(networkBuffer) {}

	InputTransaction::InputTransaction(TransactionIdentifier id,
			Script sc) : 
		previousOutput(id),
		script(sc) {}

	std::string InputTransaction::byteRepr() const{
		return previousOutput.byteRepr() +
			script.byteRepr();
	}

} // namespace ressources
