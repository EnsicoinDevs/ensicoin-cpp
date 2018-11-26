#include "messages.hpp"
#include "transaction.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <memory>
#include <string>
#include <vector>

namespace message{
	TransactionMessage::TransactionMessage(ressources::Transaction::pointer txPtr) :
		Message(tx),
		transaction(txPtr) {}
	TransactionMessage::TransactionMessage(NetworkBuffer* 
			networkBuffer) :
		Message(block),
		transaction(std::make_shared\
				<ressources::Transaction>(networkBuffer)) {}
	ressources::Transaction::pointer TransactionMessage::getTx() 
		const{
		return transaction;
	}

	std::string TransactionMessage::payload() const{
		return transaction->byteRepr();
	}
} // namespace message
