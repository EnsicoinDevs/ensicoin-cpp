#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include "hashmemory.hpp"
#include "script.hpp"

#include <iterator>
#include <memory>
#include <rapidjson/document.h>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using namespace rapidjson;

/// \brief Reference to the OutputTransaction of another Transaction
struct TransactionIdentifier{
	/// \brief Hash of the refrenced Transaction
	std::string transactionHash;
	/// \brief Index of the OutputTransaction
	int index;
	
	/// \brief Get the raw string representation
	/// \details the raw string representation is obtained
	/// by concatanateing all the fields
	std::string str() const;
	/// \brief Get the JSON reperesentation
	/// \param document Document used for the Allocation of
	/// members
	Value json(Document* document) const;
	/// \brief Load the fields from a JSON object
	/// \param val JSON object to be parsed
	void load(Value* val);
};
/// \brief Repersent an input of a Transation
struct InputTransaction{
	/// \brief Reference to another Transaction
	TransactionIdentifier previousOutput;
	/// \brief The script attached to the input
	std::stack<std::string> inputStack;
	
	/// \brief Get the raw string representation
	/// \details the raw string representation is obtained
	/// by concatanateing all the fields
	std::string str() const;
	/// \brief Get the JSON reperesentation
	/// \param document Document used for the Allocation of
	/// members
	Value json(Document* document) const;
	/// \brief Load the fields from a JSON object
	/// \param val JSON object to be parsed
	void load(Value* val);
};

/// \brief Represent the output of a Transaction
struct OutputTransaction{
	/// \brief The Value make available
	int value;
	/// \brief the code attached to the input
	std::vector<std::string> scriptInstructions;

	/// \brief Get the raw string representation
	/// \details the raw string representation is obtained
	/// by concatanateing all the fields
	std::string str() const;
	/// \brief Get the JSON reperesentation
	/// \param document Document used for the Allocation of
	/// members
	Value json(Document* document) const;
	/// \brief Load the fields from a JSON object
	/// \param val JSON object to be parsed
	void load(Value* val);
};

class Mempool;

enum TXType { Orphan, Regular };

/// \brief A class representing an exchange of ensicoins
class Transaction : public std::enable_shared_from_this<Transaction> {
	protected:
		/// \brief Referenced in constants.hpp
		int version;
		/// \brief User defined flags
		std::vector<std::string> transactionFlags;
		/// \brief All InputTransaction
		std::vector<InputTransaction> inputs;
		/// \brief All OutputTransaction
		std::vector<OutputTransaction> outputs;
	public:
		/// \brief Creates an empty Transaction
		Transaction();
		/// \brief Create a Transaction from all data fields
		Transaction(	int ver, 	
				std::vector<std::string> initialFlags, 
				std::vector<InputTransaction> initialInputs, 
				std::vector<OutputTransaction> initialOutputs);
		/// \brief Parse a transaction from JSON
		/// \param doc the JSON to be parsed
		explicit Transaction( rapidjson::Document* doc);
		
		/// \brief returns the version used
		int getVersion() const;
		/// \brief returns the flags
		std::vector<std::string> getFlags() const;
		/// \brief returns all Transaction referenced
		std::vector<TransactionIdentifier> getInputsId() const;
		/// \brief Calculate the value of all
		/// OutputTransaction
		int outputValue() const;
		
		/// \brief Get all the InputTransaction
		std::vector<InputTransaction> getInputs() const;
		/// \brief Get the script for the designed
		/// OutputTransaction
		std::vector<std::string> getScriptOfOutput(int index) const;

		/// \brief Checks if the transaction has an 
		/// OutputTransaction
		/// \param index index to be verified if exists
		bool hasOutput(int index) const;
		/// \brief Get the value of the index-th output
		int valueOfOutput(int index) const;
		/// \brief Gets the number of OutputTransaction
		int outputCount() const;

		/// \brief Checks if the Transaction is valid by
		/// itself
		bool check();
		
		/// \brief Get the raw string representation
		/// \details The raw string is obtained by 
		/// concatanateing all the raw strings of the
		/// fields
		std::string rawStr() const;
		/// \brief Get the hash to be used for signing the
		/// Transaction
		std::string hashWithoutInputs() const;
		/// \brief Taking two times sha256 of the rawStr
		std::string hash() const;
		/// \brief JSON representation of the Transaction
		/// \param includeInputs does the representation
		/// include the inputs
		/// \param document Document used for Allocating
		/// members
		Value json(bool includeInputs, Document* document) const;
		/// \brief Get the JSON string representing the
		/// Transaction
		std::string str() const;
};

#endif /* TRANSACTION_HPP */
