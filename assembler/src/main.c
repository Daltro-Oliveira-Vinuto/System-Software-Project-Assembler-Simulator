#include <stdio.h>

typedef struct {
	int code;
	int capacity;
	int number;
} Class;

int main() {
	const unsigned max = 10;
	Class arr_c[max];
	unsigned i;
	Class* p_c;

	for(i = 0; i < max; i++) {
		arr_c[i].code = i;
		arr_c[i].capacity = i;
		arr_c[i].number = i;
	}


	p_c = arr_c;
	for(i = 0; i < max; i++) {
		printf("code: %d\n", p_c->code);
		p_c = p_c + 1;
	}

	return 0;
}