#include "target.hpp"
#include "blockchain.hpp"
#include "blocks.hpp"
#include "constants.hpp"

namespace ressources{
	void BlockTarget::update(manager::BlockIndex* bIndex, int timeTaken,const std::string& currentHash){
		auto blockWindow = bIndex->getWindowFrom(currentHash);
	}

	bool BlockTarget::blockInferior(const Block& target) const {
		return target.integerHash() < value;
	}

	uint32_t BlockTarget::getValue() const{
		return 0;
	}

	std::string BlockTarget::byteRepr() const{
		return networkable::Uint32(getValue()).byteRepr();
	}

	BlockTarget::BlockTarget(NetworkBuffer* networkBuffer){
		auto compressed = networkable::Uint32(networkBuffer).getValue();
		value = CryptoPP::Integer( compressed & 0xffffff) << (8*((compressed >> 3*8) - 3));
	}
} // namespace ressources
