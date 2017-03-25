#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "allvars.h"
#include "proto.h"


/* Do this before releasing the arrays */
void close_all_files()
{
  int i;

  for(i=0; i<gadget_file_num; i++)
    {
	   fclose(fp_gadget[i]);
     state("Fisnish closing Gadget files.");
    }

  for(i=0; i<ahf_file_num; i++)
	  {
	   fclose(fp_ahf_halo[i]);
  	 fclose(fp_ahf_part[i]);
     fclose(fp_ahf_subhalo[i]);     
    }

  state("All Gadget and AHF files closed.");
}                      /* end close_all_files */
