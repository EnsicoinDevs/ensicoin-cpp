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

#include "transaction.hpp"
#include "crypto.hpp"
#include "script.hpp"
#include "blocks.hpp"
#include "hashmemory.hpp"
#include "messages.hpp"

using namespace CryptoPP;

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

	Document* d = new Document();
	d->SetObject();
	
	WhoAmI* messageTest = new WhoAmI();
	Value messageJson = messageTest->json(d);

	StringBuffer buffer;
    	PrettyWriter<StringBuffer> writer(buffer);
    	messageJson.Accept(writer);

	std::cout << buffer.GetString() << std::endl;

	return 0;
}
