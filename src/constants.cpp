#include "constants.hpp"

#include <cstdlib>
#include <string>

const std::string DATA_PATH = std::string(std::getenv("HOME")) + "/.ensicoincpp";
const std::string UTXO_DB = DATA_PATH + "/utxo";
const std::string BLOCKCHAIN_DB = DATA_PATH + "/blockchain";
const std::string BLOCKCHAIN_STAT = DATA_PATH + "/bcstats.json";
