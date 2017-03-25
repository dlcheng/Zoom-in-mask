#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "allvars.h"
#include "proto.h"

int main()
{ 
  init_all();
  out_put();
    
  close_all_files();   /* before free all arrays */  
  free_all_arrays();
	
  state("Job finished.");	
  
  return 0;	
}         /* end main */	
