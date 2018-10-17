#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <string>
#include <vector>
#include <iterator>
#include <stack>

class Script{
	private:
		std::vector<std::string>::iterator codePointer, endCode;
		std::stack<std::string> data;
		bool valid;
		std::string transactionHash;
	public:
		Script(	std::vector<std::string>::iterator initalInstruction,
		 	std::vector<std::string>::iterator endInstruction, 
			std::stack<std::string> initalData,
			std::string transactionHash);
		void debug();
		bool step();
};

#endif /* SCRIPT_HPP */
