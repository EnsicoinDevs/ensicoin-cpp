#include <iostream>
/*#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>*/

#include <algorithm>
#include <vector>
#include <string>
#include <stack>
#include <cryptopp/eccrypto.h>
#include <cryptopp/hex.h>
#include <cryptopp/oids.h>
#include <cryptopp/osrng.h>
#include <cryptopp/filters.h>

#include <sstream>

#include "transaction.hpp"
#include "crypto.hpp"

using namespace CryptoPP;

int main(){
	AutoSeededRandomPool prng, rrng;
	
	eCurve::PrivateKey privateKey;
	privateKey.Initialize( prng, ASN1::secp256k1() );
	eCurve::Signer signer(privateKey);

	eCurve::PublicKey publicKey;
	privateKey.MakePublicKey( publicKey );

	const Integer& x1 = privateKey.GetPrivateExponent();
	const ECP::Point q = publicKey.GetPublicElement();
	
	std::string message("TEST PLEASE");
	ECDSASignature signature(message, privateKey);
	
	ECDSASignature signBis(signature.hex());

	std::cout << "Message : " << message << std::endl;
	std::cout << "Signature : " << signature.hex() << std::endl; 
	std::cout << "Signature (2) : " << signBis.hex() << std::endl; 
	std::cout << "Signature Function : " << signature.verify(message, hexPublicKey(q)) << std::endl;
	std::cout << "Signature Function (2) : " << signBis.verify(message, hexPublicKey(q)) << std::endl;
	
	std::vector<std::string> code = {"OP_TEST","OP_RETEST"};
	std::stack<std::string> data({"test1", "test2"});
	Script test(code.begin(), code.end(), data);

	return 0;
}
