enum { sz = 9, rand_seed = 5 };

void fill_array (int a[], int i, int prev) {
	a [i] = (i > 5 ? i : 9-i);
	if (i > 0)
		fill_array (a, i-1, a[i]);
}

int fetch_value (int a[], int pos) {
	int v = a [pos];
	a [pos] = 1<<30;
	return v;
}

int select_min (int a[], int i, int mx_pos) {
	if (a [i] < a [mx_pos])
		mx_pos = i;

	if (i == 0)
		return fetch_value (a, mx_pos);
	else
		return select_min (a, i-1, mx_pos);
}

void select_sort_array (int a[], int b[], int idx) {
	if (idx == sz)
		return;

	b [idx] = select_min (a, sz-1, 0);
	select_sort_array (a, b, idx+1);
}

int array_to_int (int b[], int idx, int acc) {
	if (idx == sz)
		return acc;

	acc = acc * 10 + b [idx];
	return array_to_int (b, idx+1, acc);
}

#ifdef NATIVE
#include <stdio.h>
void print_array (const char* p, int b[]) {
	printf ("%s", p);
	for (int i = 0; i < sz; i++)
		printf ("%d ", b[i]);
	printf ("\n");
}
#endif

int select_sort () {
	int a [sz];
	int b [sz];
	fill_array (a, sz-1, rand_seed);
#ifdef NATIVE
	print_array ("Original array: ", a);
#endif

	select_sort_array (a, b, 0);

#ifdef NATIVE
	print_array ("Sorted array: ", b);
#endif

	return array_to_int (b, 0, 0);
}

#ifdef NATIVE
int main () {
	printf ("\nResult = %d\n", select_sort());
}
#endif
