int func(int n) {
   int res=1;
   int iter=n;
   while(iter>0){
      if(iter%11 == 0) {
         return res;
      } else {
         if(iter%7 == 0) {
            break;
         } else {
            if(iter%5 == 0) {
               continue;
            } else {
               if(iter%3 == 0) {
                  goto ERR;
               }
            }
         }

      }
      res*=iter;
      iter--;
   }
   return res;
ERR: 
   return -1;
}