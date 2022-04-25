long increment(long inc[], long size) 
{
    long p1, p2, p3, s;
    p1 = p2 = p3 = 1;
    s = -1;
    do 
    {
        if (++s % 2) 
        {
            inc[s] = 8*p1 - 6*p2 + 1;
        } 
        else 
        {
            inc[s] = 9*p1 - 9*p3 + 1;
            p2 *= 2;
            p3 *= 2;
        }
    p1 *= 2;
    } 
    while(3*inc[s] < size);  
 
    return s > 0 ? --s : 0;
}
 
 
void shellSort(long a[], long size) 
{
    long inc, i, j, seq[40];
    long s;
 
    s = increment(seq, size); // вычисление последовательности приращений
    while (s >= 0)  // сортировка вставками с инкрементами inc[] 
    {
         inc = seq[s--];         
         for (i = inc; i < size; ++i) 
         {
             long temp = a[i];
             for (j = i; (j >= inc) && (temp < a[j-inc]); j -= inc) {
                a[j] = a[j - inc];
             }
             a[j] = temp;
         }
    }
}