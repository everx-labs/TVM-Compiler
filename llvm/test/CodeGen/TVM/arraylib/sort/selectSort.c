void selectSort(long* arr, long size) 
{
    long tmp, i, j, pos;
    for(i = 0; i < size; ++i) // i - номер текущего шага
    { 
        pos = i; 
        tmp = arr[i];
        for(j = i + 1; j < size; ++j) // цикл выбора наименьшего элемента
        {
            if (arr[j] < tmp) 
            {
               pos = j; 
               tmp = arr[j]; 
            }
        }
        arr[pos] = arr[i]; 
        arr[i] = tmp; // меняем местами наименьший с a[i]
    }
}