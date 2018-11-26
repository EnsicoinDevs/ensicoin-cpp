#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "blocks.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"
#include "transaction.hpp"

#include <ctime>
#include <memory>
#include <string>
#include <vector>


/// \brief Networkable content to be sent
namespace message{

	/// \brief Base class for all network communcation
	class Message : public std::enable_shared_from_this<Message>,
	public networkable::Networkable {
		public:
			/// \brief Shared_pointer to a Message
			using messagePointer = std::shared_ptr\
					       <Message>;
			/// \brief Possible Message types
			enum message_type{	
				whoami, 
				whoamiack, 
				getaddr, 
				addr, 
				inv, 
				getdata, 
				notfound, 
				block, 
				tx, 
				getblocks, 
				getmempool,
				unknown};
			/// \brief get the type of the Message
			message_type getType() const;
			/// \brief Get the message_type as a string
			std::string getTypeAsString() const;
			/// \brief Parse a string into message_type
			message_type typeFromString(
					const std::string& 
					typeString) const;
			std::string byteRepr() const override;
		protected:
			/// \brief Defined in constants.hpp
			int magic;
			/// \brief Type of message
			message_type type;

			/// \brief Get the payload as a byteRepr
			virtual std::string payload() const = 0;

			/// \brief Construct a Message with empty 
			/// Payload
			/// \param type set the type of the content
			explicit Message(message_type type);
	};

	/// \brief Message used for intializing the Connection
	class WhoAmI : public Message {
		private:
			std::string payload() const override;
			/// \brief Defined in constants.hpp
			networkable::Uint32 version;
			/// \brief Current time
			networkable::Uint64 timestamp;
		public:
			/// \brief Construct a WhoAmI Message
			WhoAmI();
			/// \brief Parses a WhoAmI Message
			/// \param networkBuffer buffer containing
			/// the raw Representation
			explicit WhoAmI(NetworkBuffer* 
					networkBuffer);
	};

	/// \brief Acknowledge a Connection after a WhoAmI
	class WhoAmIAck : public Message {
		private:
			std::string payload() const override;
		public:
			/// \brief Construct a WhoAmIAck
			WhoAmIAck();
	};

	/// \brief Send all known Node
	class Addr : public Message {
		private:
			std::string payload() const override;
			/// \brief List of known Node address
			std::vector<networkable::Address> addresses;
		public:
			/// \brief Construt a Addr from addresses
			explicit Addr(std::vector<networkable::\
					Address> addressList);
			/// \brief Parse a Addr Message
			/// \param networkBuffer buffer containing
			/// the raw representation
			explicit Addr(NetworkBuffer* networkBuffer);
	};

	/// \brief Ask for all the Node known
	class GetAddr : public Message{
		private:
			std::string payload() const override;
		public:
			/// Construct a GetAddr
			GetAddr();
	};

	/// \brief Message to get the JSON String representing 
	/// ressources
	class GetData : public Message {
		private:
			std::string payload() const override;
			/// \brief Ressources asked
			std::vector<networkable::Inv_vect> data;
		public:
			/// \brief Gives the type of the data asked
			networkable::Inv_vect::ressource_type 
				dataType() const;
			/// \brief Gives all the hashes of the 
			/// asked data
			std::vector<std::string> dataAsked() const;

			/// \brief Creates a GetData 
			/// from a vector of networkable::Inv_vect
			explicit GetData(std::vector<networkable::\
					Inv_vect> invData);
			/// \brief Parses a GetData
			/// \param networkBuffer buffer containing 
			/// the raw data
			explicit GetData(NetworkBuffer* 
					networkBuffer);
	};

	/// \brief Transmit Block or Transaction to a peer
	class Inv : public Message {
		private:
			std::string payload() const override;

			/// \brief data to be transmited
			std::vector<networkable::Inv_vect> data;
		public:
			/// \brief Retrives the type from InvData
			networkable::Inv_vect::ressource_type 
				getRessourceType() const;
			/// \brief Retriveses the hashes of each 
			/// Inv_vect
			std::vector<std::string> 
				getRessourceHashes() const;

			/// \brief Create a Inv from a vector of
			/// Inv_vect
			explicit Inv(std::vector<networkable::\
					Inv_vect> invData);
			/// \brief Parse an Inv
			/// \param networkBuffer buffer containing 
			/// the raw data
			explicit Inv(NetworkBuffer* networkBuffer);
	};


	/// \brief Message signaling an asked ressource is unknown
	class NotFound : public Message {
		private:
			std::string payload() const override;
			/// \brief Unknown ressources
			std::vector<networkable::Inv_vect> data;
		public:
			/// \brief Create a NotFound from the 
			/// ressources
			explicit NotFound(std::vector<networkable::\
					Inv_vect> invData);
			/// \brief Parses a NotFound
			/// \param networkBuffer buffer containing
			/// the raw data
			explicit NotFound(NetworkBuffer* 
					networkBuffer);
	};

	/// \brief Message sending a Block over
	class BlockMessage : public Message {
		private:
			std::string payload() const override;
			/// \brief Pointer to the sent Block to avoid
			/// RAM usage
			std::shared_ptr<Block> blockRef;
		public:
			/// \brief Returns a pointer to the Block
			std::shared_ptr<Block> getBlock() const;
			/// \brief Create a BlockMessage from a Block
			explicit BlockMessage(std::shared_ptr<Block>
					blockPtr);
			/// \brief Parses a binary string in a 
			/// BlockMessage
			/// \param networkBuffer buffer containing
			/// the raw data
			explicit BlockMessage(NetworkBuffer* 
					networkBuffer);
	};

	/// \brief Message sending a Transaction over
	class TransactionMessage : public Message {
		private:
			std::string payload() const override;
			/// \brief Pointer to the sent Transaction to
			/// avoid RAM usgae
			ressources::Transaction::pointer transaction;
		public:
			/// \brief Returns a shared pointet to teh ressources::Transaction
			ressources::Transaction::pointer getTx() const;
			/// \brief Create a TransactionMessage from a
			/// Transaction
			explicit TransactionMessage(ressources::Transaction::pointer);
			/// \brief Parses a binary string in a 
			/// TransactionMessage
			/// \param networkBuffer buffer containing 
			/// the raw data
			explicit TransactionMessage(NetworkBuffer* 
					networkBuffer);
	};

	/// \brief Message asking a node to sync Block 
	/// until a certain point
	class GetBlocks : public Message {
		private:
			std::string payload() const override;
			/// \brief Known Block for reference
			std::vector< std::string > blockHashes;
			/// \brief Where to stop sending Block
			std::string stopHash;
		public:
			/// \brief Create a GetBlocks from the 
			/// known hashes
			GetBlocks(std::vector< std::string > 
					hashList, 
					std::string stopHashString);
			/// \brief Parses a binary string in a 
			/// GetBlocks
			/// \param networkBuffer buffer containing 
			/// the raw data
			explicit GetBlocks(NetworkBuffer* 
					networkBuffer);
	};

	/// \brief Message to sync the Mempool with another node
	class GetMempool : public Message{
		private:
			std::string payload() const override;
		public:
			/// \brief Create a GetMempool
			GetMempool();
	};

}; // namespace message

#endif /* MESSAGES_HPP */
