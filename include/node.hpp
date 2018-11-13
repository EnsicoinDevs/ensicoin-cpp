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
#include <memory>
#include <vector>

/// \brief Node handling messages and processing
///
class Node{
	private:
//		HashMemory mainChain;
//		std::vector< HashMemory > secondaryChains;
		
		/// \brief Mempool of the Node
		Mempool mempool;
		/// \brief Main Blockchain
		Blockchain blockchain;
		
		/// \brief Accept TCP connections from other nodes
		asio::ip::tcp::acceptor acceptor;
		std::vector<Connection::pointer> connections; ///< Vector of all current connections
		
		/// \brief Handles a connection
		/// \param newConnection a shared pointer to the connection
		void handleAccept(Connection::pointer newConnection);
	public:
		/// \brief Construct a Node
		/// \param io_context an asio::io_context
		/// \details Executes the startup routine and creates mempools 
		explicit Node(asio::io_context& io_context);
		
		/// \brief Main loop of Node
		///
		void run();
};

#endif /* NODE_HPP */
