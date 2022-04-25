void shaker_sort(long array[], long size)
{
    for (long left_idx = 0, right_idx = size - 1;
         left_idx < right_idx;)
    {
        for (long idx = left_idx; idx < right_idx; idx++)
        {
            if (array[idx + 1] < array[idx])
            {
                long tmp = array[idx];
                array[idx] = array[idx + 1];
                array[idx + 1] = tmp;
            }
        }
        right_idx--;

        for (long idx = right_idx; idx > left_idx; idx--)
        {
            if (array[idx - 1] >  array[idx])
            {
                long tmp = array[idx - 1];
                array[idx - 1] = array[idx];
                array[idx] = tmp;
            }
        }
        left_idx++;
    }
}