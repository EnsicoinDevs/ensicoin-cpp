#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <ctime>
#include <string>

class Message {
	private:
		int magic;
		std::string type;
		std::time_t timestamp;
	public:
		std::string getType();
};



#endif /* MESSAGES_HPP */
