// RUN: %clang -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry main | FileCheck %s
void bubbleSort(long* arr, long size)
{
    long tmp, i, j;

    for(i = 0; i < size - 1; ++i)
    {            
        for(j = 0; j < size - 1; ++j)
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

int isSorted(long* arr, long size) {
    for (int i = 0; i < size - 1; ++i)
        if (arr[i] > arr[i + 1])
            return 0;
    return 1;
}

int main() {
    long arr[] = {-1, 1, -2, 2, -3, 3};
    bubbleSort(arr, 6);
// CHECK-NOT: custom error
    __builtin_tvm_throwif(!isSorted(arr, 6), 13);
    return 0;
}
