long long A (long long m, long long n) {
	if (m == 0)
		return n+1;
	else if (n == 0)
		return A (m-1, 1);
	else
		return A (m-1, A (m, n-1));
}

long long A_22 () {
	return A (2,2);
}

long long A_24 () {
	return A (2,4);
}

long long A_33 () {
	return A (3,3);
}