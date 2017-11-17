
/* Copyright 2014 United States Government as represented by the
 * Administrator of the National Aeronautics and Space
 * Administration. No copyright is claimed in the United States under
 * Title 17, U.S. Code.  All Other Rights Reserved.
 *
 * The refine platform is licensed under the Apache License, Version
 * 2.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ref_mpi.h"

#include "ref_malloc.h"

int main( int argc, char *argv[] )
{
  REF_MPI ref_mpi;
  RSS( ref_mpi_start( argc, argv ), "start" );
  RSS( ref_mpi_create( &ref_mpi ), "make mpi" );

  if ( !ref_mpi_para(ref_mpi) )
    { /* start */
      REIS( 1, ref_mpi_m(ref_mpi), "n" );
      REIS( 0, ref_mpi_rank(ref_mpi), "rank" );
      RAS( ref_mpi_once(ref_mpi), "master" );
    }
  else
    {
      REF_INT part;
      REF_INT *a_size, *b_size;
      REF_INT bc;

      if ( ref_mpi_once(ref_mpi) )
	printf("number of processors %d \n",ref_mpi_m(ref_mpi));

      bc = REF_EMPTY;
      if ( ref_mpi_once(ref_mpi) ) bc = 5;
      RSS( ref_mpi_stopwatch_start( ref_mpi ), "sw start");
      RSS( ref_mpi_bcast( ref_mpi, &bc, 1, REF_INT_TYPE ), "bcast" );
      RSS( ref_mpi_stopwatch_stop( ref_mpi, "integer broadcast" ), "sw start");
      REIS( 5, bc, "bc wrong" );

      ref_malloc_init( a_size, ref_mpi_m(ref_mpi), REF_INT, REF_EMPTY );
      ref_malloc_init( b_size, ref_mpi_m(ref_mpi), REF_INT, REF_EMPTY );

      each_ref_mpi_part(ref_mpi, part)
	a_size[part] = part;

      RSS( ref_mpi_stopwatch_start( ref_mpi ), "sw start");
      RSS( ref_mpi_alltoall( ref_mpi,
			     a_size, b_size, REF_INT_TYPE ), "alltoall sizes");
      RSS( ref_mpi_stopwatch_stop( ref_mpi, "integer alltoall" ), "sw start");

      each_ref_mpi_part(ref_mpi, part)
	REIS(part, a_size[part], "a_size changed" );
      each_ref_mpi_part(ref_mpi, part)
	REIS(ref_mpi_rank(ref_mpi), b_size[part], "b_size wrong" );

      ref_free( b_size );
      ref_free( a_size );
    }

  RSS( ref_mpi_free( ref_mpi ), "mpi free" );
  RSS( ref_mpi_stop( ), "stop" );

  return 0;
}
