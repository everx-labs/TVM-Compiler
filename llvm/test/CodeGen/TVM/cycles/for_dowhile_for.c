int foo(int, int, int);
int func(int n1, int n2, int n3) {
   int res=0;;
   for(int v1=0; v1<n1; v1++) {
      int v2=0;
      do {
         for(int v3=0; v3<n3; v3++) {
            res+=foo(v1,v2,v3);
         }
         v2++;
      } while(v2<n2);
   }
   return(res);
}
