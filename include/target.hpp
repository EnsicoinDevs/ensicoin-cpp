#ifndef TARGET_HPP
#define TARGET_HPP

#include "networkbuffer.hpp"
#include "networkable.hpp"

#include <cryptopp/integer.h>
#include <string>

namespace manager{
	class BlockIndex;
}

namespace ressources{

	class Block;

	/// \brief A target for a Block
	class BlockTarget final : public networkable::Networkable{
		private:
			/// \brief Target to whom a Block needs to be
			/// inferior
			CryptoPP::Integer value;
		public:
			/// \brief Updates a target to reflect the time taken to 
			/// generate 2016 Blocks
			void update(manager::BlockIndex* bIndex,
					int timeTaken,const std::string& currentHash);
			/// \brief Checks if a Block in inferior to the
			/// BlockTarget
			bool blockInferior(const Block& block) const;
			/// \brief Get the compressed value
			/// \todo FIXE
			uint32_t getValue() const;
			std::string byteRepr() const override;
			explicit BlockTarget(NetworkBuffer* networkBuffer);
	};
} // namespace ressources

#endif /* TARGET_HPP */
