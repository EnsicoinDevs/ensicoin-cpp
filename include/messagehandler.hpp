#ifndef MESSAGEHANDLER_HPP
#define MESSAGEHANDLER_HPP

#include <memory>

#include "messages.hpp"

class Node;
class Connection;


class Handler {
	private:
		struct params{
			Message::messagePointer message; 
			Node* node;
			std::shared_ptr<Connection> connection;
		};
		
		static bool whoami(params&);
		static bool unknown(params&, std::string);
	public:
		static bool handle(rapidjson::Document* doc, Node* node,std::shared_ptr<Connection> connection);
	
};

#endif /* MESSAGEHANDLER_HPP */
