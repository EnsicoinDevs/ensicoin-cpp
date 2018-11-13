#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <string>
#include <vector>
#include <iterator>
#include <stack>

/// \brief Script used in Transaction
class Script{
	private:
		/// \brief Iterators used to execute the script
		std::vector<std::string>::iterator codePointer, endCode;
		/// \brief Stack of the program
		std::stack<std::string> data;
		/// \brief Validity of the Transaction due to the
		/// current execution
		bool valid;
		/// \brief Hash to be used to check the signature
		std::string transactionHash;
	public:
		/// \brief Create a Script from the necessary data
		Script(	std::vector<std::string>::iterator initalInstruction,
		 	std::vector<std::string>::iterator endInstruction, 
			std::stack<std::string> initalData,
			std::string transactionHash);
		/// \brief Shows the content of the stack
		void debug();
		/// \brief Execute one code operation and returns
		/// the validity
		bool step();
		/// \brief Gives the state of the Script
		bool done() const;
};

#endif /* SCRIPT_HPP */
