#include <iostream>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include <asio.hpp>

#include "constants.hpp"
#include "util.hpp"
#include "node.hpp"
#include "networkable.hpp"

int main(){
	int status;
	status = mkdir(constants::DATA_PATH.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if( status != 0 && errno != EEXIST){
		char buffer[ 256 ];
		std::cout << "Error when creating base directory : " << strerror_r( errno, buffer, 256 ) << std::endl;
	}
	status = mkdir(constants::BLOCKCHAIN_DB.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if( status != 0 && errno != EEXIST){
		char buffer[ 256 ];
		std::cout << "Error when creating directory for blockchain : " << strerror_r( errno, buffer, 256 ) << std::endl;
	}

	auto debug = spdlog::stderr_color_mt("debug");
	asio::io_context io_context;
	Node node(io_context);
	io_context.run();

	return 0;
}
