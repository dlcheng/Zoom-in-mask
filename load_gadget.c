#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "allvars.h"
#include "proto.h"

/* This function detects the file number of gadget input, and link them to the 'fp_gadget' file array
 * The largest simulation of default Gadget2 to do is 1024^3, limited by the total particle number in
 * int scale.
 */

void detect_and_link_gadget_file()
{
  char file_name[500];
  unsigned int block_size;
  int i;

  FILE *fp;

  sprintf(file_name, "%s%s", GADGET_INPUT_PATH, GADGET_FILE_BASE);

  if((fp = fopen(file_name, "rb")) != NULL)
    {
	  state("Found SINGLE gadget file.");
	  rewind(fp);
	  fread(&block_size, sizeof(int), 1, fp);
	  fread(&g_head, sizeof(GADGET_HEAD), 1, fp);
	  gadget_file_num = 1;
	  fclose(fp);
    }
  else
    {
	  sprintf(file_name, "%s%s%s%d", GADGET_INPUT_PATH, GADGET_FILE_BASE, ".", 0);
	  if((fp = fopen(file_name, "rb")) != NULL)
      {
	    state("Found MULTI gadget files.");
	    rewind(fp);
	    fread(&block_size, sizeof(int), 1, fp);
	    fread(&g_head, sizeof(GADGET_HEAD), 1, fp);
	    gadget_file_num = g_head.NumFiles;
	    fclose(fp);
      }
     else
       end_run("OPEN GADGET FILE");
	  }

/* Total number of dark matter particles used in the simulation */
  total_part_num = g_head.NallHW[1];
  total_part_num = total_part_num << 32;
  total_part_num += g_head.Nall[1];

  boxsize = g_head.BoxSize;                        /* boxsize in unit of kpc/h */
  alloc_gadget_file_array();                       /* allocate gadget file array */

  for(i=0; i<gadget_file_num; i++)
    {
	   if(gadget_file_num == 1)
	     {
       sprintf(file_name, "%s%s", GADGET_INPUT_PATH, GADGET_FILE_BASE);
	     fp_gadget[i] = fopen(file_name, "rb");
	     check_gadget_file(file_name);
	     rewind(fp_gadget[i]);
  	   }
  	 else
  	   {
	     sprintf(file_name, "%s%s%s%d", GADGET_INPUT_PATH, GADGET_FILE_BASE, ".", i);
	     fp_gadget[i] = fopen(file_name, "rb");
	     check_gadget_file(file_name);
	     rewind(fp_gadget[i]);
	     }
    }
}                            /* end load_gadget_head */


/* This function fills the p_part array and sort it according to its ID */
void load_gedget_part()
{
  int i;
  unsigned block_size, id, j;
  long int filled_num = 0;
  GADGET_HEAD local_head;
  float pos[3];
  float vel[3];
  float mass;

  alloc_part_array();

  for(i=0; i<gadget_file_num; i++)
    {
      fread(&block_size, sizeof(block_size), 1, fp_gadget[i]);
      fread(&local_head, sizeof(GADGET_HEAD), 1, fp_gadget[i]);
      fread(&block_size, sizeof(block_size), 1, fp_gadget[i]);
      /* Read the POS */
      fread(&block_size, sizeof(block_size), 1, fp_gadget[i]);
      for(j=0; j<local_head.Npart[1]; j++)
        {
         fread(pos, 3 * sizeof(float), 1, fp_gadget[i]);
         copy_float_array(p_part[filled_num + j].pos, pos, 3);
	      }
      fread(&block_size, sizeof(block_size), 1, fp_gadget[i]);
      /* Read the VEL */
      fread(&block_size, sizeof(block_size), 1, fp_gadget[i]);
      for(j=0; j<local_head.Npart[1]; j++)
        {
         fread(vel, 3 * sizeof(float), 1, fp_gadget[i]);
         copy_float_array(p_part[filled_num + j].vel, vel, 3);
         transfer_to_peculiar_vel(p_part[filled_num + j].vel);
	      }
      fread(&block_size, sizeof(block_size), 1, fp_gadget[i]);
      /* Read the ID */
      fread(&block_size, sizeof(block_size), 1, fp_gadget[i]);
      for(j=0; j<local_head.Npart[1]; j++)
        {
         fread(&id, sizeof(unsigned), 1, fp_gadget[i]);       /* This already assumed that the ID is int scale */
         p_part[filled_num + j].id = id;
	      }
      fread(&block_size, sizeof(block_size), 1, fp_gadget[i]);
      /* Read the MASS */
      if(local_head.Massarr[1] == 0)
        {
        fread(&block_size, sizeof(block_size), 1, fp_gadget[i]);
        for(j=0; j<local_head.Npart[1]; j++)
          {
          fread(&mass, sizeof(float), 1, fp_gadget[i]);
          p_part[filled_num + j].mass = mass * 1e10;                /* mass unit is M_sun/h here */
	        }
        fread(&block_size, sizeof(block_size), 1, fp_gadget[i]);
	      }
	    else
		    for(j=0; j<local_head.Npart[1]; j++)
          p_part[filled_num + j].mass = local_head.Massarr[1] * 1e10; /* mass unit is M_sun/h here */

      filled_num += local_head.Npart[1];
   }

  sort_gadget_part(0, total_part_num-1);
  check_sorted_part_array();

  state("Sorted the P_PART array");
}   /* end load_gadget_part */

void transfer_to_peculiar_vel(float *vel)
{
  int i;
  double a = 1.0 / (1.0 + REDSHIFT);

  for(i=0; i<3; i++)
    vel[i] = vel[i] * sqrt(a);
}                                         /* end transfer_to_peculiar_vel */

void check_sorted_part_array()
{
  unsigned i;
  unsigned temp_id;

  for(i=0; i<total_part_num; i++)
    {
#if (ID_START == 0)
     temp_id = p_part[i].id;
#endif
#if (ID_START == 1)
     temp_id = p_part[i].id - 1;
#endif

     if(i != temp_id)
       {
        printf("p_part[%u].id = %u,g_head.original_num = %lu\n", i, p_part[i].id, total_part_num);
        end_run("SORTING GADGET PARTICLES");
       }
    }
}   /* end check_sorted_part_array */

void sort_gadget_part(long int l, long int r)
{
  long int i = l;
  long int j = r;
  long int key = p_part[(i+j)/2].id;
  while(i < j)
    {
    for(;(i<r)&&(p_part[i].id < key);i++);
    for(;(j>l)&&(p_part[j].id > key);j--);
    if(i <= j)
      {
      swap_gadget_part(&p_part[i],&p_part[j]);
      i++;
      j--;
      }
    }
 if(i<r)
   sort_gadget_part(i,r);
 if(j>l)
   sort_gadget_part(l,j);
}                                                   /* end sort_gadget_part */

void swap_gadget_part(PPT_PART_INFO a , PPT_PART_INFO b)
{
 int i;
 PART_INFO temp;

 temp.id = b->id;
 for(i=0; i<3; i++)
   {
   temp.pos[i] = b->pos[i];
   temp.vel[i] = b->vel[i];
   }
 temp.mass = b->mass;                                                     /* store b */

 b->id = a->id;
 for(i=0 ; i<3; i++)
   {
   b->pos[i] = a->pos[i];
   b->vel[i] = a->vel[i];
   }
 b->mass = a->mass;                                                       /* b = a*/

 a->id = temp.id;
 for(i=0 ; i<3; i++)
   {
   a->pos[i] = temp.pos[i];
   a->vel[i] = temp.vel[i];
   }
 a->mass = temp.mass;                                                     /* a = temp */
}                  /* end swap_gadget_part */

/* copy a from b */
void copy_float_array(float *a, float *b, int n)
{
  int i;

  for(i=0; i<n; i++)
     a[i] = b[i];
}                    /* end copy_float_array */

/* This function is used to check the consistence of the gadget file */

void check_gadget_file(char *file_name)
{
  FILE *fp;
  fp = fopen(file_name, "rb");

  GADGET_HEAD H;
  unsigned temp1, temp2;  /* used to read the block 4-byte-head */
  unsigned cali;          /* block value include overflow */
  long int disp;          /* displacement to move the file point */

/* HEAD block check */
#ifdef VERBOSE
  printf("Begin to check the file %s\n", file_name);
  printf("Checking the HEAD ...\n");
#endif
  fread(&temp1, sizeof(temp1), 1, fp);
  fread(&H, 256, 1, fp);
  fread(&temp2, sizeof(temp2), 1, fp);
  if(temp2!=temp1)
    end_run("Eorror detected in the checking HEAD block.");

/* POS block check */
#ifdef VERBOSE
  printf("Checking the POS ...\n");
#endif
  cali = 3 * sizeof(float) * H.Npart[1];
  disp = 3 * sizeof(float) * H.Npart[1];  /* disp != clai */
  fread(&temp1, sizeof(temp1), 1, fp);
  if(temp1 != cali)
	  end_run("Error detected in the checking POS block.");
  fseek(fp, disp, 1);
  fread(&temp2, sizeof(temp2), 1, fp);
  if(temp1 != temp2)
	  end_run("Error detected in the checking POS block.");

/* VEL block check */
#ifdef VERBOSE
  printf("Checking the VEL structure...\n");
#endif
  cali = 3 * sizeof(float) * H.Npart[1];
  disp = 3 * sizeof(float) * H.Npart[1];  /* disp != clai */
  fread(&temp1, sizeof(temp1), 1, fp);
  if(temp1 != cali)
    end_run("Error detected in the checking VEL block.");
  fseek(fp, disp, 1);
  fread(&temp2, sizeof(temp2), 1, fp);
  if(temp1 != temp2)
    end_run("Error detected in the checking VEL block.");

/* ID block check */
#ifdef VERBOSE
  printf("Checking the ID structure...\n");
#endif
  cali = sizeof(int) * H.Npart[1];
  disp = sizeof(float) * H.Npart[1];  /* disp != clai */
  fread(&temp1, sizeof(temp1), 1, fp);
  if(temp1 != cali)
	  end_run("Error detected in the checking ID block.");
  fseek(fp, disp, 1);
  fread(&temp2, sizeof(temp2), 1, fp);
  if(temp1 != temp2)
    end_run("Error detected in the checking ID block.");

/* Mass block check, if any */
  if(H.Massarr[1] == 0.0)
    {
#ifdef VERBOSE
    printf("Checking the MASS structure...\n");
#endif
    cali = sizeof(float) * H.Npart[1];
    disp = sizeof(float) * H.Npart[1];  /* disp != clai */
    fread(&temp1, sizeof(temp1), 1, fp);
    if(temp1 != cali)
      end_run("Error detected in the checking MASS block.");
    fseek(fp, disp, 1);
    fread(&temp2, sizeof(temp2), 1, fp);
    if(temp1 != temp2)
      end_run("Error detected in the checking MASS block.");
    }

/* Done with the check */
#ifdef VERBOSE
  printf("Gadget file check done, the file is consistent.\n");
#endif
}                            /* end check_gadget_file */
