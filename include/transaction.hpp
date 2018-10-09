#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>
#include <vector>
#include <iterator>
#include <stack>

class Script{
	private:
		std::vector<std::string>::iterator codePointer, endCode;
		std::stack<std::string> data;
		bool valid;
	public:
		Script(	std::vector<std::string>::iterator initalInstruction,
		 	std::vector<std::string>::iterator endInstruction, 
			std::stack<std::string> initalData);
		void debug();
		void step();
};

/*class Transaction{
	private:
		int version;
		std::vector<std::string> flags;
		
};*/

#endif /* TRANSACTION_HPP */
