#ifndef MESSAGEHANDLER_HPP
#define MESSAGEHANDLER_HPP

#include <memory>

#include "messages.hpp"

class Node;
class Connection;

/// \brief Handles actions to be taken on Message
class Handler {
	private:
		/// \brief Parameters functions use in handling
		/// a Message
		struct params{
			/// \brief JSON representation of a Message
			rapidjson::Document* doc;
			/// \brief Node to act on state
			Node* node;
			/// \brief Connection to respond
			std::shared_ptr<Connection> connection;
		};
		
		/// \brief Does a WhoAmI handshake beetween the nodes
		static bool whoami(params&);
		/// \brief Checks if data if known
		/// \returns Respond with true if all ressources are
		/// known (including if the ressource type is
		/// unknown)
		static bool inv(params&);
		/// \brief Responds with all ressources asked
		/// \brief \returns false if any of the ressources is
		/// not found.
		static bool getdata(params&);
		/// \brief reports a message is unknown
		static bool unknown();
	public:
		/// \brief Parses the JSON document and takes action
		/// \param doc JSON representation of Message
		/// \param node pointer to Node
		/// \param connection pointer to the current 
		/// Connection
		static bool handle(rapidjson::Document* doc, Node* node,std::shared_ptr<Connection> connection);
	
};

#endif /* MESSAGEHANDLER_HPP */
