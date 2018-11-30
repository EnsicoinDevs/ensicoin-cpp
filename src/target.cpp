#include "target.hpp"
#include "blockchain.hpp"
#include "blocks.hpp"
#include "constants.hpp"

namespace ressources{
	void BlockTarget::update(BlockIndex* bIndex, int timeTaken,const std::string& currentHash){
		auto blockWindow = bIndex->getWindowFrom(currentHash);
	}

	bool BlockTarget::blockInferior(const Block& target) const {
		return target.integerHash() < value;
	}

	std::string BlockTarget::compressed() const{

	}
} // namespace ressources
