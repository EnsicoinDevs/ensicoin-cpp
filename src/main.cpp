#include <iostream>
/*#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>*/

#include <vector>
#include <string>
#include <stack>

#include "transaction.hpp"
#include "hash.hpp"

int main(){
	std::vector<std::string> code = {"OP_TEST","OP_RETEST"};
	std::stack<std::string> data({"test1", "test2"});
	Script test(code.begin(), code.end(), data);
	test.debug();

	

	std::cout << ripemd160("hello world") << std::endl;

	return 0;
}
