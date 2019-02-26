int func(int arg){
   if(arg<1) return 1;
   int res=1;
   int iterator=arg;
   do 
   {
      res*=iterator;
      iterator--;
   } while(iterator>0);
   return res;
}