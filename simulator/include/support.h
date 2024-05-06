#include <string>
#include <vector>
#include <unordered_map>

#ifndef SUPPORT_H
#define SUPPORT_H


bool read_main_arguments(int argc, char** argv, 
						std::string& file);


bool read_file(const std::string& file_name, 
				std::vector<std::string>& lines);

bool save_file(const std::string& file_name,
				std::vector<std::string>& lines,
				const std::string string);

void print_vector(const std::vector<std::string>& vector, 
					const std::string& string="",
					bool new_line=true);


void convert_to_lowercase(std::vector<std::string>& lines);

std::string make_string(const char* char_ptr);

void invert_unordered_map(std::unordered_map<std::string, int>& opcode_table,
				std::unordered_map<int, std::string>& opcode_table_inverted);


#endif