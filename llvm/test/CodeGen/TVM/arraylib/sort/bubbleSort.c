void bubbleSort(long* arr, long size)
{
    long tmp, i, j;
 
    for(i = 0; i < size - 1; ++i) // i - номер прохода
    {            
        for(j = 0; j < size - 1; ++j) // внутренний цикл прохода
        {     
            if (arr[j + 1] < arr[j]) 
            {
                tmp = arr[j + 1]; 
                arr[j + 1] = arr[j]; 
                arr[j] = tmp;
            }
        }
    }
}