#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "allvars.h"
#include "proto.h"
#include "define.h"

/*
* The routine determines a cubic region in the N-body initial condition,
* the nested box will cover all the particles that end up
* within the later formed halo. The box is also required to be aligned to
* the simulation box. 
* Technically, the function finds the miminum and maximum values of 
* the particle coordinates in each dimension.
*/
void determine_cut_box(unsigned j)
{
  unsigned int i, id;

  double pos[3];
  double dis[3];

  double xmin =  2. * boxsize;
  double xmax = -2. * boxsize;
  double ymin =  2. * boxsize;
  double ymax = -2. * boxsize;
  double zmin =  2. * boxsize;
  double zmax = -2. * boxsize;

  for(i=0; i<p_halo[j].ahf_halo.npart; i++)
    {
    id = p_halo[j].p_ahf_part[i];
    pos_to_center(p_part[id].pos, p_halo[j].ahf_halo.pos, pos);

    if(pos[0] < xmin)
    	xmin = pos[0];
    if(pos[0] > xmax)
    	xmax = pos[0];

    if(pos[1] < ymin)
    	ymin = pos[1];
    if(pos[1] > ymax)
    	ymax = pos[1];

    if(pos[2] < zmin)
    	zmin = pos[2];
    if(pos[2] > zmax)
    	zmax = pos[2];
    }

  dis[0] = xmax - xmin;
  dis[1] = ymax - ymin;
  dis[2] = zmax - zmin;

  p_halo[j].cut_center[0] = (xmax + xmin) / 2.;
  p_halo[j].cut_center[1] = (ymax + ymin) / 2.;
  p_halo[j].cut_center[2] = (zmax + zmin) / 2.;

  move_to_box(p_halo[j].cut_center);

  for(i=0; i<3; i++)
  	p_halo[j].cut_center[i] /= boxsize;

  p_halo[j].cut_boundary = 0.;

  for(i=0; i<3; i++)
  	if(dis[i] > p_halo[j].cut_boundary)
  		p_halo[j].cut_boundary = dis[i];

  p_halo[j].cut_boundary /= boxsize;

}  /* end determine_cut_box */


/* 
*  This function gives the particle position after correcting the periodic 
*  boundary condtion to the halo center. It makes sure the new position is within 
*  the virial radius of the halo. Note that the returned particle position may
*  beyond the simulation box.
*  The principle the routine uses is that if the relative distance of any dimension 
*  is out the range [-L/2, L/2], a correctino is made either by adding or substracting the
*  box length to the coordinates.
*  Attention: the center format is double, while the particle pos is in float.
*/
void pos_to_center(float *pos, double *center, double *new_pos)
{
  int i;
  double dis;

  for(i=0; i<3; i++)
    {
     new_pos[i] = pos[i];
     dis = pos[i] - center[i];
     if(dis > boxsize/2.)
     	new_pos[i] -= boxsize;
     if(dis < -boxsize/2.)
     	new_pos[i] += boxsize;
    }
}  /* end pos_to_center */

/* This function moves a position outside the box to the box
*  with the periodic boundary condition.
*/
void move_to_box(double *pos)
{
  int i;

  for(i=0; i<3; i++)
   {
   while(pos[i] >= boxsize)
   	 pos[i] -= boxsize;

   while(pos[i] < 0.)
   	 pos[i] += boxsize;
   }
}  /* end move_to_box */
