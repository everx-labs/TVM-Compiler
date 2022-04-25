int func(int arg){
   int res = 1;
   for(int iterator = arg; iterator > 0; iterator--) {
      if(iterator%11 == 0) {
         iterator/= 11;
         continue;
      }
      res*= iterator;
   }
   return res;
}