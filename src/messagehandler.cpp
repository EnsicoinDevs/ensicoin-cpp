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
	param.doc = doc;
	std::string type = (*doc)["type"].GetString();
	
	std::cerr << type << " from " << connection->remote() << std::endl;
	if (type == "whoami"){
		return whoami(param);
	}
	if( type == "inv"){
		return inv(param);
	}
	else
		return unknown();
}

bool Handler::inv(params& p){
	auto message = std::make_shared<Inv>(p.doc);
	if(message->getRessourceType() == "t"){
		for(auto& hash : message->getRessources()){
			if(!p.node->transactionExists(hash)){
				p.connection->sendMessage(message->respondRequest());
				return false;
			}
		}
		return true;
	}
	else if (message->getRessourceType() == "b"){
		std::cerr << "Blocks not supported" << std::endl;
		return true;
	}
	else{
		std::cerr << "Invalid type in Inv : " << message->getRessourceType() << std::endl;
		return true;
	}
}

bool Handler::whoami(params& p){
	p.connection->wave();
	return true;
}

bool Handler::unknown(){
	std::cerr << "Unknown message" << std::endl;
	return false;
}
