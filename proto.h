#ifndef ALLVAR_H
 #include "allvars.h"
#endif

void detect_and_link_gadget_file();
void load_gedget_part();
void transfer_to_peculiar_vel(float *vel);
void check_sorted_part_array();
void sort_gadget_part(long int l, long int r);
void swap_gadget_part(PPT_PART_INFO a , PPT_PART_INFO b);
void copy_float_array(float *a, float *b, int n);
void check_gadget_file(char *file_name);

void detect_and_link_ahf_file();
void effective_rewind(FILE **p);

void init_halo_array();
void construct_ahf_halo_info(unsigned i, unsigned j);
void construct_ahf_halo_part(unsigned i, unsigned j);
void construct_ahf_subhalo();
void init_halo_trust_flag(unsigned j);

void determine_cut_box(unsigned j);
void pos_to_center(float *pos, double *center, double *new_pos);
void move_to_box(double *pos);

void init_all();

void alloc_gadget_file_array();
void alloc_ahf_file_arrays();
void alloc_halo_array();
void alloc_part_array();
void alloc_halo_part_array(unsigned j);
void alloc_halo_subhalo_array(unsigned host_id, unsigned sub_num);
void free_all_arrays();

void out_put();

void close_all_files();

void state(char *s);
void end_run(char *s);
