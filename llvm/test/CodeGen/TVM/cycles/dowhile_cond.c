int func(int arg){
   int res,iterator;
   res = iterator = arg;
   do 
   {
      iterator--;
      if(iterator%11 == 0) {
         return res;
      } else {
         if(iterator%7 == 0) {
            break;
         } else {
            if(iterator%5 == 0) {
               continue;
            } else {
               if(iterator%3 == 0) {
                  goto ERR;
               }
            }
         }

      }
      res*=iterator;
   } while(iterator>0);
   return res;
ERR: 
   return -1;
}