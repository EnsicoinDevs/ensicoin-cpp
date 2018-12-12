#ifndef NODE_HPP
#define NODE_HPP

#include "blockchain.hpp"
#include "blocks.hpp"
#include "connection.hpp"
#include "hashmemory.hpp"
#include "mempool.hpp"
#include "transaction.hpp"
#include "utxo.hpp"

#include <asio.hpp>
#include <map>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <vector>

/// \brief Node handling messages and processing
class Node{
	private:
		/// \brief Logger
		std::shared_ptr<spdlog::logger> logger;

		/// \brief Mempool of the Node
		manager::Mempool mempool;
		/// \brief Main Blockchain
		manager::BlockIndex blockchain;
		
		/// \brief Accept TCP connections from other nodes
		asio::ip::tcp::acceptor acceptor;
		std::map<std::string,
				 network::Connection::pointer> connectedNodes; ///< Vector of all current connections
		std::map<std::string,
			     network::Connection::pointer> connectedRelays;
		std::map<std::string,
			     network::Connection::pointer> unregistred;

		/// \brief Handles a connection
		/// \param newConnection a shared pointer to the connection
		void handleAccept(
				network::Connection::pointer newConnection);
	public:
		void registerConnection(network::Connection::pointer conn);

		/// \brief Construct a Node
		/// \param io_context an asio::io_context
		/// \details Executes the startup 
		/// routine and creates the mempool
		explicit Node(asio::io_context& io_context);

		/// \brief Checks if a transaction is known (either
		/// Orphan or Regular)
		bool transactionExists(std::string txHash) const;
		
		/// \brief Main loop of Node
		///
		void run();
};

#endif /* NODE_HPP */
