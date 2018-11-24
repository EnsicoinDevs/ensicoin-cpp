#include <iostream>

#include <sstream>
#include <string>
#include <cryptopp/eccrypto.h>
#include <cryptopp/ripemd.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/oids.h>
#include <cryptopp/osrng.h>

#include "crypto.hpp"

using namespace CryptoPP;

std::string hexPublicKey(const ECP::Point q){
	std::ostringstream qx;
	qx << std::hex << q.x;
	std::ostringstream qy;
	qy << std::hex << q.y;

	std::string x(qx.str());
	std::string y(qy.str());
	x = std::string("0",65-x.length()) + x;
	y = std::string("0",65-y.length()) + y;
	
	std::string pubKey(x+y);
	pubKey.erase(std::remove(pubKey.begin(), pubKey.end(), 'h' ), pubKey.end());

	return pubKey;
}

std::string ripemd160(const std::string& binaryMessage){
	std::string value;
	RIPEMD160 ripeHash;

	StringSource ss( binaryMessage, true,
		new HashFilter( ripeHash,
				new StringSink( value)
		)
	);

	return value;
}

std::string sha256(const std::string& binaryMessage){
	std::string value;
	SHA256 shaHash;

	StringSource ss( binaryMessage, true,
		new HashFilter( shaHash,
				new StringSink( value)
		)
	);
	return value;
}

ECDSASignature::ECDSASignature(std::string message, eCurve::PrivateKey privateKey){
	eCurve::Signer signer( privateKey );
	AutoSeededRandomPool prng;

	StringSource ss( message, true, 
		new SignerFilter( prng,
			signer,
			new StringSink(signature)
		)
	);
}

ECDSASignature::ECDSASignature(std::string encodedSignature){
	StringSource ss(encodedSignature, true,
		new HexDecoder( new StringSink(signature) )
	);
}

std::string ECDSASignature::hex() const {
	std::string asciiSignature;

	StringSource ss( signature, true,
		new HexEncoder(
			new StringSink( asciiSignature ),
			false
		)
	);

	return asciiSignature;
}

bool ECDSASignature::verify(std::string message, std::string publicKeyString) const {
	eCurve::PublicKey publicKey;

	HexDecoder decoder;
	decoder.Put( (byte*)&publicKeyString[0], publicKeyString.size());
	decoder.MessageEnd();

	ECP::Point q;
	size_t len = decoder.MaxRetrievable();

	q.identity = false;
	q.x.Decode(decoder, len/2);
	q.y.Decode(decoder, len/2);
	
	publicKey.Initialize( ASN1::secp256k1(), q);

	eCurve::Verifier verifier(publicKey);
	bool result = verifier.VerifyMessage( (const byte*)&message[0], message.size(), (const byte*)&signature[0], signature.size() );

	return result;
}
