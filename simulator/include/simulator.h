#include <string>
#include <unordered_map>
#include "support.h"

#ifndef SIMULATOR_CPP
#define SIMULATOR_CPP

bool simulator(const std::string& object_file);

bool find_stop_position(const std::vector<int>& vector_machine_words,
						int& stop_position);

void load_tables(std::unordered_map<std::string, int>& opcode_table,
				std::unordered_map<std::string, int>& directive_table);

#endif