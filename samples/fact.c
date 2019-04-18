// Tail recursion in this code is optimized, when
// compiled with -O1 flag.

int fact (int n) {
    if (n > 0)
        return n * fact (n-1);
    else
        return 1;
}