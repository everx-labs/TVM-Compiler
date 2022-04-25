long bSearchR(long * arr, long start, long end, long key)
{   long mid = 0;
 
    if (start <= end)
    {
        mid = (start + end) / 2;
 
        if (key < arr[mid]) 
            return bSearchR(arr, start, mid - 1, key);
        if (key > arr[mid])
            return bSearchR(arr, mid + 1, end, key);
 
        return mid;
    }
 
    return -1;
}