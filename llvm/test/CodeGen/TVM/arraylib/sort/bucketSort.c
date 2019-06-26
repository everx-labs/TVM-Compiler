void bucketSort(long array[], long n)
{   
 long i, j;   
 long count[n];  
 for(i=0; i < n; i++)
 {   
  count[i] = 0;   
 }     
 for(i=0; i < n; i++)
 {    
  (count[array[i]])++; 
 }     
 for(i=0,j=0; i < n; i++)
 {   
  for(; count[i]>0;(count[i])--) 
  {       
   array[j++] = i; 
  }  
 }   
}    