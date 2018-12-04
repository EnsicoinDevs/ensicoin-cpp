#ifndef MESSAGEHANDLER_HPP
#define MESSAGEHANDLER_HPP

#include "networkbuffer.hpp"
#include "messages.hpp"
#include "node.hpp"
#include "connection.hpp"

namespace network{
	
	class MessageHandler{
		private:
			using message_type = message::Message::message_type;
			NetworkBuffer* buffer;
			Node* node;
			Connection::pointer conn;
		
			void onWhoAmI();
			void onWhoAmIAck();
			
			void onUnknown();
		
		public:
			MessageHandler(message::Message::message_type type,
						   NetworkBuffer* buffer_,
						   Node* node_,
						   Connection::pointer conn_);
	};
} // namespace network


#endif /* MESSAGEHANDLER_HPP */
