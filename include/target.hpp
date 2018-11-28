#ifndef TARGET_HPP
#define TARGET_HPP

#include <string>
#include <deque>

namespace ressources{

	class Block;
	class Blockchain;
	class SlidingWindow;

	/// \brief A target for a Block
	class BlockTarget{
		private:
			/// \brief Target to whom a Block needs to be
			/// inferior
			std::string value;
		public:
			/// \brief Updates a target to reflect the time taken to 
			/// generate 2016 Blocks
			void update(const SlidingWindow& window);
			/// \brief Checks if a Block in inferior to the
			/// BlockTarget
			bool blockInferior(const Block& block) const;
	};
	
	class SlidingWindow{
		private:
			std::deque<BlockTarget> targets;
			uint32_t size;
		public:
			void slide(uint64_t timeTaken);
			uint32_t average();
			SlidingWindow(uint32_t size, const Blockchain& blockchain);
	};

} // namespace ressources

#endif /* TARGET_HPP */
