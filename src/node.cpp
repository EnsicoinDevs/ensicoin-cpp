#include "node.hpp"
#include "connection.hpp"
#include "constants.hpp"
#include "messages.hpp"
#include "networkable.hpp"
#include "util.hpp"

#include <asio.hpp>
#include <functional>
#include <iostream>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <stdexcept>

Node::Node(asio::io_context& io_context) : 
	logger(spdlog::stdout_color_mt("console")),
	mempool(logger),
	blockchain(logger),
	acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(),
												 constants::LISTEN_PORT)){
	
	acceptor.listen();
	run();
	
	logger->set_level(spdlog::level::trace);
	logger->info("Node started");

	//ressources::Block GenesisBlock({0,{"ici cest limag"},"","",1566862920,42},{});
	
	ressources::TransactionIdentifier testID("obiwan kenobi", 42);

	ressources::InputTransaction testInput(testID, ressources::Script({},"KABAK"));

	ressources::OutputTransaction testOutput(42, ressources::Script({},"BAKAAK"));
	
	ressources::Transaction testTransaction(-1, {"I AM A FLAG", "A FLAGGY FLAG"}, {testInput, testInput}, {testOutput, testOutput});
	auto testTxPtr = std::make_shared<ressources::Transaction>(testTransaction);

	std::string testTrHash = testTransaction.hash();
	networkable::Inv_vect invData(networkable::Inv_vect::ressource_type::txRes, testTrHash);

	const std::string johynIP("78.248.188.120");
	const std::string myIP("82.235.104.10");
	const std::string jauvioNathan("92.129.133.102");
	const std::string localIP("127.0.0.1");
	auto messageTest = std::make_shared<message::WhoAmI>();
	//auto invTest = message::Message::pointer( new message::Inv({invData}));
	//auto getDataTest = message::Message::pointer( new message::GetData({invData}));
	//auto sendGenesis = std::make_shared<message::BlockMessage>(std::make_shared<ressources::Block>(GenesisBlock));
	//auto msgTestTr = std::make_shared<message::TransactionMessage>(testTxPtr);
	//auto msgMempool = std::make_shared<message::GetMempool>();

	network::Connection::pointer testConnection = network::Connection::create(io_context, this, logger);
	connections.push_back(testConnection);
	testConnection->bind(asio::ip::address::from_string(johynIP));

	//testConnection->sendMessage(invTest);
	//testConnection->sendMessage(msgMempool);
}

void Node::run(){
	network::Connection::pointer newConnection = network::Connection::create(acceptor.get_executor().context(), this, logger);
	acceptor.async_accept(newConnection->getSocket(), std::bind( &Node::handleAccept, this, newConnection ));

	/*for(auto& conn : connections){
		conn->idle();
	}*/
}

bool Node::transactionExists(std::string txHash) const {
	return mempool.exists(txHash) || mempool.orphanExists(txHash);
}

void Node::handleAccept(network::Connection::pointer newConnection){
	newConnection->start();
	connections.push_back(newConnection);
	run();
}
