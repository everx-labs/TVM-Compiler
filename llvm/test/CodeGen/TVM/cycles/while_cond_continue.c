int func(int arg){
   int res = 1;
   int iterator = arg;
   while(iterator > 0)
   {
      if(iterator%11 == 0) {
         iterator/= 11;
         continue;
      }
      res*= iterator;
      iterator--;
   }
   return res;
}