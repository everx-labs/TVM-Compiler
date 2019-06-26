long bSearch(long *arr, long size, long key)
{
    long start = 0;
    long end = size;
    long mid = 0;
 
    while(start <= end)
    {
        mid = (start + end) / 2;
 
        if (key == arr[mid]) return mid;
 
        if (key < arr[mid])
            end = mid - 1;
        else
            start = mid + 1;
    }
 
    return -1;
}