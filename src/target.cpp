#include "target.hpp"
#include "blocks.hpp"

namespace ressources{
	void BlockTarget::update(SlidingWindow window){
		value = window.average();
	}

	bool BlockTarget::blockInferior(const Block& target) const {
		return target.compressedHash() < value;
	}
} // namespace ressources
