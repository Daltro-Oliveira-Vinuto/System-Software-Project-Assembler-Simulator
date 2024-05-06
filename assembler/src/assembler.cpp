
#include "support.h"
#include "assembler.h"

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <sstream>


void remove_commas(std::vector<std::string>& lines) {
	std::vector<std::string> temporary_lines = {};

	for(unsigned i = 0; i < lines.size(); i++) {
		std::string line = lines[i];
		std::stringstream stream(line);
		std::string word = "";
		std::string new_line = "";
		std::string comma = ",";
		std::string space = " ";

		while(stream >> word) {
			if (word.find(comma) != std::string::npos) {
				word.replace(word.find(comma), space.size(), space);
			}
			new_line += word + " ";
		}

		new_line.erase(new_line.size()-1);

		temporary_lines.push_back(new_line);
	}

	for(unsigned i = 0; i < temporary_lines.size(); i++) {
		std::string line = temporary_lines[i];
		std::stringstream stream(line);
		std::string word = "";
		std::string new_line = "";

		while(stream >> word) {
			new_line += word + " ";
		}

		new_line.erase(new_line.size()-1);

		temporary_lines[i] = (new_line);
	}


	lines.assign(temporary_lines.begin(), temporary_lines.end());

}

unsigned force_text_data_order(std::vector<std::string>& lines) {
	unsigned number_of_errors = 0;
	std::string secao_data = "secao data";
	std::string secao_text = "secao text";

	std::vector< std::string > temporary_lines = {};

	int secao_data_begin=-1, secao_data_end=-1;
	int secao_text_begin=-1, secao_text_end=-1;
	for(unsigned i = 0; i < lines.size(); i++) {
		std::string line = lines[i];

		if (line == secao_data) {
			secao_data_begin = i+1;
			if (secao_text_begin != -1) {
				secao_text_end = i-1;
			}
		}

		if (line == secao_text) {
			secao_text_begin = i+1;
			if (secao_data_begin != -1) {
				secao_data_end = i-1;
			}
		}

	}

	if (secao_text_begin == -1) {
		printf("Erro semantico no programa! secao text nao encontrada!\n");
		number_of_errors+=1;
	}

	if (secao_data_begin == -1) {
		printf("Erro semantico no programa! secao data nao encontrada!\n");
		number_of_errors+=1;
	}

	if (secao_text_begin != -1 && secao_text_end == -1) {
		secao_text_end = lines.size()-1;
	}

	if (secao_data_begin != -1 && secao_data_end == -1) {
		secao_data_end = lines.size()-1;
	}

	if (secao_text_begin != -1 && secao_data_begin != -1) {

		for(int i = secao_text_begin; i <= secao_text_end; i++) {
			std::string line = lines[i];

			temporary_lines.push_back(line);
		}

		for(int i = secao_data_begin; i <= secao_data_end; i++) {
			std::string line = lines[i];

			temporary_lines.push_back(line);
		}

		lines.clear();
		lines.assign(temporary_lines.begin(), temporary_lines.end());
	}

	return number_of_errors;
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

unsigned expand_macros(std::vector<std::string>& lines) {
	unsigned number_of_errors = 0;
	
	std::unordered_map<std::string, std::vector<std::string> > macros;
	std::unordered_map<std::string, std::vector<std::string> > macro_args;

	std::vector<std::string> temporary_lines = {};

	for(unsigned i = 0; i < lines.size(); i++) {
		std::string line = lines[i];
		std::stringstream stream(line);
		std::string word = "";
		std::string new_line = "";
		std::vector<std::string> new_line_vector = {};
		while(stream >> word) {	
			new_line_vector.push_back(word);
		}

		
		// if this line is a macro definition then save the macro in the MDT
		if (new_line_vector[0][new_line_vector[0].size()-1] == ':') {
			// the first string is a label
			if (new_line_vector.size() >=2 && 
				new_line_vector[1] == "macro") {
				std::string macro_name = new_line_vector[0];
			    macro_name.erase(macro_name.size()-1); // delete symbol ':'

				// a macro definition start in this line
				// then copy macro from definition to endmacro
				std::vector<std::string> macro_lines = {};  

				std::vector<std::string> vector_macro_args = {};
				unsigned j = 2;
				while ( j< new_line_vector.size() ) {
					vector_macro_args.push_back(new_line_vector[j]);
					j++;
				}
				macro_args[macro_name] = vector_macro_args;

				bool found_end_macro = false;
				i+= 1;
				while (i < lines.size() && lines[i] != "endmacro") {
					// if it is a macro

					std::stringstream macro_stream(lines[i]);
					std::string macro_stream_word = "";
					std::vector<std::string> vector_macro_stream = {};
					while(macro_stream >> macro_stream_word) {
						vector_macro_stream.push_back(macro_stream_word);
					}

					if (macros.find(vector_macro_stream[0]) != macros.end()) {

						std::string macro_name_first = vector_macro_stream[0];
						if (vector_macro_stream.size() > 3) {
							printf("Error sintatico na linha %d! Numero de argumentos da macro maior que dois\7\n",i);
							number_of_errors+= 1;
						}
						std::vector<std::string> vector_args = {};

						unsigned j = 1;
						while( j < vector_macro_stream.size()) {
							vector_args.push_back(vector_macro_stream[j]);
							j++;
						}

						if (vector_args.size() != macro_args[macro_name_first].size()) {
							printf("Error semantico na linha %d! Numero de argumentos da definicao da macro é diferente do numero de argumentos da chamada da macro!\7\n",i);	
						} else {
							std::vector<std::string> vector_macro_lines = macros[macro_name_first];
							std::vector<std::string> new_vector_macro_lines = {};
							for(unsigned j = 0; j < vector_macro_lines.size(); j++) {
								std::string macro_line = vector_macro_lines[j];

								for(unsigned k = 0; k < vector_args.size(); k++) {
									if (macro_line.contains(macro_args[macro_name_first][k]) ) {
										// why vector_args[k].size() not works?
										macro_line.replace(macro_line.find(macro_args[macro_name_first][k]),
														   vector_args.size(),
														   vector_args[k]+" ");

									}
								}

								new_vector_macro_lines.push_back(macro_line);
							}

							for(unsigned j = 0; j < new_vector_macro_lines.size(); j++) {
								std::string final_line = new_vector_macro_lines[j];

								macro_lines.push_back(final_line);
							}

						}


						i+=1;
					} else {
						// if it is not macro
						macro_lines.push_back(lines[i]);
						i+= 1;

						if (lines[i] == "endmacro") {
							found_end_macro = true;
						}
					}
				}

				if (found_end_macro == false) {
					number_of_errors++;
					std::cout << "Erro sintatico! endmacro da macro ";
					std::cout << macro_name << " nao encontrada!\n";
				}


				macros[macro_name] = macro_lines;
				continue;
			}

		} 
		
		
		// if it is a macro call then expand the macro and
		// replace the macro parameters  
		if (macros.find(new_line_vector[0]) != macros.end()) {
			std::string macro_name = new_line_vector[0];
			if (new_line_vector.size() > 3) {
				printf("Error sintatico na linha %d! Numero de argumentos da macro maior que dois\7\n",i);
				number_of_errors+= 1;
			}
			std::vector<std::string> vector_args = {};

			unsigned j = 1;
			while( j < new_line_vector.size()) {
				vector_args.push_back(new_line_vector[j]);
				j++;
			}

			if (vector_args.size() != macro_args[macro_name].size()) {
				printf("Error semantico na linha %d! Numero de argumentos da definicao da macro é diferente do numero de argumentos da chamada da macro!\7\n",i);	
			} else {
				std::vector<std::string> vector_macro_lines = macros[macro_name];
				std::vector<std::string> new_vector_macro_lines = {};
				for(unsigned j = 0; j < vector_macro_lines.size(); j++) {
					std::string macro_line = vector_macro_lines[j];

					for(unsigned k = 0; k < vector_args.size(); k++) {
						if (macro_line.contains(macro_args[macro_name][k]) ) {
							// why vector_args[k].size() not works?
							macro_line.replace(macro_line.find(macro_args[macro_name][k]),
											   vector_args.size(),
											   vector_args[k]+" ");

						}
					}

					new_vector_macro_lines.push_back(macro_line);
				}

				for(unsigned j = 0; j < new_vector_macro_lines.size(); j++) {
					std::string final_line = new_vector_macro_lines[j];

					temporary_lines.push_back(final_line);
				}
				continue;
					
			}

		}	
		
		// if it is not not a macro definion neither expansion then
		// just create new_line
		for(unsigned j = 0; j < new_line_vector.size(); j++) {
			if (j == new_line_vector.size()-1) {
				new_line += new_line_vector[j];
			} else {
				new_line += new_line_vector[j]+ " ";
			}
		}

		if (new_line != "") {
			temporary_lines.push_back(new_line);
		}



	}	
		
	lines.clear();
	lines.assign(temporary_lines.begin(), temporary_lines.end());
	
	

	return number_of_errors;
}


unsigned preprocess(const std::string& assembly_file,
					std::vector<std::string>& lines,
					std::vector<std::string>& lines_macro_expansion,
					const std::unordered_map<std::string, int>& opcode_table) {

	unsigned number_of_errors = 0;

	// read assembly file
	bool success = read_file(assembly_file, lines);

	if (success == false) {
		std::cout << "Error to read file " << assembly_file << std::endl;
		number_of_errors = 1;
	} else {
		// file readed with success 

		// convert all strings to lowercase
		convert_to_lowercase(lines);

		// remove tabs, unnecessary spaces and newlines;
		clean_file(lines);

		// correct labels alone in some line(the labels are put in the next line)
		number_of_errors += correct_labels_position(lines);

		// replace commas by spaces
		remove_commas(lines);

		number_of_errors+= expand_macros(lines);

		number_of_errors+= force_text_data_order(lines);

		// check for lexic erros
		number_of_errors += check_for_lexic_errors(lines, opcode_table);
		lines_macro_expansion.assign(lines.begin(), lines.end());


	}

	return number_of_errors;
}


bool assemble(const std::string& assembly_file) {
	bool success = false;
	unsigned number_of_errors = 0;
	std::vector<std::string> lines, lines_macro_expansion;
	std::unordered_map<std::string, int> symbol_table;
	std::unordered_map<std::string, int> opcode_table;
	std::unordered_map<std::string, int> directive_table;
	std::vector<std::string> object_vector = {};

	load_tables(opcode_table, directive_table);

	number_of_errors+= preprocess(assembly_file, lines, lines_macro_expansion
								  ,opcode_table);

	try_save_preprocessed_file(assembly_file, lines_macro_expansion);


	number_of_errors += first_pass(lines, 
								   opcode_table,
								   directive_table,
								   symbol_table);

	number_of_errors += second_pass(lines,
									opcode_table,   
									directive_table, 
									symbol_table,
									object_vector);


	if (number_of_errors == 0) {
		success = true;
		success = try_save_object_file(assembly_file, object_vector);
	} else {
		printf("Error assemble failed!\7\n");

	}

	return success;
}

unsigned first_pass(const std::vector<std::string>& lines, 
					const std::unordered_map<std::string, int>& opcode_table,
					const std::unordered_map<std::string, int>& directive_table,
					std::unordered_map<std::string, int>& symbol_table) {
	unsigned number_of_errors = 0;

	unsigned memory_position = 0;

	for(unsigned i = 0; i < lines.size(); i++) {
		std::string line = lines[i];

		std::stringstream stream(line);
		std::string input_word;
		std::vector<std::string> vector_words = {};
		while(stream >> input_word) {
			vector_words.push_back(input_word);
		}

		for(unsigned j = 0; j < vector_words.size(); j++) {
			std::string word = vector_words[j];

			// check the word is is label
			if (word[word.size()-1] == ':') {
				std::string label = word.erase(word.size()-1);

				// check if label is already in symbol table
				if (symbol_table.find(label) != symbol_table.end()) {
					printf("Erro semantico na linha %d, ", i+1);
					printf("rotulos so podem ser definidos uma unica vez!\n");
					number_of_errors++;
				} else {
					symbol_table[label] = memory_position;
				}
				continue;
				
			} else { 
			// the word is not a label then must be a opcode or directive

				// if word is opcode
				if (opcode_table.find(word) != opcode_table.end()) {
					std::string opcode = word;
					bool error = false;
					int number_of_arguments = vector_words.size()-1-j;
					if (opcode == "stop") {
						memory_position+= 1;

						if (number_of_arguments != 0) {
							error = true;
						}
					} else if (opcode == "copy") {
						memory_position+= 3;

						if (number_of_arguments != 2) {
							error = true;
						}
					} else {
						memory_position+= 2;

						if (number_of_arguments != 1) {
							error = true;
						}
					}

					if (error) {
						printf("Error sintatico na linha %d, ", i+1);
						printf("quantidade de operadores errada!\n");
						number_of_errors+= 1;
					}

				// if word is directive
				} else if (directive_table.find(word) != directive_table.end()) {
					int number_of_arguments = vector_words.size()-1-j;
					bool error = false;
					if (word == "const") {
						memory_position+= 1;
						if (number_of_arguments!= 1) {
							error = true;
						}
					} else if (word == "space") {
						if (number_of_arguments == 0) {
							memory_position+=1;
						} else if (number_of_arguments == 1) {
							unsigned argument = stoi( vector_words[j+1] );
							memory_position+= argument;
						}

						if (number_of_arguments != 0 &&
							number_of_arguments != 1) {
							error = true;
						}
					}

					if (error) {
						printf("Erro sintatico na linha %d, ", i+1);
						printf("numero de parametros invalido!\n");
						number_of_errors++;
					}
				} else {
					// if word is not opcode or directive then there's a error
					printf("Erro sintatico na linha %d, ", i+1);
					printf("comando deve ser um opcode ou diretiva!\n");
					number_of_errors++;
				}

				break;
			}
		}

	}



	return number_of_errors;
}	

unsigned second_pass(const std::vector<std::string>& lines, 
					const std::unordered_map<std::string, int>& opcode_table,
					const std::unordered_map<std::string, int>& directive_table,
					const std::unordered_map<std::string, int>& symbol_table,
					std::vector<std::string>& object_vector) {
	unsigned number_of_errors = 0;

	unsigned memory_position = 0;
	object_vector = {};

	for(unsigned i = 0; i < lines.size(); i++) {
		std::string line = lines[i];
		std::stringstream stream(line);
		std::string input_word;

		std::vector<std::string> vector_words = {};
		while(stream >> input_word) {
			vector_words.push_back(input_word);
		}

		for(unsigned j = 0; j < vector_words.size(); j++) {
			std::string word = vector_words[j];

			// check if it is a label, if true then go to the next word
			if (word[word.size()-1] == ':') {
				continue;
			} else {
				// if it is not a label then it must be a opcode or directive

				// if word is a opcode
				if (opcode_table.find(word) != opcode_table.end()) {
					std::string opcode = word;
					int opcode_number;

					opcode_number = opcode_table.at(opcode);
					object_vector.push_back(
						std::to_string(opcode_number) + " ");

					memory_position+= 1;

					j+= 1;
					while( j < vector_words.size()) {
						int offset = 0;
						std::string variable;

						if (vector_words[j].contains("+")
							|| (vector_words[j].contains("-"))
							) {
							std::string symbol_plus = "+";
							std::string symbol_minus = "-";
							std::string space = " ";
							bool operation_plus = false;
							bool operation_minus = false;
							int number;

							if (vector_words[j].contains(symbol_plus)) {
								vector_words[j].replace(
									vector_words[j].find(symbol_plus),
									space.size(), space);

								operation_plus = true;
							} else if (
								vector_words[j].contains(symbol_minus)
								) {
									vector_words[j].replace(
										vector_words[j].find(symbol_minus),
										space.size(), space);

									operation_minus = true;

							}

							std::stringstream aux_stream(vector_words[j]);
							std::string aux_word;
							std::vector<std::string> aux_vector = {};
							while(aux_stream >> aux_word) {
								aux_vector.push_back(aux_word);
							}

							variable = aux_vector[0];
							number = stoi(aux_vector[1]);

							if (operation_plus) {
								offset = number;
							} else if (operation_minus) {
								offset = -number;
							}
						} else {
							variable = vector_words[j];
						}

						if (symbol_table.find(variable) ==
							 symbol_table.end()) {
							printf("Erro semantico na linha %d, ", i+1);
							printf("identificador ");
							std::cout << variable << " ";
							printf(" nao encontrado!\n");
							number_of_errors+= 1;
						} else {
							int memory_value =
								 symbol_table.at(variable)+offset;

							object_vector.push_back(
								std::to_string(memory_value)+" ");
						}

						memory_position+=1;
						j+= 1;
						
					}
				}else if (directive_table.find(word) !=
						 directive_table.end()) {
					// if word is a directive
					std::string directive = word;
					int number_of_arguments = vector_words.size()-j-1;

					if (directive == "const") {
						int value_const = stoi(vector_words[j+1]);
						object_vector.push_back(
							std::to_string(value_const)+" ");
						memory_position+=1;
					} else if (directive == "space") {
						if (number_of_arguments == 0) {
							object_vector.push_back("0 ");
							memory_position+=1;
						} else if (number_of_arguments == 1) {
							unsigned argument = stoi(vector_words[j+1]);

							for(unsigned k = 0; k < argument; k++) {
								object_vector.push_back("0 ");
							}
							memory_position+= argument;
						}
					}
					break;
				} else {
					// the word is not opcode or a directive
					printf("Error sintatico na linha %d, ", i+1);
					printf("comando deve ser um opcode ou diretiva\n");
					number_of_errors++;
				}
			}

		}

	}

	return number_of_errors;
}

bool try_save_object_file(const std::string& assembly_file, 
						  std::vector<std::string>& object_vector){
		bool success = true;
		
		std::string object_file = assembly_file;
		std::string extension_obj = ".obj";
		std::string extension_asm = ".asm";
		std::string extension_mcr = ".mcr";
		if (assembly_file.contains(extension_asm)) {

			object_file.replace(object_file.find(extension_asm),
								extension_asm.size(), extension_obj);

			std::string string = "";
			if (save_file(object_file, object_vector, string)) {
				std::cout << "File: " << assembly_file << ", ";
				std::cout << "assembled with sucess to the object file: "
					<< object_file << std::endl;
			} else {
				success = false;
				std::cout <<"Error to save object file: " << object_file << std::endl;
			}

		} else if (assembly_file.contains(extension_mcr)) {

			object_file.replace(object_file.find(extension_mcr),
								extension_mcr.size(), extension_obj);

			std::string string = "";
			if (save_file(object_file, object_vector, string)) {
				std::cout << "File: " << assembly_file << ", ";
				std::cout << "assembled with sucess to the object file: "
					<< object_file << std::endl;
			} else {
				success = false;
				std::cout <<"Error to save object file: " << object_file << std::endl;
			}

		} else {
			printf("Error! Invalid assembly extension! Extension should be: .asm or .mcr\7\n");
		}

		return success;
}

void try_save_preprocessed_file(const std::string& assembly_file,
								std::vector<std::string>& lines) {
	std::string extension_mcr = ".mcr";
	std::string extension_pre = ".pre";
	std::string preprocess_file = assembly_file;
	if (assembly_file.contains(extension_mcr)) {

		preprocess_file.replace(preprocess_file.find(extension_mcr),
							extension_pre.size(),
							extension_pre);

		std::string string = "\n";
		if(save_file(preprocess_file, lines, string)) {
			std::cout << "File: "
			    << preprocess_file << " preprocessed, generated with success!\n";
		} else {
			std::cout << "Error to save preprocessed file: " << preprocess_file << std::endl;
		}
	}

}

unsigned check_for_lexic_errors(const std::vector<std::string>& lines,
		 const std::unordered_map<std::string, int>& opcode_table) {

	unsigned number_of_errors = 0;

	for(unsigned i = 0; i < lines.size(); i++) {
		std::string line = lines[i];

		std::stringstream stream(line);
		std::string input_word;
		std::vector< std::string > vector_words = {};

		while(stream >> input_word) {
			vector_words.push_back(input_word);
		}

		for(unsigned j = 0; j < vector_words.size(); j++) {
			std::string word = vector_words[j];

			// check the label size(should be at maximum 30 characters)
			if ( word.size() > 30 ) {
				number_of_errors+= 1;
				printf("Erro lexico na linha %d!", i); 
				printf("Numero de caracteres maior que 30!\n");
			}

			// check if the first character is a number(except after const,space)
			if ((j == 0) && (word[0] >= '0' && word[0] <= '9') ){
				printf("Erro lexico na linha %d!",i);
				printf(" Identificadores de variaveis e rotulos");
				printf(" nao podem comecar com numeros!\n");
				number_of_errors++;
			}else if ( (j>=1)) {
				if ( ( vector_words[j-1] != "const"
					&& vector_words[j-1] != "space") ) {

					if ((word[0] >= '0' && word[0] <= '9')) {
						printf("Erro lexico na linha %d!",i+1);
						printf(" Identificadores de variaveis e rotulos");
						printf(" nao podem comecar com numeros!\n");
						number_of_errors++;
					}
				}

			}

			// check if there is a special character other than underscore(_)
			// except + in variables, - after const and ':' in labels

			// check if it is a label or not
			if ((j == 0)) {
				for(unsigned k = 0; k < word.size(); k++) {
					char letter = word[k];

					if ( (k != word.size()-1 )
						&&(
						  (letter < '0' || letter > '9')
						  &&(letter <'a' || letter >'z')
						  &&(letter != '_')
						  ) )
						   {

						printf("Erro lexico na linha %d! O character %c e invalido!\n",
							i+1,letter);
						number_of_errors++;
					}else if ( (k == word.size()-1 && letter != ':')
						&&(
						  (letter < '0' || letter > '9')
						  &&(letter <'a' || letter >'z')
						  &&(letter != '_')
						  ) ) {

						printf("Erro lexico na linha %d! O character %c e invalido!\n",
							 	i+1, letter);
						number_of_errors++;
					}
				}
			} else if ((j >= 1)) {
					// check if there is a number after the keyword space
					if (vector_words[j-1] == "const") {
						for(unsigned k = 0; k < word.size(); k++) {
							char letter = word[k];

							if ( (k != 0) ) {
								if ((letter < '0' || letter > '9')) {
									printf("Erro sintatico na linha %d!",i+1);
									printf(", apos const deve vir numero!\n");
									number_of_errors++;
								}
							} else if ((k == 0) && 
										( (letter != '-')
										  && (letter < '0' || letter > '9') 
										  )
										) {
											printf("Erro sintatico na linha %d!",i+1);
											printf(", apos const deve vir numero!\n");
											number_of_errors++;
										}
						}
					} else if (opcode_table.find(vector_words[j-1]) !=
								opcode_table.end() ||
								(j>= 2 && (vector_words[j-2] == "copy")) 
								) {
						// the previous string was a opcode
						for(unsigned k = 0; k < word.size(); k++) {
							char letter = word[k];

							if ((letter < '0' || letter > '9')
								  &&(letter <'a' || letter >'z')
								  &&(letter != '_')
								  &&(letter != '-')
								  &&(letter != '+')
								  ) {
								printf("Erro lexico na linha %d!",i+1);
								printf(" O character %c e invalido!\n",letter);
								number_of_errors++;
							}
						}
					} else {
						// the previous string was not a opcode

						for(unsigned k = 0; k < word.size(); k++) {
							char letter = word[k];

							if ((letter < '0' || letter > '9')
								  &&(letter <'a' || letter >'z')
								  &&(letter != '_')
								  ) {
								printf("Erro lexico na linha %d!",i+1);
								printf(" O character %c e invalido!\n",letter);
								number_of_errors++;
							}
						}
					}

			}



		}
	}




	return number_of_errors;
}

void clean_file(std::vector<std::string>& lines) {
	std::vector<std::string> temporary_lines = {};

	// remove the tabs, unnecessary spaces, blank lines and commentaries
	for(unsigned i = 0; i < lines.size(); i++) {
		std::string line = lines[i];
		std::stringstream stream(line);
		std::string word = "";

		std::string new_line = "";
		while(stream >> word) {
			if (word[0] == ';') {
				break; // the rest of the line is a commentary
			}

			if (word != "\t" && word !="\n") {
				new_line+= word+" ";
			}
		}

		if (new_line != "") {
			// next line remove the unnecessary space after the last word
			new_line.erase(new_line.size()-1); 

			temporary_lines.push_back(new_line);
		}
	}

	lines.clear();
	lines.assign(temporary_lines.begin(), temporary_lines.end());

}

unsigned correct_labels_position(std::vector<std::string>& lines) {
	unsigned number_of_errors = 0;

	std::vector<std::string> temporary_lines = {};

	bool label_used = true;
	std::string label_alone = "";
	for(unsigned i = 0; i < lines.size(); i++) {
		std::string line = lines[i];
		std::stringstream stream(line);
		std::string word;
		std::vector<std::string> new_line_vector = {};

		while(stream >> word) {
			new_line_vector.push_back(word);
		}

		std::string new_line = "";

		// if line has only one element
		if (new_line_vector.size() == 1) {
			// and if this element is label
			if (new_line_vector[0][new_line_vector[0].size()-1] == ':') {
				// and if this label is alone in the line
				if (new_line_vector.size() == 1) {
					label_alone = new_line_vector[0];
					label_used = false;
					continue; // got to the next line
				} 
			}
		} 
		// if is not a label alone in the line
		// then check if label_alone != ""

		// if the label_alone == "" then the previous line don't have a label alone
		
		if (label_alone == "") {
			// then assembly new line
			for(unsigned i = 0; i < new_line_vector.size(); i++ ){
				if (i == new_line_vector.size() -1) {
					// is the last element of the new_line_vector
					new_line += new_line_vector[i];
				} else {
					// is not the last element of the new_line_vector
					new_line+= new_line_vector[i] + " ";
				}
			}
		} else {
			// otherwise concatenate the label alone with the actual line
			new_line+= label_alone + " ";
			label_alone = "";
			label_used = true;
			for(unsigned i = 0; i < new_line_vector.size(); i++ ){
				if (i == new_line_vector.size() -1) {
					// is the last element of the new_line_vector
					new_line += new_line_vector[i];
				} else {
					// is not the last element of the new_line_vector
					new_line+= new_line_vector[i] + " ";
				}
			}
		}
		

		if (new_line != "") {
			temporary_lines.push_back(new_line);
		}
	}

	// if there is a label alone in the last line of the assembly file
	// then this is a sintatic error
	if (label_used == false) {	
		number_of_errors += 1;
		printf("Erro sintatico! label deve vir acompanhado de instrucao, diretiva ou macro na mesma linha ou proxima linha!\7\n");
	}


	lines.clear();
	lines.assign(temporary_lines.begin(), temporary_lines.end());

	return number_of_errors;
}