#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "allvars.h"
#include "proto.h"

void state(char *s)
{
  printf("%s\n", s);	
}                    /* end state */	

void end_run(char *s)
{
  printf("Error deteced in %s.\n", s);	
  exit(0);		
}                   /* end end_run */	
