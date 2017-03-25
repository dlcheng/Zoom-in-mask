#ifndef ALLVAR_H
#define ALLVAR_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "define.h"

typedef struct gadget_head GADGET_HEAD;
typedef struct gadget_head *PPT_GADGET_HEAD;
typedef struct part_info PART_INFO;
typedef struct part_info *PPT_PART_INFO;
typedef struct file_info FILE_INFO;
typedef struct file_info *PPT_FILE_INFO;
typedef struct ahf_halo_info AHF_HALO_INFO;
typedef struct ahf_halo_info *PPT_AHF_HALO_INFO;
typedef struct ahf_subhalo_info AHF_SUBHALO_INFO;
typedef struct ahf_subhalo_info *PPT_AHF_SUBHALO_INFO;
typedef struct halo_info HALO_INFO;
typedef struct halo_info *PPT_HALO_INFO;

struct gadget_head
{
  unsigned int Npart[6];
  double Massarr[6];
  double Time;
  double Redshift;
  int FlagSfr;
  int FlagFeedback;
  int Nall[6];
  int  FlagCooling;
  int NumFiles;
  double BoxSize;
  double Omega0;
  double OmegaLambda;
  double HubbleParam;
  int FlagAge;
  int FlagMetals;
  int NallHW[6];
  int Flag_entr_ics;
  char unused[60];
};

struct part_info
{
  unsigned id;                                  /* ID in int */
  float pos[3];
  float vel[3];
  float mass;
};

struct file_info
{
  unsigned file_tag;                            /* tag of the file, start from 0 */
  unsigned halo_num_in_file;        			      /* total number of halos in the AHF file */
};

struct ahf_halo_info
{
  unsigned halo_id;
  int host_flag;                                /* -1 if it is host, otherwise, ID of host halo */
  unsigned sub_num;                             /* number of subhalos inside */
  double Mvir;                                  /* mass of the halo, in unit of M_sun/h */
  unsigned npart;                               /* number of particles of the halo */
  double pos[3];                                /* position of the center, in unit of kpc/h */
  double vel[3];                                /* peculiar velocity of halo, in unit of km/s */
  double Rvir;                                  /* virial radius in comoving scale */
  double Rvmax;                                 /* position of rotation curve maximum */
  double r2;                                    /* position where %rho r^2 peaks */
  double mbp_offset;                            /* offset between most bound particle and halo center */
  double com_offset;                            /* offset between cnter-of-mass and halo center */
  double Vmax;                                  /* maximum of rotation curve */
  double v_esc;                                 /* escape velocity at Rvir */
  double sigV;                                  /* 3D velocity dispersion */
  double lambda;                                /* spin parameter (Bullock et al. 2001) */
  double lambdaE;                               /* classical spin parameter (Peebles) */
  double vec_L[3];                              /* orientation of angular momentum */
  double b_c;                                   /* b over c, shape parameter, a<b<c */
  double a_c;                                   /* a over c, shape parameter*/
  double vec_c[3];                              /* direction of a, shape parameter */
  double vec_b[3];                              /* direction of b, shape parameter */
  double vec_a[3];                              /* direction of c, shape parameter */
  double ovdens;                                /* overdensity at virial radius */
  int nbins;                                    /* number of bins used for ahf_profile_info */
  double fMhires;                               /* mass fraction in high resolution particles for zoom simulations */
  double Ekin;                                  /* kinetic energy, in uit of M_sun/h*(km/s)^2 */
  double Epot;                                  /* potential energy, same unit as Ekin */
  double SurfP;                                 /* surface pressure(Shaw et al. 2006), same unit as Ekin */
  double Phi0;                                  /* %Phi_0 used in unbinding procedure, in unit of (km/s)^2 */
  double cNFW;                                  /* NFW concentration (Prada et al. 2012) */
};

struct ahf_subhalo_info
{
  unsigned  sub_id;                             /* subhalo position in the halo array */      
};

struct halo_info
{
  int trust_flag;                                /* controlled only by particle number, good 1, bad 0 */
  unsigned int trust_sub_num;                    /* the number of trusted subhaloes, may be smaller than ahf_halo.sub_num */

  double cut_center[3];                          /* the center of cut box, in unit of box size */
  double cut_boundary;                           /* the length of cut box, in unit of box size */

  AHF_HALO_INFO ahf_halo;
/* pointers */
  unsigned int * p_ahf_part;                     /* point to particle ID */
  
  PPT_AHF_SUBHALO_INFO  p_ahf_subhalo;           /* point to the subhalo array */       
};

/* some global parameters */
extern int ahf_file_num;                         /* the number of AHF files produced by multi CPU */
extern int gadget_file_num;                      /* the file number of gadget files */
extern PPT_FILE_INFO p_file_info;                /* point to the file info array */
extern unsigned total_halo_num;                  /* total halo number in all files */
extern long int total_part_num;                  /* total particle numbe in the gadget file */
extern PPT_HALO_INFO p_halo;                     /* point to the halo array */

extern GADGET_HEAD g_head;                       /* the header gadget file */
extern double boxsize;                           /* boxsize in unit of kpc/h */
extern PPT_PART_INFO p_part;                     /* point to the array of all particles */

/* related to the files */
extern FILE **fp_ahf_halo;                       /* point to multi AHF.halos files, if any */
extern FILE **fp_ahf_part;                       /* point to multi AHF.particles files, if any */
extern FILE **fp_ahf_subhalo;                    /* point to multi AHF.substructures files, if any */
extern FILE **fp_gadget;                         /* point to multi gadget files, if any */

extern double total_halo_mass;                   /* total mass in the halos, no matter it's good or not by our criteria */
extern double total_envir_mass;                  /* total mass in the environment */

#endif
