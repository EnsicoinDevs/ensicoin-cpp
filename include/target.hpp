#ifndef TARGET_HPP
#define TARGET_HPP

#include <cryptopp/integer.h>
#include <string>

class BlockIndex;

namespace ressources{

	class Block;

	/// \brief A target for a Block
	class BlockTarget{
		private:
			/// \brief Target to whom a Block needs to be
			/// inferior
			CryptoPP::Integer value;
		public:
			/// \brief Updates a target to reflect the time taken to 
			/// generate 2016 Blocks
			void update(BlockIndex* bIndex, int timeTaken,const std::string& currentHash);
			/// \brief Checks if a Block in inferior to the
			/// BlockTarget
			bool blockInferior(const Block& block) const;
			std::string compressed() const;
	};
} // namespace ressources

#endif /* TARGET_HPP */
