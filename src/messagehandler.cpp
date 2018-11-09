#include "messagehandler.hpp"

#include "connection.hpp"
#include "messages.hpp"
#include "node.hpp"

#include <iostream>
#include <memory>
#include <rapidjson/document.h>

bool Handler::handle(rapidjson::Document* doc, Node* node, std::shared_ptr<Connection> connection){
	params param;
	param.node = node;
	param.connection = connection;
	std::string type = (*doc)["type"].GetString();
	
	std::cerr << type << " from " << connection->remote() << std::endl;
	if (type == "whoami"){
		param.message = std::make_shared<WhoAmI>();
		return whoami(param);
	}
	else
		return unknown(param, type);
}

bool Handler::whoami(params& p){
	p.connection->wave();
	return true;
}

bool Handler::unknown(params& p, std::string unknownType){
	return false;
}
