int func(int arg){
   int res = 1;
   int iterator = arg;
   while(iterator>0)
   {
      res*=iterator;
      if(iterator%11 == 0) {
         goto ERR;
      } else {
         iterator--;
      }
   }
   return res;
ERR: 
   return -1;
}