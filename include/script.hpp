#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <iterator>
#include <stack>
#include <string>
#include <vector>

namespace ressources{
	/// \brief Script used in Transaction
	class Script : networkable::Networkable { 
		public:
			/// \brief Instruction set of the Script
			enum opcode : unsigned char {
				OP_FALSE = 0x00,
				OP_TRUE = 0x50,
				OP_DUP = 0x64,
				OP_EQUAL = 0x78,
				OP_VERIFY = 0x8c,
				OP_HASH160 = 0xa0,
				OP_CHECKSIG = 0xaa,
				OP_STACK = 0x01,
				OP_UNKNOWN
			};
			/// \brief to store the argument for
			/// a given opcode if necessary
			struct instruction{
				/// \brief Operation to be performed
				opcode operation;
				/// \brief Argument of the operation
				unsigned char argument;
			};
			/// \brief Converts a uchar to an opcode
			instruction parseInstruction(unsigned char
					chr) const;
			/// \brief Code to be excuted by the Script
			using code = std::vector<instruction>;

			/// \brief Gives the number of instruction
			size_t length() const;

			/// \brief Change the signing Hash
			void setSHash(const std::string& shash);

			std::string byteRepr() const override;
			/// \brief Create a Script from the 
			/// necessary data
			Script(	code instructions,
					std::string shash);
			/// \brief Create a empty script
			Script();
			/// \brief Extract a script from a 
			/// NetworkBuffer
			explicit Script(NetworkBuffer* 
					networkBuffer);
			/// \brief Shows the content of the stack
			void debug();
			/// \brief Execute one code operation and
			/// returns the validity
			bool step();
			/// \brief Gives the state of the Script
			bool done() const;
			/// \brief chains the script execution
			void chain(Script* following);
		private:
			/// \brief Code to be run
			code scriptInstructions;
			/// \brief Iterator to track the current
			/// instruction
			code::iterator codePointer;
			/// \brief Stack of the program
			std::stack<std::vector<unsigned char> > data;
			/// \brief Validity of the Transaction
			/// due to the current execution
			bool valid;
			/// \brief Hash to be used to check
			/// the signature
			std::string signingHash;
	};

} // namespace ressources

#endif /* SCRIPT_HPP */
