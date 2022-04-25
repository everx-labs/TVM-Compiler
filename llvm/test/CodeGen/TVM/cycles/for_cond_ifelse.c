int func(int arg){
   int res = 1;
   for(int iterator = arg; iterator > 0; iterator--) {
      if(iterator%11 == 0) {
         res*=iterator;
      } else {
         res+=iterator;
      }
   }
   return res;
}