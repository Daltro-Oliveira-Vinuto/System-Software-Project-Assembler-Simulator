#include "simulator.h"
#include "support.h"
#include <string>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <limits>

 

bool simulator(const std::string& object_file) {
	bool success;
	std::vector<std::string> lines;

	success = read_file(object_file, lines);

	if (success) {
		std::unordered_map<std::string, int> opcode_table;
		std::unordered_map<int, std::string> opcode_table_reversed = {};
		std::unordered_map<std::string, int> directive_table;

		load_tables(opcode_table, directive_table);
		invert_unordered_map(opcode_table, opcode_table_reversed);

		bool found_errors = false;

		if (lines.size() > 0) {

			std::string line = lines[0];
			std::vector<int> vector_machine_words = {};

			std::stringstream stream(line);
			std::string input_word;
			while(stream >> input_word) {
				vector_machine_words.push_back(stoi( input_word ) );
			}	

			bool running;
			int stop_position = -1;
			success = find_stop_position(vector_machine_words, stop_position);

			if (success == false) {
				printf("Error! A ultima instrucao deve ser 'stop'!\7\n");
				running = false;
			} else {
				running = true;
			}

			int pc, acc;
			pc = 0;
			acc = 0;

			int output_data;
			bool should_print = false;

			while(running){
				int opcode;

				if (pc >= 0 && (unsigned)pc < vector_machine_words.size()) {
					opcode = vector_machine_words[pc];

					if (opcode_table_reversed.find(opcode) == opcode_table_reversed.end()) {
						printf("Error! Opcode invalido!\7\n");
						running = false;
						found_errors = true;
					} else {
						std::string assembly_operation;

						assembly_operation = opcode_table_reversed.at(opcode);

						// execute instruction
						if (assembly_operation == "add") {
							int data, data_address;
							data_address = vector_machine_words[pc+1];
							data = vector_machine_words[data_address];

							acc+= data;

						}else if(assembly_operation == "sub") {
							int data, data_address;
							data_address = vector_machine_words[pc+1];
							data = vector_machine_words[data_address];

							acc-= data;

						} else if (assembly_operation == "mul") {
							int data, data_address;
							data_address = vector_machine_words[pc+1];
							data = vector_machine_words[data_address];

							acc*= data;

						} else if (assembly_operation == "div") {
							int data, data_address;
							data_address = vector_machine_words[pc+1];
							data = vector_machine_words[data_address];

							acc/= data;

						} else if (assembly_operation == "copy") {
							int data, data_address_from, data_address_to;
							data_address_from = vector_machine_words[pc+1];
							data_address_to = vector_machine_words[pc+2];

							vector_machine_words[data_address_to] =
									vector_machine_words[data_address_from];
							
						} else if (assembly_operation == "load") {
							int data, data_address;
							data_address = vector_machine_words[pc+1];
							data = vector_machine_words[data_address];

							acc = data;

						} else if (assembly_operation == "store") {
							int data, data_address;
							data_address = vector_machine_words[pc+1];
							
							vector_machine_words[data_address] = acc;

						} else if (assembly_operation == "input") {
							int data, data_address;
							data_address = vector_machine_words[pc+1];
							
							std::cin >> data;
							//getline(std::istream, data);
							//fflush(stdin);

							vector_machine_words[data_address] = data;

							vector_machine_words[data_address] = data;

						} else if (assembly_operation == "output") {
							int data, data_address;
							data_address = vector_machine_words[pc+1];
							data = vector_machine_words[data_address];

							// print value after print the value of PC, ACC
							//printf("%d\n",data);
							should_print = true;
							output_data = data;

						}


						// update pc
						if (assembly_operation == "stop") {
							running = false;
							success = true;
						} else if (assembly_operation == "copy") {
							pc+=3;
						} else if (assembly_operation == "jmp") {
							pc = vector_machine_words[pc+1];
						} else if (assembly_operation == "jmpn"
								&& acc < 0) {
							pc = vector_machine_words[pc+1];
						} else if (assembly_operation == "jmpp"
								&& acc > 0) {
							pc = vector_machine_words[pc+1];
						}  else if (assembly_operation == "jmpz"
								&& acc == 0) {
							pc = vector_machine_words[pc+1];
						} else {
							pc+=2;
						}

						std::cout <<"Apos executar (";
						std::cout << assembly_operation << "): ";
						printf("PC <- %d ACC <- %d ", pc, acc);
						
						
						if (should_print) {
							printf(" SAIDA: %d ",output_data);
							should_print = false;
						} else {
							printf(" ");
						}


						char key_pressed;
						scanf("%c", &key_pressed);

						if (assembly_operation == "input") {
							scanf("%c", &key_pressed);
						}


						if (pc > stop_position) {
							running = false;
							found_errors = true;
							printf("Segmentation Fault!\n");
						} 
						
	
				    }

				} else {
					running = false;
					found_errors = true;
					printf("Segmentation Fault!\7\n");
				}

			} 

		} else {
			success = false;
			printf("Error! Arquivo vazio sem instrucoes!\7\n");
		}

		if (!found_errors) {
			success = true;
		} else {
			success = false;
		}

 	} else {
 		printf("Error to read file: ");
 		std::cout << object_file << " \n";
 		success = false;
 	}


 	return success;
}

bool find_stop_position(const std::vector<int>& vector_machine_words,
						int& stop_position) {
	bool success = false;
	bool running = true;
	int pc = 0;

	while(running) {
		if (pc >= 0 && (unsigned)pc < vector_machine_words.size()) {
			int opcode;

			opcode = vector_machine_words[pc];

			if (opcode == 9) {
				pc+= 3;
			} else if (opcode == 14) {
				stop_position = pc;
				running = false;
				success = true;
			} else if (opcode >= 1 && opcode <= 13) {
				pc+= 2;
			} else {
				success = false;
				running = false;
			}
		} else {
			success = false;
			running = false;
		}
	}

	return success;
}

void load_tables(std::unordered_map<std::string, int>& opcode_table,
				std::unordered_map<std::string, int>& directive_table) {

	directive_table["const"] = 0;
	directive_table["space"] = 1;

	opcode_table["add"] =  1;
	opcode_table["sub"] =  2;
	opcode_table["mul"] =  3;
	opcode_table["div"] =  4;
	opcode_table["jmp"] =  5;
	opcode_table["jmpn"] =  6;
	opcode_table["jmpp"] =  7;
	opcode_table["jmpz"] =  8;
	opcode_table["copy"] =  9;
	opcode_table["load"] =  10;
	opcode_table["store"] =  11;
	opcode_table["input"] =  12;
	opcode_table["output"] =  13;
	opcode_table["stop"] =  14;

}
