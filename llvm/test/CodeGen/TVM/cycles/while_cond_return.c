int func(int arg){
   int res = 1;
   int iterator = arg;
   while(iterator>0)
   {
      res*=iterator;
      if(iterator%11 == 0) {
         return res;
      } else {
         iterator--;
      }
   }
   return res;
}