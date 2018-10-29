#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <map>
#include <cryptopp/eccrypto.h>
#include <cryptopp/hex.h>
#include <cryptopp/oids.h>
#include <cryptopp/osrng.h>
#include <cryptopp/filters.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include <asio.hpp>

#include "transaction.hpp"
#include "crypto.hpp"
#include "script.hpp"
#include "blocks.hpp"
#include "messages.hpp"
#include "node.hpp"

using namespace CryptoPP;
using namespace asio;
using asio::ip::tcp;


int main(){
	Block GenesisBlock({0,{"ici cest limag"},"","",1566862920,42},{});

	AutoSeededRandomPool prng, rrng;

	eCurve::PrivateKey privateKey;
	privateKey.Initialize( prng, ASN1::secp256k1() );
	eCurve::Signer signer(privateKey);

	eCurve::PublicKey publicKey;
	privateKey.MakePublicKey( publicKey );

	const ECP::Point q = publicKey.GetPublicElement();

	std::string message("TEST PLEASE");
	ECDSASignature signature(message, privateKey);

	std::vector<std::string> code = {"OP_DUP","OP_HASH160", ripemd160(hexPublicKey(q)), "OP_EQUAL", "OP_VERIFY", "OP_CHECKSIG"};
	std::stack<std::string> data({signature.hex(), hexPublicKey(q)});
	Script testScript(code.begin(), code.end(), data, message);

	TransactionIdentifier testID = { "obiwan kenobi", 42};
	InputTransaction testInput = { testID, std::stack<std::string>({signature.hex(), hexPublicKey(q)})};
	OutputTransaction testOutput = { 42, code};

	Transaction testTransaction(-1, {"I AM A FLAG", "A FLAGGY FLAG"}, {testInput, testInput}, {testOutput, testOutput});

	WhoAmI* messageTest = new WhoAmI();
	Node node;

	const std::string johynStringIP("78.248.188.120");
	asio::ip::tcp::socket socket = node.sendMessage(messageTest, asio::ip::address::from_string(johynStringIP));
	Document d = node.readJSON(socket);
	
	rapidjson::StringBuffer docBuffer;
	rapidjson::PrettyWriter<StringBuffer> writer(docBuffer);
	d.Accept(writer);

	const std::string docStr = docBuffer.GetString();

	std::cout << docStr << std::endl;

	return 0;
}
