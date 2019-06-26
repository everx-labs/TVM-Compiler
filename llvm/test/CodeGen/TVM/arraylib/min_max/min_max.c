struct item
{
    long value;
    long position;
};

struct result {
    struct item min;
    struct item max;
} Result;

struct result* min_max(long arr[], long len)
{
    if(len <= 0)
        return(0);
    Result.min.value = arr[0];
    Result.min.position = 0;
    Result.max.value = arr[0];
    Result.max.position = 0;
    
    for(long i=0; i < len; i++) {
        if (arr[i] > Result.max.value) {
            Result.max.value = arr[i];
            Result.max.position = i;
        }
        if (arr[i] < Result.min.value) {
            Result.min.value = arr[i];
            Result.min.position = i;
        }
    }

    return(&Result);
}