#include "messages.hpp"
#include "blocks.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <memory>
#include <string>
#include <vector>

namespace message{
	TransactionMessage::TransactionMessage(std::shared_ptr\
			<Transaction> txPtr) :
		Message(tx),
		transaction(txPtr) {}
	TransactionMessage::TransactionMessage(NetworkBuffer* 
			networkBuffer) :
		Message(block),
		transaction(std::make_shared\
				<Transaction>(networkBuffer)) {}
	std::shared_ptr<Transaction> TransactionMessage::getTx() 
		const{
		return transaction;
	}

	std::string TransactionMessage::payload() const{
		return transaction->byteRepr();
	}
} // namespace message
