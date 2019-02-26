int func(int n) {
   int res=1;
   int iter=n;
   while(iter>0){
      res*=iter;
      iter--;
   }
   return res;
}