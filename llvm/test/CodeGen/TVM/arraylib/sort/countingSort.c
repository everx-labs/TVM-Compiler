/*
  arr -- the input array of integers to be sorted
  n -- the length of the input array
  k -- a number such that all integers are in the range 0..k-1
*/
void countSort(long arr[], long n, long k)
{
	// create an integer array of size n to store sorted array
	long output[n];

	// create an integer array of size k, initialized by all zero
	long freq[k];
    for (long i = 0; i < k; i++)
       freq[i] = 0;

	// 1. using value of integer in the input array as index,
	// store count of each integer in freq[] array
	for (long i = 0; i < n; i++)
		freq[arr[i]]++;

	// 2. calculate the starting index for each integer
	long total = 0;
	for (long i = 0; i < k; i++)
	{
		long oldCount = freq[i];
		freq[i] = total;
		total += oldCount;
	}

	// 3. copy to output array, preserving order of inputs with equal keys
	for (long i = 0; i < n; i++)
	{
		output[freq[arr[i]]] = arr[i];
		freq[arr[i]]++;
	}

	// copy the output array back to the input array
	for (long i = 0; i < n; i++)
		arr[i] = output[i];
}