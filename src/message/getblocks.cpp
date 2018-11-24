#include "messages.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <string>
#include <vector>

namespace message{
	GetBlocks::GetBlocks(std::vector<std::string> hashList,
			std::string stopHashString) :
		Message(getblocks),
		blockHashes(hashList),
		stopHash(stopHashString) {}
	GetBlocks::GetBlocks(NetworkBuffer* networkBuffer) :
		Message(getblocks) {
		auto hashCount = networkBuffer->readVar_uint()\
					.getValue();
		for(uint64_t i = 0; i < hashCount; ++i){
			blockHashes.push_back(networkBuffer\
					->readHash());
		}
		stopHash = networkBuffer->readHash();
	}

	std::string GetBlocks::payload() const{
		std::string out = networkable::Var_uint(
				blockHashes.size()).byteRepr();
		for(auto& hs : blockHashes){
			out += hs;
		}
		out += stopHash;
		return out;
	}
} // namespace message
