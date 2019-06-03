struct List {
	struct List* next;
	int value;
};

int list_length (struct List* p) {
	if (p)
		return list_length (p->next) + 1;
	else
		return 0;
}

struct List init_struct (struct List* next, int value) {
	struct List dst;
	dst.next = next;
	dst.value = value;
	return dst;
}

int check_main () {
	struct List a0 = init_struct (0, 5);
	struct List a1 = init_struct (&a0, 3);
	struct List a2 = init_struct (&a1, 2);
	struct List a3 = init_struct (&a2, 1);
	struct List a4 = init_struct (&a3, 1);

	return list_length (&a4);
}

#ifdef NATIVE
#include <stdio.h>

int main () {
	printf ("List length = %d\n", check_main ());
	return 0;
}

#endif