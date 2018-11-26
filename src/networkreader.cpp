#include "networkreader.hpp"


#include "connection.hpp"
#include "messages.hpp"
#include "networkbuffer.hpp"
#include "node.hpp"

#include <iostream>
#include <memory>

namespace network{

	NetworkReader::NetworkReader(Node* nod, 
				Connection::pointer conn,
				NetworkBuffer* networkBuffer) :
		node(nod), connection(conn), buffer(networkBuffer) {}

	void NetworkReader::handle(){
		
	}

	uint64_t NetworkReader::readRequest() const{
		return toBeRead;
	}

	uint64_t NetworkReader::readCount() const{
		return readSize;
	}
	
} // namespace network
