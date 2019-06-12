int func(int arg1, int arg2) {
   int res=1;
   int iter=arg1;
   while(iter > 0) {
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
               } else {
                  int iter1 = arg2;
                  while(iter1 > 0) {
                     if(iter1%11 == 0) {
                        return res;
                     } else {
                        if(iter1%7 == 0) {
                           break;
                        } else {
                           if(iter1%5 == 0) {
                              continue;
                           } else {
                              if(iter1%3 == 0) {
                                 goto ERR;
                              }
                           }
                        }
                     }
                     res*=iter1;
                     iter1--;
                  }
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