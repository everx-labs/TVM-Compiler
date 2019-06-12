int func(int arg){
   int res,iterator;
   res = iterator = arg;
   do 
   {
      if(iterator%11 == 0) {
         break;
      } else {
         iterator--;
      }
      res*=iterator;
   } while(iterator>0);
   return res;
}