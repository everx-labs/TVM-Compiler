void downHeap(long a[], long k, long n) 
{
    //  процедура просеивания следующего элемента 
    //  До процедуры: a[k+1]...a[n]  - пирамида 
    //  После:  a[k]...a[n]  - пирамида 
    long new_elem;
    long child;
    new_elem = a[k];
    
    while(k <= n/2) // пока у a[k] есть дети 
    {       
        child = 2*k;
        
        if( child < n && a[child] < a[child+1] ) //  выбираем большего сына 
            child++;
        if( new_elem >= a[child] ) 
            break; 
        // иначе 
        a[k] = a[child];    // переносим сына наверх 
        k = child;
    }
    a[k] = new_elem;
}
 
void heapSort(long a[], long size) 
{
    long i;
    long temp;
 
  // строим пирамиду 
    for(i = size / 2 - 1; i >= 0; --i) 
        downHeap(a, i, size-1);
  
  // теперь a[0]...a[size-1] пирамида 
 
    for(i=size-1; i > 0; --i) 
    {
        // меняем первый с последним 
        temp = a[i]; 
        a[i] = a[0]; 
        a[0] = temp;
        // восстанавливаем пирамидальность a[0]...a[i-1] 
        downHeap(a, 0, i-1); 
    }
}