#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "ref_export.h"
#include "ref_import.h"
#include "ref_part.h"
#include "ref_grid.h"
#include "ref_validation.h"
#include "ref_histogram.h"
#include "ref_args.h"
#include "ref_adapt.h"
#include "ref_mpi.h"
#include "ref_migrate.h"
#include "ref_metric.h"

int main( int argc, char *argv[] )
{
  char *input_filename = NULL;
  char *output_filename = NULL;
  char *metric_filename = NULL;
  REF_INT location;
  REF_INT i, passes;

  REF_GRID ref_grid;
  
  RSS( ref_args_find( argc, argv, "-b", &location), "-b argument missing" );
  printf(" %s ",argv[location]);
  RAS( location<argc-1, "-b missing");
  input_filename = argv[1+location];
  printf("'%s'\n",input_filename);
  
  RSS( ref_args_find( argc, argv, "-M", &location), "-M argument missing" );
  printf(" %s ",argv[location]);
  RAS( location<argc-1, "-M missing");
  metric_filename = argv[1+location];
  printf("'%s'\n",metric_filename);
  
  RSS( ref_args_find( argc, argv, "-o", &location), "-o argument missing" );
  printf(" %s ",argv[location]);
  RAS( location<argc-1, "-o missing");
  output_filename = argv[1+location];
  printf("'%s'\n",output_filename);

  RSS( ref_import_by_extension( &ref_grid, input_filename ), "in" );
  ref_grid_inspect(ref_grid);
  RSS( ref_part_bamg_metric( ref_grid, metric_filename ), "metric" );

  RSS( ref_metric_inspect( ref_grid_node(ref_grid) ), "insp");

  RSS( ref_histogram_quality( ref_grid ), "gram");
  RSS( ref_histogram_ratio( ref_grid ), "gram");

  passes = 20;
  for (i = 0; i<passes; i++ )
    {
      printf(" pass %d of %d\n",i,passes);
      RSS( ref_adapt_pass( ref_grid ), "pass");
      ref_mpi_stopwatch_stop("pass");
      RSS(ref_validation_cell_volume(ref_grid),"vol");
      RSS( ref_histogram_quality( ref_grid ), "gram");
      RSS( ref_histogram_ratio( ref_grid ), "gram");
      RSS(ref_migrate_to_balance(ref_grid),"balance");
      ref_mpi_stopwatch_stop("balance");
    }
  
  RSS( ref_export_by_extension( ref_grid, output_filename ), "out" );

  return 0;
}
