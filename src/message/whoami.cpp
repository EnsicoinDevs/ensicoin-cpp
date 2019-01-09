#include "messages.hpp"

#include "constants.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <ctime>
#include <string>

namespace message{

	WhoAmI::WhoAmI(networkable::Address nodeAddress) : 
			   Message(whoami),
			   version(constants::VERSION),
			   address(nodeAddress),
			   services({"node"}){}

	WhoAmI::WhoAmI(NetworkBuffer* networkBuffer) : 
		Message(whoami),
		version(networkable::Uint32(networkBuffer).getValue()),
		address(networkable::Address(networkBuffer)),
		services(networkable::Var_strArray(networkBuffer).getValue()) {}

	std::string WhoAmI::payload() const{
		return 	networkable::Uint32(version).byteRepr() + 
				address.byteRepr() +
				networkable::Var_strArray(services).byteRepr();
	}
	
	WhoAmIAck::WhoAmIAck() : Message(whoamiack) {}

	std::string WhoAmIAck::payload() const{
		return "";
	}

} //namespace message
