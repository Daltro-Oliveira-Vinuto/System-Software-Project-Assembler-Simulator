#all_valgrind: clear compile_and_assemble_debug link load_on_valgrind
all_debug: clear compile_and_assemble_debug link
all_release: clear compile_and_assemble_release link clear_objs load
all_c: clear compile_c assemble_c link_c load_c 

clear_all:
	rm -f -r *.o bin/*.exe *.txt *.s vgcore* ./bin/montador

clear_objs:
	rm -f -r *.o 


# CPP compilation =======================================
SRC := src/*.cpp 
ELF := bin/montador
VALGRIND_TEXT := valgrind.txt
#MAIN_ARGUMENTS := assets/convert_to_binary.asm
MAIN_ARGUMENTS := assets/fatorial_macro.asm

G++ := g++
INCLUDE_PATHS := -I./include
LIBRARY_PATHS := -L
LINK_FLAGS := -lm 
COMPILATION_FLAGS_DEBUG := -Wall -Wextra -std=c++23 -g3 -O0 
COMPILATION_FLAGS_RELEASE := -Wall -Wextra -std=c++23 -g0 -O3
VALGRIND_FLAGS := --leak-check=full  --track-origins=yes --show-leak-kinds=all # --log-file=$(VALGRIND_TEXT)

compile_and_assemble_debug:
	$(G++) $(SRC) $(INCLUDE_PATHS) $(LIBRARY_PATHS)  $(COMPILATION_FLAGS_DEBUG) -c

compile_and_assemble_release:
	$(G++) $(SRC) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILATION_FLAGS_RELEASE) -c

link:
	$(G++) *.o  $(LINK_FLAGS) -o $(ELF)

load:
	./$(ELF) $(MAIN_ARGUMENTS)

load_on_valgrind:
	valgrind $(VALGRIND_FLAGS) ./$(ELF) $(MAIN_ARGUMENTS)
	#subl $(VALGRIND_TEXT) # needs sublime

# type 'lay next' and enter
debug:
	gdb  -ex 'set disassembly-flavor intel' -ex 'b main' --args $(ELF) $(MAIN_ARGUMENTS) 
# CPP compilation =========================================


# C compilation ========================================
SRC_C := src/main.c

ELF_C := bin/main

clear:
	clear 

compile_c:
	gcc  $(SRC_C) -S -Wall -Wextra -masm=intel -m32 -g3 -O0

assemble_c:
	gcc *.s -c -masm=intel -m32 

link_c:
	gcc *.o -m32 -o $(ELF_C)

load_c:
	./$(ELF_C);

# type 'lay next' and enter
debug_c:
	gdb  -ex 'set disassembly-flavor intel' -ex 'b main' --args $(ELF_C) argument1
# C compilation =========================================