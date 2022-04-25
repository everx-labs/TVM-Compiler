int func(int arg1, int arg2) {
   int res=1;
   for(int iter = arg1; iter > 0; iter--) {
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
                  do {
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
                  } while(iter1 > 0);
               }
            }
         }

      }
      res*=iter;
   }
   return res;
ERR: 
   return -1;
}