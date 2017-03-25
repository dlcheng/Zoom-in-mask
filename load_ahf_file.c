#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "allvars.h"
#include "proto.h"

void detect_and_link_ahf_file()
{
  FILE *fp;
  int i=0;
  char file_name[500];

  sprintf(file_name, "%s%s%s%.3f%s",
          AHF_INPUT_PATH, AHF_FILE_BASE, ".z", REDSHIFT, ".AHF_particles");

  if((fp = fopen(file_name, "r")) != NULL)
    {
	   state("Found SINGLE CPU AHF files.");
	   ahf_file_num = 1;
	   fclose(fp);
	  }
  else
    {
    sprintf(file_name, "%s%s%s%.4d%s%.3f%s",
            AHF_INPUT_PATH, AHF_FILE_BASE, ".", i, ".z", REDSHIFT, ".AHF_particles");
	  if((fp = fopen(file_name, "r")) != NULL)
	    {
	     state("Found MULTI CPUs AHF files.");
	     fclose(fp);
	     do{
		     i++;
		     sprintf(file_name, "%s%s%s%.4d%s%.3f%s",
		             AHF_INPUT_PATH, AHF_FILE_BASE, ".", i, ".z", REDSHIFT, ".AHF_particles");
		     if((fp = fopen(file_name, "r")) != NULL)
		        fclose(fp);
		     else
		        break;
		   }while(1 == 1);

	   ahf_file_num = i;
	  }
	 else
     end_run("OPENING AHF FILES");
    }

  alloc_ahf_file_arrays();

  if(ahf_file_num == 1)
    {
    sprintf(file_name, "%s%s%s%.3f%s",
            AHF_INPUT_PATH, AHF_FILE_BASE, ".z", REDSHIFT, ".AHF_halos");
    if((fp_ahf_halo[0] = fopen(file_name, "r")) != NULL)
      state("AHF_halos file opened.");
    sprintf(file_name, "%s%s%s%.3f%s",
            AHF_INPUT_PATH, AHF_FILE_BASE, ".z", REDSHIFT, ".AHF_particles");
    if((fp_ahf_part[0] = fopen(file_name, "r")) != NULL)
      state("AHF_particles file opened.");
    sprintf(file_name, "%s%s%s%.3f%s", 
            AHF_INPUT_PATH, AHF_FILE_BASE, ".z", REDSHIFT, ".AHF_substructure");
    if((fp_ahf_subhalo[0] = fopen(file_name, "r")) != NULL)
      state("AHF_substructure file opened.");          
	}
  else
    {
	  for(i=0; i<ahf_file_num; i++)
	    {
      sprintf(file_name, "%s%s%s%.4d%s%.3f%s",
              AHF_INPUT_PATH, AHF_FILE_BASE, ".", i, ".z", REDSHIFT, ".AHF_halos");
      if((fp_ahf_halo[i] = fopen(file_name, "r")) != NULL)
        printf("AHF_halos file %d opened.\n", i);
      sprintf(file_name, "%s%s%s%.4d%s%.3f%s",
              AHF_INPUT_PATH, AHF_FILE_BASE, ".", i, ".z", REDSHIFT, ".AHF_particles");
      if((fp_ahf_part[i] = fopen(file_name, "r")) != NULL)
        printf("AHF_particles file %d opened.\n", i);
	   }
	 }

  for(i=0; i<ahf_file_num; i++)
    {
    effective_rewind(&fp_ahf_halo[i]);          /* rewind but ommit the comment line */
    effective_rewind(&fp_ahf_part[i]);
    if(ahf_file_num == 1)                       /* AHF-1.0-049 only output subhaloes when using single core */
      effective_rewind(&fp_ahf_subhalo[i]);    
    }

  total_halo_num = 0;
/* making p_file_info array useful */
  for(i=0; i<ahf_file_num; i++)
    {
	   p_file_info[i].file_tag = i;
	   fscanf(fp_ahf_part[i], "%d\n", &p_file_info[i].halo_num_in_file);
	   printf("AHF file %d, halo number %d.\n", i, p_file_info[i].halo_num_in_file);
	   total_halo_num += p_file_info[i].halo_num_in_file;
    }
}                              /* end detect_ahf_file */


void effective_rewind(FILE **p)
{
 char a_temp;

 rewind(*p);
 a_temp = fgetc(*p);
 if(a_temp == 35)
   do{
     a_temp = fgetc(*p);
     }while(a_temp != 10);     /* read the useless first line if any*/
 else
   rewind(*p);
 
}	                            /* end effective_rewind */
