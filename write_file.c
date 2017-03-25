#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "allvars.h"
#include "proto.h"

void out_put()
{
  FILE *fp_halo;
  char file_name[500];
  unsigned j;           /* run over halo number */

  sprintf(file_name, "%s%s", OUTPUT_PATH, "halo_cut_catalog.txt");
  fp_halo = fopen(file_name, "w+");
  fprintf(fp_halo, "%s\n", "#(1)HaloID (2)Npart (3)Ntrust_sub (4)Mvir (5)Rvir (6)cNFW_ahf (7)X (8)Y (9)Z (10)Cut_x (11)Cut_y (12)Cut_z (13)Cut_L");

  for(j=0; j<total_halo_num; j++)
    {
    if(p_halo[j].trust_flag == 1 && p_halo[j].ahf_halo.host_flag == -1)     /* for trusted host halos */
      {
      fprintf(fp_halo, "%u\t%d\t%u\t%.6e\t%.6e\t%.6e\t%.6e\t%.6e\t%.6e\t%.6e\t%.6e\t%.6e\t%.6e\n",
                        p_halo[j].ahf_halo.halo_id, p_halo[j].ahf_halo.npart,
                        p_halo[j].trust_sub_num,
                        p_halo[j].ahf_halo.Mvir, p_halo[j].ahf_halo.Rvir,
                        p_halo[j].ahf_halo.cNFW,
                        p_halo[j].ahf_halo.pos[0], p_halo[j].ahf_halo.pos[1], p_halo[j].ahf_halo.pos[2],
                        p_halo[j].cut_center[0], p_halo[j].cut_center[1], p_halo[j].cut_center[2],
                        p_halo[j].cut_boundary);
      }
    }

  fclose(fp_halo);
  state("Finish output.");
}            /*end out_put */
