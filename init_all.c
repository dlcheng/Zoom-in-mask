#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "allvars.h"
#include "proto.h"

void init_all()
{
  detect_and_link_gadget_file();	
  detect_and_link_ahf_file();
  state("Finish linking Gadget and AHF files.");
  
  state("Begin to load Gadget file to memory.");
  load_gedget_part();
    
  init_halo_array();
  state("Finish loading the halo array.");
		
}              /* end init_all */	
