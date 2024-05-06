// Author: Daltro Oliveira Vinuto
// Mat: 160025966

#include <stdio.h>
#include <iostream>
#include <string>
#include "support.h"
#include "simulator.h"


int main(int argc, char **argv) {
	std::string object_file;

	bool success;

	success = read_main_arguments(argc, argv, object_file);

	if (success) {
		success = simulator(object_file);

		if (success) {
			printf("O programa terminou sem erros!\n");
		} else {
			printf("O programa terminou com erros!\n\7");
		}
	} else {
		printf("Error! Numero de argumentos invalido!\n\7");
	}
	
}