void quickSortR(long *a, long N) {
// На входе - массив a[], a[N] - его последний элемент.
 
    long i = 0, j = N;      // поставить указатели на исходные места
    long temp, p;
 
    p = a[ N>>1 ];      // центральный элемент
 
    // процедура разделения
    do {
        while ( a[i] < p ) i++;
        while ( a[j] > p ) j--;
 
        if (i <= j) {
            temp = a[i]; a[i] = a[j]; a[j] = temp;
            i++; j--;
        }
    } while ( i<=j );
 
    // рекурсивные вызовы, если есть, что сортировать 
    if ( j > 0 ) quickSortR(a, j);
    if ( N > i ) quickSortR(a+i, N-i);
}