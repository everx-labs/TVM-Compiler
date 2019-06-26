long search(long key, long arr[], long len)
{
    for(long i=0; i < len; i++)
        if (arr[i] == key)
            return(i);
    return(-1);
}