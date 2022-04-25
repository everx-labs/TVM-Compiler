long sum(long arr[], long len) {
    long res = 0;
    for(long i = 0; i < len; i++) {
        res+=arr[i];
    }
    return(res);
}

long sumR(long arr[], long len) {
    if(len <= 0) return(0);
    return(arr[len - 1] + sumR(arr, len - 1));
}
