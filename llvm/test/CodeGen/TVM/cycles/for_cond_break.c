int func(int arg){
   int res = 1;
   for(int iterator = arg; iterator > 0; iterator--) {
      res*=iterator;
      if(iterator%11 == 0) {
         break;
      } else {
         iterator--;
      }
   }
   return res;
}