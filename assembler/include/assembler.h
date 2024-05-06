#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

bool assemble(const std::string& assembly_file);

unsigned preprocess(const std::string& assembly_file,
					std::vector<std::string>& lines,
					std::vector<std::string>& lines_macro_expansion,
					const std::unordered_map<std::string, int>& opcode_table);

unsigned expand_macros(std::vector<std::string>& lines);


void load_tables(std::unordered_map<std::string, int>& opcode_table,
				std::unordered_map<std::string, int>& directive_table);


unsigned first_pass(const std::vector<std::string>& lines, 
					const std::unordered_map<std::string, int>& opcode_table,
					const std::unordered_map<std::string, int>& directive_table,
					std::unordered_map<std::string, int>& symbol_table);

unsigned second_pass(const std::vector<std::string>& lines, 
					const std::unordered_map<std::string, int>& opcode_table,
					const std::unordered_map<std::string, int>& directive_table,
					const std::unordered_map<std::string, int>& symbol_table,
					std::vector<std::string>& object_vector);



void try_save_preprocessed_file(const std::string& assembly_file,
								std::vector<std::string>& lines);

bool try_save_object_file(const std::string &assembly_file, 
						  std::vector<std::string>& object_vector);

unsigned check_for_lexic_errors(const std::vector<std::string>& lines,
			const std::unordered_map<std::string, int>& opcode_table);

void clean_file(std::vector<std::string>& lines);

unsigned correct_labels_position(std::vector<std::string>& lines);

unsigned force_text_data_order(std::vector<std::string>& lines);

void remove_commas(std::vector<std::string>& lines);

#endif