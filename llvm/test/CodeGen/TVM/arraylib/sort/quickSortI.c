void qSortI(long a[], long size) {
 
    long i, j; // указатели, участвующие в разделении
    long lb, ub; // границы сортируемого в цикле фрагмента
 
    long lbstack[50], ubstack[50]; // стек запросов
    // каждый запрос задается парой значений,
    // а именно: левой(lbstack) и правой(ubstack)
    // границами промежутка
    long stackpos = 1; // текущая позиция стека
    long ppos; // середина массива
    long pivot; // опорный элемент
    long temp;
 
    lbstack[1] = 0;
    ubstack[1] = size-1;
 
    do {
        // Взять границы lb и ub текущего массива из стека.
        lb = lbstack[ stackpos ];
        ub = ubstack[ stackpos ];
        stackpos--;
 
        do {
            // Шаг 1. Разделение по элементу pivot
            ppos = ( lb + ub ) >> 1;
            i = lb; j = ub; pivot = a[ppos];
            do {
                while ( a[i] < pivot ) i++;
                while ( pivot < a[j] ) j--;
                if ( i <= j ) {
                    temp = a[i]; a[i] = a[j]; a[j] = temp;
                    i++; j--;
                }
            } while ( i <= j );
 
            // Сейчас указатель i указывает на начало правого подмассива,
            // j - на конец левого (см. иллюстрацию выше), lb ? j ? i ? ub.
            // Возможен случай, когда указатель i или j выходит за границу массива
 
            // Шаги 2, 3. Отправляем большую часть в стек и двигаем lb,ub
            if ( i < ppos ) { // правая часть больше
                if ( i < ub ) { // если в ней больше 1 элемента - нужно
                    stackpos++; // сортировать, запрос в стек
                    lbstack[ stackpos ] = i;
                    ubstack[ stackpos ] = ub;
                }
            ub = j; // следующая итерация разделения
            // будет работать с левой частью
            } else { // левая часть больше
                if ( j > lb ) {
                    stackpos++;
                    lbstack[ stackpos ] = lb;
                    ubstack[ stackpos ] = j;
                }
                lb = i;
            }
        } while ( lb < ub ); // пока в меньшей части более 1 элемента
    } while ( stackpos != 0 ); // пока есть запросы в стеке
}