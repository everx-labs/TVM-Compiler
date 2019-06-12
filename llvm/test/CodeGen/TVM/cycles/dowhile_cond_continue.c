int func(int arg){
   int res,iterator;
   res = iterator = arg;
   do 
   {
      iterator--;
      if(iterator%11 == 0) {
         iterator/= 11;
         continue;
      }
      res*= iterator;
   } while(iterator > 0);
   return res;
}