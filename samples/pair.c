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

i64 test_pair () {
	struct Pair p = mk_pair ();
	return p.left + p.right;
}
