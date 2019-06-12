int func(int arg){
   int res,iterator;
   res = iterator = arg;
   do 
   {
      iterator--;
      if(iterator%11 == 0) {
         res*=iterator;
      } else {
         res+=iterator;
      }
   } while(iterator>0);
   return res;
}