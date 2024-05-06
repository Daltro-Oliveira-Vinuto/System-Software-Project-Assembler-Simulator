// Author: Daltro Oliveira Vinuto
// Mat: 160025966

#include <stdio.h>
#include <iostream>
#include <string>

#include "assembler.h"
#include "support.h"


int main(int argc, char **argv) {
	std::string assembly_file;  

	// read assembly file name
	bool success;
	success = read_main_arguments(argc, argv, assembly_file);
	if (!success) {
		printf("Error! Enter the assembly file name!\7\n");
	} else {
		// assemble the code  

		success = assemble(assembly_file);

		if (success) {
			printf("Assemble done successfully!\n");
		} else {
			printf("Error to assemble because errors were detected!\7\n");
		}
	}

}