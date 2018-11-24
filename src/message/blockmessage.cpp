#include "messages.hpp"
#include "blocks.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <memory>
#include <string>
#include <vector>

namespace message{
	BlockMessage::BlockMessage(std::shared_ptr<Block> blockPtr) :
		Message(block),
		blockRef(blockPtr) {}
	BlockMessage::BlockMessage(NetworkBuffer* networkBuffer) :
		Message(block),
		blockRef(std::make_shared<Block>(networkBuffer)) {}
	std::shared_ptr<Block> BlockMessage::getBlock() const{
		return blockRef;
	}

	std::string BlockMessage::payload() const{
		return blockRef->byteRepr();
	}
} // namespace message
