#include "messages.hpp"
#include "blocks.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <memory>
#include <string>
#include <vector>

namespace message{
	BlockMessage::BlockMessage(ressources::Block::pointer blockPtr) :
		Message(block),
		blockRef(blockPtr) {}
	BlockMessage::BlockMessage(NetworkBuffer* networkBuffer) :
		Message(block),
		blockRef(std::make_shared<ressources::Block>(networkBuffer)) {}
	ressources::Block::pointer BlockMessage::getBlock() const{
		return blockRef;
	}

	std::string BlockMessage::payload() const{
		return blockRef->byteRepr();
	}
} // namespace message
