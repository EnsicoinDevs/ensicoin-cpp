#include "messages.hpp"

#include "constants.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <ctime>
#include <string>

namespace message{

	WhoAmI::WhoAmI() : Message(whoami), 
			   version(constants::VERSION),
			   timestamp(std::time(nullptr)) {}

	WhoAmI::WhoAmI(NetworkBuffer* networkBuffer) : 
		Message(whoami),
		version(networkable::Uint32(networkBuffer).getValue()),
		timestamp(networkable::Uint64(networkBuffer).getValue()) {}

	std::string WhoAmI::payload() const{
		return 	networkable::Uint32(version).byteRepr() + 
				networkable::Uint64(timestamp).byteRepr();
	}
	
	WhoAmIAck::WhoAmIAck() : Message(whoamiack) {}

	std::string WhoAmIAck::payload() const{
		return "";
	}

} //namespace message
