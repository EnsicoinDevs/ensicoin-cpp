#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "messages.hpp"
#include "logger.hpp"

#include <asio.hpp>
#include <memory>

class Node;

/// \brief Objects to handle communiction between Node
namespace network{

	/// \brief A Connection to a peer
	class Connection : 
		public std::enable_shared_from_this<Connection>{
		public:
			enum status { Initiated ,Waiting, 
						  Idle, WaitingAck,
						  Ack };
			enum type { nodepeer, relaypeer, walletpeer, peer};
			/// \brief Shared pointer to a Connection
			using pointer = std::shared_ptr<Connection>;

			/// \brief Creates a shared_ptr of Connection
			/// \param io_context io_context for async operation
			/// \param node pointer to a Node to handle actions
			static pointer create(asio::io_context& io_context,
								  Node* node,
								  std::shared_ptr<Logger> lg);
			/// \brief IP Adress of remote connection
			std::string remote() const;
			/// \brief Reference to the socket
			asio::ip::tcp::socket& getSocket();
			/// \brief Initial state of Connection
			void start();
			/// \brief Send a Message
			/// \param message shared_ptr of a Message
			void sendMessage(message::Message::pointer message);
			/// \brief Connect to a remote address
			/// \param address IP address to connect to
			void bind(asio::ip::address address);
			/// \brief Idle loop of the Connection
			void idle();

			inline type getPeerType() const{
				return peerType;
			}

			/// \brief Exchange of message::WhoAmI
			void updateStatus(int connectionVersion,
							  type peerType_);
		private:
			std::string readAll();

			/// \brief Construct a Connection
			Connection(asio::io_context& io_context,
					   Node* node,
					   std::shared_ptr<Logger> logger);

			/// \brief Called wwhen recevieving a message
			void handleHeader();

			/// \brief Called when reading a Message
			void handleMessage(const networkable::MessageHeader&
					header);

			/// \brief Called when writing a Message
			void handleWrite(const  std::string& type);

			/// \brief Socket used for network access
			asio::ip::tcp::socket socket;

			/// \brief Pointer to the node to handle Message actions
			Node* node;

			int versionUsed;
			status currentStatus;
			/// \brief NetworkBuffer for the connection
			NetworkBuffer netBuffer;

			/// \brief Message buffered before WhoAmI exchange
			std::vector<message::Message::pointer> bufferedMessages;
			/// \brief Buffered content read from the node
			asio::streambuf buffer;
			std::shared_ptr<Logger> logger;
			type peerType;
	};

} // namespace network

#endif /* CONNECTION_HPP */
