long sum_even(long arr[], long len) {
    long res = 0;
    for(long i = 0; i < len; i+=2) {
        res+=arr[i];
    }
    return(res);
}

long sum_odd(long arr[], long len) {
    long res = 0;
    for(long i = 1; i < len; i+=2) {
        res+=arr[i];
    }
    return(res);
}