
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

#ifndef REF_GATHER_H
#define REF_GATHER_H

#include "ref_defs.h"

BEGIN_C_DECLORATION
typedef struct REF_GATHER_STRUCT REF_GATHER_STRUCT;
typedef REF_GATHER_STRUCT * REF_GATHER;
END_C_DECLORATION

#include "ref_grid.h"
#include "ref_cell.h"
#include "ref_node.h"

#include "ref_mpi.h"

BEGIN_C_DECLORATION
struct REF_GATHER_STRUCT {
  REF_BOOL recording;
  FILE *file;
  REF_DBL time;
};

REF_STATUS ref_gather_create( REF_GATHER *ref_gather );
REF_STATUS ref_gather_free( REF_GATHER ref_gather );

#define ref_gather_blocking_frame( ref_grid, zone_title )		\
  RSS( ref_gather_tec_movie_frame( ref_grid, zone_title ), "movie frame" )

REF_STATUS ref_gather_plot( REF_GRID ref_grid, const char *filename );

REF_STATUS ref_gather_tec_movie_record_button( REF_GATHER ref_gather,
					       REF_BOOL on_or_off );
REF_STATUS ref_gather_tec_movie_frame( REF_GRID ref_grid,
				       const char *zone_title );

REF_STATUS ref_gather_tec_part( REF_GRID ref_grid, const char *filename );

REF_STATUS ref_gather_by_extension( REF_GRID ref_grid, const char *filename );
REF_STATUS ref_gather_meshb( REF_GRID ref_grid, const char *filename );
REF_STATUS ref_gather_b8_ugrid( REF_GRID ref_grid, const char *filename );
REF_STATUS ref_gather_metric( REF_GRID ref_grid, const char *filename );

REF_STATUS ref_gather_ncell( REF_NODE ref_node, REF_CELL ref_cell, 
			     REF_INT *ncell );
REF_STATUS ref_gather_ncell_quality( REF_NODE ref_node, REF_CELL ref_cell, 
				     REF_DBL min_quality, REF_INT *ncell );
REF_STATUS ref_gather_ngeom( REF_NODE ref_node, REF_GEOM ref_geom, 
			     REF_INT type, REF_INT *ngeom );

REF_STATUS ref_gather_node( REF_NODE ref_node,
			    REF_BOOL swap_endian, REF_BOOL has_id, FILE *file );
REF_STATUS ref_gather_node_tec_part( REF_NODE ref_node, FILE *file );
REF_STATUS ref_gather_node_metric( REF_NODE ref_node, FILE *file );
REF_STATUS ref_gather_node_metric_solb( REF_NODE ref_node, FILE *file );

REF_STATUS ref_gather_geom( REF_NODE ref_node,
			    REF_GEOM ref_geom, 
			    REF_INT type, FILE *file );
REF_STATUS ref_gather_cell( REF_NODE ref_node,
			    REF_CELL ref_cell, 
			    REF_BOOL faceid_insted_of_c2n, REF_BOOL always_id,
			    REF_BOOL swap_endian,
			    REF_BOOL select_faceid, REF_INT faceid,
			    FILE *file );
REF_STATUS ref_gather_cell_tec( REF_NODE ref_node,
				REF_CELL ref_cell,
				FILE *file );
REF_STATUS ref_gather_cell_quality_tec( REF_NODE ref_node,
					REF_CELL ref_cell, 
					REF_DBL min_quality, FILE *file );

END_C_DECLORATION

#endif /* REF_GATHER_H */
