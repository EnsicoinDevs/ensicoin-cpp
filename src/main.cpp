#include <vector>
#include <string>
#include <stack>
#include <cryptopp/eccrypto.h>
#include <cryptopp/hex.h>
#include <cryptopp/oids.h>
#include <cryptopp/osrng.h>
#include <cryptopp/filters.h>

#include <asio.hpp>

#include "crypto.hpp"
#include "script.hpp"
#include "blocks.hpp"
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

	asio::io_context io_context;
	Node node(io_context);
	io_context.run();

	return 0;
}
