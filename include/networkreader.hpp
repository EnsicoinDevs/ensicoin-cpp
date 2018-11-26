#ifndef NETWORKREADER_HPP
#define NETWORKREADER_HPP

#include <memory>

#include "messages.hpp"
#include "networkbuffer.hpp"

class Node;

namespace network{

	class Connection;

	/// \brief Handles actions to be taken on message::Message
	class NetworkReader {
		private:
			/// \brief Track the progress in reading a
			/// message::Message
			struct state{
				/// \brief Currently reading a message::Message
				bool reading;
				/// \brief message::Message being read
				message::Message::message_type type;
				/// \brief Postion in reading a message
				uint8_t readingPosition;
			};
			/// \brief Current state of reading
			state currentState;
			/// \brief How much bytes are to be read from the Connection
			uint64_t toBeRead;
			/// \brief How much bytes have been read
			uint64_t readSize;
			
			
			Node* node;
			std::shared_ptr<Connection> connection;
			/// \buffer storing the raw data
			NetworkBuffer* buffer;

			/// \brief Does a WhoAmI handshake beetween the nodes
			bool whoami();
			/// \brief Checks if data if known
			/// \returns Respond with true if all ressources are
			/// known (including if the ressource type is
			/// unknown)
			bool inv();
			/// \brief Responds with all ressources asked
			/// \brief \returns false if any of the ressources is
			/// not found.
			bool getdata();
			/// \brief reports a message is unknown
			static bool unknown();
		public:
			/// \brief Construct a NetworkReader with reference to
			/// Connection and Node
			NetworkReader(Node* node,
					std::shared_ptr<Connection> connection,
					NetworkBuffer* buff);

			/// \brief handle raw data
			void handle();

			/// \brief Get the number of bytes to read
			/// \returns 0 if the message::Message is totally parsed
			uint64_t readRequest() const;
			/// \brief Return the total number of bytes read
			uint64_t readCount() const;
	};

} // namespace network

#endif /* NETWORKREADER_HPP */
