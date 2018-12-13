#ifndef MESSAGEHANDLER_HPP
#define MESSAGEHANDLER_HPP

#include "networkbuffer.hpp"
#include "messages.hpp"
#include "node.hpp"
#include "connection.hpp"

#include <memory>
#include <spdlog/spdlog.h>

namespace network{
	
	class MessageHandler{
		private:
			using message_type = message::Message::message_type;
			NetworkBuffer* buffer;
			Node* node;
			Connection::pointer conn;
		
			std::shared_ptr<spdlog::logger> logger;

			void onWhoAmI();
			void onWhoAmIAck();
			
			void onInv();

			void onUnknown();
		
		public:
			MessageHandler(message::Message::message_type type,
						   NetworkBuffer* buffer_,
						   Node* node_,
						   Connection::pointer conn_,
						   std::shared_ptr<spdlog::logger> logger_);
	};
} // namespace network


#endif /* MESSAGEHANDLER_HPP */
