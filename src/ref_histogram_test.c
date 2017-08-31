#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "ref_histogram.h"
#include "ref_mpi.h"
#include "ref_grid.h"
#include "ref_migrate.h"
#include "ref_part.h"

#include "ref_node.h"
#include "ref_cell.h"
#include "ref_sort.h"
#include "ref_adj.h"
#include "ref_list.h"
#include "ref_matrix.h"
#include "ref_math.h"

#include "ref_edge.h"
#include "ref_metric.h"

#include "ref_malloc.h"

#include "ref_adapt.h"
#include "ref_split.h"
#include "ref_collapse.h"
#include "ref_smooth.h"
#include  "ref_twod.h"

#include "ref_gather.h"

int main( int argc, char *argv[] )
{

  RSS( ref_mpi_start( argc, argv ), "start" );

  if ( 2 == argc )
    {
      REF_HISTOGRAM ref_histogram;
      FILE *file;
      REF_DBL ratio;
      
      RSS( ref_histogram_create(&ref_histogram),"create");

      file = fopen(argv[1],"r");
      if (NULL == (void *)file) printf("unable to open %s\n",argv[1]);
      RNS(file, "unable to open file" );
      while(1 == fscanf(file,"%lf",&ratio))
	{
	  RSS( ref_histogram_add( ref_histogram, ratio ), "add");
	}
      fclose(file);

      RSS( ref_histogram_gather( ref_histogram ), "gather");
      if ( ref_mpi_master ) RSS( ref_histogram_print( ref_histogram,
						      "edge ratio"), "print");
      if ( 1 == ref_mpi_n ) RSS( ref_histogram_export( ref_histogram,
						       "edge-ratio"), "print");

      file = fopen(argv[1],"r");
      if (NULL == (void *)file) printf("unable to open %s\n",argv[1]);
      RNS(file, "unable to open file" );
      while(1 == fscanf(file,"%lf",&ratio))
	{
	  RSS( ref_histogram_add_stat( ref_histogram, ratio ), "add");
	}
      fclose(file);

      RSS( ref_histogram_gather_stat( ref_histogram ), "gather");
      if ( ref_mpi_master ) 
	RSS( ref_histogram_print_stat( ref_histogram), "pr stat");
      
      RSS( ref_histogram_free(ref_histogram), "free gram" );

      return 0;
    }

  /*

#!/usr/bin/env octave

nbin = 18

for i=0:nbin
  bin=i-9;
  ex=1.5/9*bin;
  printf("%2d : %3d : %f : 2^%f\n",i,bin,2^ex,ex)
end

   */

  {
    REF_HISTOGRAM ref_histogram;
    REIS(REF_NULL, ref_histogram_free(NULL),"dont free NULL");
    RSS(ref_histogram_create(&ref_histogram),"create");
    RSS(ref_histogram_free(ref_histogram),"free");
  }

  { /* min max */
    REF_HISTOGRAM ref_histogram;
    REF_DBL tol = -1.0;
    RSS(ref_histogram_create(&ref_histogram),"create");
    RSS(ref_histogram_add(ref_histogram, 0.5),"add 0.5");
    RSS(ref_histogram_add(ref_histogram, 2.0),"add 2.0");
    RSS(ref_histogram_gather(ref_histogram),"gather");
    if ( ref_mpi_master )
      {
	RWDS(0.5,ref_histogram_min(ref_histogram),tol,"tot");
	RWDS(2.0,ref_histogram_max(ref_histogram),tol,"tot");
      }
    RSS(ref_histogram_free(ref_histogram),"free");
  }

  { /* total init */
    REF_HISTOGRAM ref_histogram;
    REF_DBL tol = -1.0;
    RSS(ref_histogram_create(&ref_histogram),"create");
    RWDS(0.0,ref_histogram_log_total(ref_histogram),tol,"tot");
    RSS(ref_histogram_free(ref_histogram),"free");
  }

  { /* total zero */
    REF_HISTOGRAM ref_histogram;
    REF_DBL tol = -1.0;
    RSS(ref_histogram_create(&ref_histogram),"create");
    RSS(ref_histogram_add(ref_histogram, 0.5),"add 0.5");
    RSS(ref_histogram_add(ref_histogram, 2.0),"add 2.0");
    RWDS(0.0,ref_histogram_log_total(ref_histogram),tol,"tot");
    RSS(ref_histogram_free(ref_histogram),"free");
  }

  { /* total three */
    REF_HISTOGRAM ref_histogram;
    REF_DBL tol = -1.0;
    RSS(ref_histogram_create(&ref_histogram),"create");
    RSS(ref_histogram_add(ref_histogram, 4.0),"add 4.0");
    RSS(ref_histogram_add(ref_histogram, 2.0),"add 2.0");
    RWDS(3.0,ref_histogram_log_total(ref_histogram),tol,"tot");
    RSS(ref_histogram_free(ref_histogram),"free");
  }

  { /* mean init */
    REF_HISTOGRAM ref_histogram;
    REF_DBL tol = -1.0;
    RSS(ref_histogram_create(&ref_histogram),"create");
    RWDS(0.0,ref_histogram_log_mean(ref_histogram),tol,"mean");
    RSS(ref_histogram_free(ref_histogram),"free");
  }

  { /* mean two three */
    REF_HISTOGRAM ref_histogram;
    REF_DBL tol = -1.0;
    RSS(ref_histogram_create(&ref_histogram),"create");
    RSS(ref_histogram_add(ref_histogram, 4.0),"add 4.0");
    RSS(ref_histogram_add(ref_histogram, 2.0),"add 2.0");
    RSS(ref_histogram_gather(ref_histogram),"gather");
    RWDS(1.5,ref_histogram_log_mean(ref_histogram),tol,"mean");
    RSS(ref_histogram_free(ref_histogram),"free");
  }

  { /* stat two three */
    REF_HISTOGRAM ref_histogram;
    REF_DBL tol = -1.0;
    RSS(ref_histogram_create(&ref_histogram),"create");
    RSS(ref_histogram_add(ref_histogram, 4.0),"add 4.0");
    RSS(ref_histogram_add(ref_histogram, 2.0),"add 2.0");
    RSS(ref_histogram_gather(ref_histogram),"gather");
    RSS(ref_histogram_add_stat(ref_histogram, 4.0),"add 4.0");
    RSS(ref_histogram_add_stat(ref_histogram, 2.0),"add 2.0");
    RSS(ref_histogram_gather_stat(ref_histogram),"gather");
    RWDS(1.0,   ref_histogram_stat(ref_histogram,0),tol,"stat 0");
    RWDS(0.0,   ref_histogram_stat(ref_histogram,1),tol,"stat 1");
    RWDS(0.25,  ref_histogram_stat(ref_histogram,2),tol,"stat 2");
    RWDS(0.00,  ref_histogram_stat(ref_histogram,3),tol,"stat 3");
    RWDS(0.0625,ref_histogram_stat(ref_histogram,4),tol,"stat 4");
    RSS(ref_histogram_free(ref_histogram),"free");
  }

  { /* smaller than 1 */
    REF_HISTOGRAM ref_histogram;
    REF_DBL obs;
    REF_INT bin;
    RSS(ref_histogram_create(&ref_histogram),"create");
    obs = 0.99999;
    bin = ref_histogram_to_bin(obs);
    REIS(8,bin,"wrong bin");
    RAS(obs>=ref_histogram_to_obs(bin),  "lower bound");
    RAS(obs<=ref_histogram_to_obs(bin+1),"upper bound");
    RSS(ref_histogram_free(ref_histogram),"free");
  }
  { /* larger than 1 */
    REF_HISTOGRAM ref_histogram;
    REF_DBL obs;
    REF_INT bin;
    RSS(ref_histogram_create(&ref_histogram),"create");
    obs = 1.00001;
    bin = ref_histogram_to_bin(obs);
    REIS(9,bin,"wrong bin");
    RAS(obs>=ref_histogram_to_obs(bin),  "lower bound");
    RAS(obs<=ref_histogram_to_obs(bin+1),"upper bound");
    RSS(ref_histogram_free(ref_histogram),"free");
  }

  { /* first box */
    REF_HISTOGRAM ref_histogram;
    REF_DBL obs;
    REF_INT bin;
    RSS(ref_histogram_create(&ref_histogram),"create");
    obs = 1e-10;
    bin = ref_histogram_to_bin(obs);
    REIS(0,bin,"wrong bin");
    RAS(obs<=ref_histogram_to_obs(bin+1),"upper bound");
   RSS(ref_histogram_free(ref_histogram),"free");
  }
  
  { /* last box */
    REF_HISTOGRAM ref_histogram;
    REF_DBL obs;
    REF_INT bin;
    RSS(ref_histogram_create(&ref_histogram),"create");
    obs = 1e10;
    bin = ref_histogram_to_bin(obs);
    REIS(ref_histogram_nbin(ref_histogram)-1,bin,"wrong bin");
    RAS(obs>ref_histogram_to_obs(bin),"upper bound");
    RSS(ref_histogram_free(ref_histogram),"free");
  }

  RSS( ref_mpi_stop(  ), "stop" );
  return 0;
}