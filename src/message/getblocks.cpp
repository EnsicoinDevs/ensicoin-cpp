#include "messages.hpp"
#include "constants.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <algorithm>
#include <string>
#include <vector>

namespace message{
	GetBlocks::GetBlocks(std::vector<std::string> hashList,
			std::string stopHashString) :
		Message(getblocks),
		blockHashes(hashList),
		stopHash(stopHashString) {}
	
	GetBlocks::GetBlocks(NetworkBuffer* networkBuffer) :
		Message(getblocks),
		blockHashes(networkable::HashArray(networkBuffer).getValue()),
		stopHash(networkable::Hash(networkBuffer).getValue()) {}

	std::string GetBlocks::payload() const{
		std::string out = networkable::HashArray(blockHashes).byteRepr();
		out += stopHash;
		return out;
	}
} // namespace message
