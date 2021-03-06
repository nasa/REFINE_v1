#!/usr/bin/env bash

set -x # echo commands
set -e # exit on first error
set -u # Treat unset variables as error

if [ $# -gt 0 ] ; then
    src=$1/src
else
    src=${HOME}/refine/parmetis/src
fi

geomfile=hemisphere.egads

# ~/esp/EngSketchPad/bin/serveCSM -batch hemisphere.csm
# ref_geom_test hemisphere.egads hemisphere.meshb
# ref_driver -i hemisphere.meshb -g hemisphere.egads -r 4 -o hemicurve

${src}/ref_acceptance hemicurve.meshb hemicurve-metric.solb 0.1
mpiexec -np 4 ${src}/ref_driver -i hemicurve.meshb -g ${geomfile} -m hemicurve-metric.solb -o hemicurve1 -r 1 -s 20
${src}/ref_acceptance hemicurve1.meshb hemicurve1-metric.solb 0.1
${src}/ref_metric_test hemicurve1.meshb hemicurve1-metric.solb > accept-hemisphere-uniform-para-01.status

cat accept-hemisphere-uniform-para-01.status
../../check.rb accept-hemisphere-uniform-para-01.status 0.1 6.0

mpiexec -np 8 ${src}/ref_interp_test hemicurve.meshb hemicurve1.meshb
mpiexec -np 8 ${src}/ref_interp_test hemicurve1.meshb hemicurve.meshb

