#include "messages.hpp"

#include "constants.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <ctime>
#include <string>

namespace message{

	WhoAmI::WhoAmI() : Message(whoami), 
			   version(VERSION),
			   timestamp(std::time(nullptr)) {}

	WhoAmI::WhoAmI(NetworkBuffer* networkBuffer) : 
		Message(whoami),
		version(networkBuffer->readUint32()),
		timestamp(networkBuffer->readUint64()) {}

	std::string WhoAmI::payload() const{
		return version.byteRepr() + 
			timestamp.byteRepr();
	}
	
	WhoAmIAck::WhoAmIAck() : Message(whoamiack) {}

	std::string WhoAmIAck::payload() const{
		return "";
	}

} //namespace message
