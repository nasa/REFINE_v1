
/* Computes metrics from faces and tets 
 *
 * Michael A. Park
 * Computational Modeling & Simulation Branch
 * NASA Langley Research Center
 * Phone:(757)864-6604
 * Email:m.a.park@larc.nasa.gov 
 */
  
/* $Id$ */

#ifndef GRIDMPI_H

#include "master_header.h"
#include "queue.h"
#include "grid.h"

BEGIN_C_DECLORATION

Grid *gridIdentityNodeGlobal(Grid *g, int offset );
Grid *gridIdentityCellGlobal(Grid *g, int offset );
Grid *gridSetAllLocal(Grid *g );
Grid *gridSetGhost(Grid *g, int node );
int gridParallelEdgeSplit(Grid *g, Queue *q, int node0, int node1 );
Grid *gridParallelEdgeSwap(Grid *g, Queue *q, int node0, int node1 );
Grid *gridApplyQueue(Grid *g, Queue *q );

Grid *gridParallelAdaptWithOutCAD(Grid *g, Queue *q, 
				  double minLength, double maxLength );
Grid *gridParallelSwap(Grid *grid, Queue *queue, double ARlimit );

END_C_DECLORATION

#endif /* GRIDMPI_H */
