void gnome_sort(long A[], long N) {
    for (long i = 0; i + 1 < N; ++i) {
        if (A[i] > A[i + 1]) {
            long tmp = A[i];
            A[i] = A[i + 1];
            A[i + 1] = tmp;
            if (i != 0)
                i -= 2; //вычитается два и потом прибавляется один
        }
    }
}