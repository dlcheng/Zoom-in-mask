/* Only valid for AHF version >=1.0
 * and dark matter only simulations
 * with total number of particles less than 2^32
 * Units:
 *  1 length = 1 kpc/h
 *  1 mass   = 1 M_sun/h
 *  1 velocity = 1 km/s
 */

#define AHF_INPUT_PATH               "/home/dlcheng/install/ahf-v1.0-049/Result/Uniform-planck/z-0/"
#define GADGET_INPUT_PATH            "/home/dlcheng/install/ohahn-music/ICs/"
#define AHF_FILE_BASE                "uniform_planck"
#define GADGET_FILE_BASE             "ics_gadget_planck.dat"
#define OUTPUT_PATH                  "./result/Uniform-planck/"


/* no meaning here */
#define RESOLUTED_HALO_PART_NUM      50         /* number of undecayed particle in a halo to be considered as trustworthy */
#define ENVIR_MASS_CRIT              1e8        /* halo with mass less than this value
                                                 * will be classified as enviroment, in unit of M_sun/h */
#define REDSHIFT                     0.