#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <cryptopp/eccrypto.h>
#include <cryptopp/hex.h>
#include <cryptopp/oids.h>
#include <cryptopp/osrng.h>
#include <cryptopp/filters.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include <asio.hpp>

#include "constants.hpp"
#include "crypto.hpp"
#include "debug.hpp"
#include "script.hpp"
#include "blocks.hpp"
#include "node.hpp"
#include "networkable.hpp"

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
	
	int status;
	status = mkdir(DATA_PATH.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if( status != 0 && errno != EEXIST){
		char buffer[ 256 ];
		std::cout << "Error when creating directory : " << strerror_r( errno, buffer, 256 ) << std::endl;
	}
	else{
		std::cout << "Directory set" << std::endl;
	}
	
	Var_uint x(0x123456789acd);
	std::cout << " X : " << x.byteRepr() << std::endl;
	auto byteX =  x.asBytes();
	Var_uint y(byteX);
	std::cout << " Y : " << y.byteRepr() << std::endl;

	asio::io_context io_context;
	//Node node(io_context);
	//io_context.run();

	return 0;
}
