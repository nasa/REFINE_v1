
#include "ruby.h"
#include "gridmetric.h"

#define GET_GRID_FROM_SELF Grid *grid; Data_Get_Struct( self, Grid, grid );

VALUE grid_edgeLength( VALUE self, VALUE n0, VALUE n1 )
{
  GET_GRID_FROM_SELF;
  return rb_float_new( gridEdgeLength( grid, NUM2INT(n0), NUM2INT(n1) ) );
}

VALUE grid_averageEdgeLength( VALUE self, VALUE node )
{
  GET_GRID_FROM_SELF;
  return rb_float_new( gridAverageEdgeLength( grid, NUM2INT(node) ) );
}

VALUE grid_spacing( VALUE self, VALUE node )
{
  GET_GRID_FROM_SELF;
  return rb_float_new( gridSpacing( grid, NUM2INT(node) ) );
}

VALUE grid_resetSpacing( VALUE self )
{
  GET_GRID_FROM_SELF;
  return (gridResetSpacing(grid)==grid?self:Qnil);
}

VALUE grid_scaleSpacing( VALUE self, VALUE node, VALUE scale )
{
  GET_GRID_FROM_SELF;
  return 
    (gridScaleSpacing(grid, NUM2INT(node), NUM2DBL(scale))==grid?self:Qnil);
}

VALUE grid_scaleSpacingSphere( VALUE self, 
			       VALUE x, VALUE y, VALUE z, VALUE r, 
			       VALUE scale )
{
  GET_GRID_FROM_SELF;
  return
    (gridScaleSpacingSphere(grid, 
			    NUM2DBL(x), NUM2DBL(y), NUM2DBL(z), NUM2DBL(r), 
			    NUM2DBL(scale))==grid?self:Qnil);
}

VALUE grid_volume( VALUE self, VALUE rb_nodes )
{
  int i, nodes[4];
  GET_GRID_FROM_SELF;
  for ( i=0 ; i<4 ; i++ ) nodes[i] = NUM2INT(rb_ary_entry(rb_nodes,i));
  return rb_float_new( gridVolume( grid, nodes ) );
}

VALUE grid_ar( VALUE self, VALUE rb_nodes )
{
  int i, nodes[4];
  GET_GRID_FROM_SELF;
  for ( i=0 ; i<4 ; i++ ) nodes[i] = NUM2INT(rb_ary_entry(rb_nodes,i));
  return rb_float_new( gridAR( grid, nodes ) );
}

VALUE grid_nodeAR( VALUE self, VALUE node )
{
  double ar;
  Grid *returnedGrid;
  GET_GRID_FROM_SELF;
  return (gridNodeAR( grid, NUM2INT(node), &ar )==grid?rb_float_new(ar):Qnil);
}

VALUE grid_cellARDerivative( VALUE self, VALUE rb_nodes )
{
  int i, nodes[4];
  double ar, dARdx[3];
  VALUE rb_ar;
  Grid *returnedGrid;
  GET_GRID_FROM_SELF;
  for ( i=0 ; i<4 ; i++ ) nodes[i] = NUM2INT(rb_ary_entry(rb_nodes,i));
  returnedGrid = gridCellARDerivative( grid, nodes, &ar, dARdx );
  if ( returnedGrid == grid ){
    rb_ar = rb_ary_new2(4);
    rb_ary_store( rb_ar, 0, rb_float_new(ar) );
    rb_ary_store( rb_ar, 1, rb_float_new(dARdx[0]) );
    rb_ary_store( rb_ar, 2, rb_float_new(dARdx[1]) );
    rb_ary_store( rb_ar, 3, rb_float_new(dARdx[2]) );
  }else{
    rb_ar = Qnil;
  }
  return rb_ar;
}

VALUE grid_nodeARDerivative( VALUE self, VALUE node )
{
  double ar, dARdx[3];
  VALUE rb_ar;
  Grid *returnedGrid;
  GET_GRID_FROM_SELF;
  returnedGrid = gridNodeARDerivative( grid, NUM2INT(node), &ar, dARdx );
  if ( returnedGrid == grid ){
    rb_ar = rb_ary_new2(4);
    rb_ary_store( rb_ar, 0, rb_float_new(ar) );
    rb_ary_store( rb_ar, 1, rb_float_new(dARdx[0]) );
    rb_ary_store( rb_ar, 2, rb_float_new(dARdx[1]) );
    rb_ary_store( rb_ar, 3, rb_float_new(dARdx[2]) );
  }else{
    rb_ar = Qnil;
  }
  return rb_ar;
}

VALUE grid_minVolume( VALUE self )
{
  GET_GRID_FROM_SELF;
  return rb_float_new( gridMinVolume( grid ) );
}

VALUE grid_minAR( VALUE self )
{
  GET_GRID_FROM_SELF;
  return rb_float_new( gridMinAR( grid ) );
}

VALUE grid_rightHandedFace( VALUE self, VALUE face )
{
  GET_GRID_FROM_SELF;
  return (gridRightHandedFace(grid, NUM2INT(face))?Qtrue:Qfalse);
}

VALUE grid_rightHandedBoundary( VALUE self )
{
  GET_GRID_FROM_SELF;
  return (gridRightHandedBoundary(grid)?Qtrue:Qfalse);
}

VALUE cGridMetric;

void Init_GridMetric() 
{
  cGridMetric = rb_define_module( "GridMetric" );
  rb_define_method( cGridMetric, "edgeLength", grid_edgeLength, 2 );
  rb_define_method( cGridMetric, "averageEdgeLength", grid_averageEdgeLength, 1 );
  rb_define_method( cGridMetric, "spacing", grid_spacing, 1 );
  rb_define_method( cGridMetric, "resetSpacing", grid_resetSpacing, 0 );
  rb_define_method( cGridMetric, "scaleSpacing", grid_scaleSpacing, 2 );
  rb_define_method( cGridMetric, "scaleSpacingSphere", grid_scaleSpacingSphere, 5 );
  rb_define_method( cGridMetric, "volume", grid_volume, 1 );
  rb_define_method( cGridMetric, "ar", grid_ar, 1 );
  rb_define_method( cGridMetric, "nodeAR", grid_nodeAR, 1 );
  rb_define_method( cGridMetric, "cellARDerivative", grid_cellARDerivative, 1 );  rb_define_method( cGridMetric, "nodeARDerivative", grid_nodeARDerivative, 1 );
  rb_define_method( cGridMetric, "minVolume", grid_minVolume, 0 );
  rb_define_method( cGridMetric, "minAR", grid_minAR, 0 );

  rb_define_method( cGridMetric, "rightHandedFace", grid_rightHandedFace, 1 );
  rb_define_method( cGridMetric, "rightHandedBoundary", grid_rightHandedBoundary, 0);
}
