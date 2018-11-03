#include "node.hpp"
#include "connection.hpp"
#include "messages.hpp"

#include <asio.hpp>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>


Node::Node(asio::io_context& io_context) : acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 4224)) {
	acceptor.listen();
	run();

	Block GenesisBlock({0,{"ici cest limag"},"","",1566862920,42},{});
	
	TransactionIdentifier testID = { "obiwan kenobi", 42};
	InputTransaction testInput = { testID, std::stack<std::string>({"Bip", "Bop"})};
	OutputTransaction testOutput = { 42, {"Truc", "Bidule"}};
	
	Transaction testTransaction(-1, {"I AM A FLAG", "A FLAGGY FLAG"}, {testInput, testInput}, {testOutput, testOutput});
	
	std::string testTrHash = testTransaction.hash();
	InvData invData("t", {testTrHash});

	const std::string johynIP("78.248.188.120");
	const std::string myIP("82.235.104.10");
	auto messageTest = std::make_shared<WhoAmI>();
	auto invTest = Message::messagePointer( new Inv(invData));
	auto getDataTest = Message::messagePointer( new GetData(invData));
	auto sendGenesis = std::make_shared<BlockMessage>(std::make_shared<Block>(GenesisBlock));
	auto msgTestTr = std::make_shared<TransactionMessage>(std::make_shared<Transaction>(testTransaction));
	auto msgMempool = std::make_shared<GetMempool>();

	std::cout << getDataTest->str() << std::endl;

	Connection::pointer testConnection = Connection::create(io_context, this);
	connections.push_back(testConnection);
	testConnection->bind( asio::ip::address::from_string(johynIP));

	testConnection->sendMessage(invTest);
}

void Node::run(){
	Connection::pointer newConnection = Connection::create(acceptor.get_executor().context(), this);
	acceptor.async_accept(newConnection->getSocket(), std::bind( &Node::handleAccept, this, newConnection ));

	/*for(auto& conn : connections){
		conn->idle();
	}*/
}

void Node::handleAccept(Connection::pointer newConnection){
	newConnection->start();
	connections.push_back(newConnection);
	run();
}
