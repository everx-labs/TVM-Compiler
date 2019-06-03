typedef long long i64;

struct Pair {
	i64 left;
	i64 right;
};

struct Pair mk_pair () {
	struct Pair dst;
	dst.left = 11;
	dst.right = 17;
	return dst;
}

void swap_pair (struct Pair* p) {
	long long tmp = p->left;
	p->left = p->right;
	p->right = tmp;
}

long long test_leftright_1 () {
	struct Pair x = mk_pair();
	swap_pair (&x);
	return x.left > x.right;
}