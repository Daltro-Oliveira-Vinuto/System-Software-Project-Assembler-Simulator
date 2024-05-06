#include "support.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

void invert_unordered_map(std::unordered_map<std::string, int>& opcode_table,
						  std::unordered_map<int, std::string>& opcode_table_inverted) {

	std::unordered_map<std::string, int>::iterator it;

	for(it = opcode_table.begin(); it != opcode_table.end(); it++) {
		std::string instruction;
		int opcode;

		instruction = it->first;
		opcode = it->second;

		opcode_table_inverted[opcode] = instruction;
	}
}


bool read_main_arguments(int argc, char **argv, std::string& file) {
	bool success = false;

	if (argc != 2) {
		printf("Error! Number of arguments is invalid!\7\n");
		success = false;
	} else {
		file = argv[1];
		success = true;
	}


	return success;
}


void print_vector(const std::vector<std::string>& vector, 
					const std::string& string,
					bool new_line) {

	for(unsigned i = 0; i < vector.size(); i++) {
		std::cout << vector[i] << string;
		if (new_line) {
			std::cout << std::endl;
		}

	}

	std::cout << std::endl;
}



bool read_file(const std::string& file_name,
			  std::vector<std::string>& lines) {

	bool success = false;
	std::fstream file;

	file.open(file_name, std::ios::in);

	if (file.is_open()) {
		std::string line;

		while(getline(file, line, '\n')) {
			lines.push_back(line);
		}


		file.close();
		success = true;
		std::cout << "File: " << file_name << " readed with success!\n";

	} else {
		std::cout << "Error to open file: " << file_name << std::endl;
		success = false;
	}

	return success;
}


bool save_file(const std::string& file_name,
				std::vector<std::string>& lines,
				const std::string string) {
	bool success = false;
	std::fstream file;

	file.open(file_name, std::ios::out);

	if (file.is_open()) {

		for(unsigned i = 0; i < lines.size(); i++) {
			file << lines[i];
			file << string;
		}


		file.close();
		success = true;
		std::cout << "File: " << file_name << " saved with success!\n";

	}else {
		std::cout << "Error to save file: " << file_name << std::endl;
		success = false;
	}

	return success;
}


void convert_to_lowercase(std::vector<std::string>& lines) {
	std::vector<std::string> temporary_lines;

	temporary_lines.assign(lines.begin(), lines.end());
	
	for(unsigned i = 0; i < temporary_lines.size(); i++) {
		std::string line = temporary_lines[i];

		std::transform(line.begin(), line.end(), 
			           line.begin(), ::tolower);

		temporary_lines[i] = line;
	}


	lines.assign(temporary_lines.begin(), temporary_lines.end());
}

// used to acess unordered_map value in gdb debugger
std::string make_string(const char* char_ptr) {
	return char_ptr;
}









