#! /bin/bash -xue

PACKAGE='ESP'
VERSION='117-rc.1'
OCC_COPY_SOURCE='/u/shared/fun3d/fun3d_users/modules/ESP/116/OpenCASCADE-7.3.1'

if [ $# -gt 0 ] ; then
   . common.sh  $1
else
   . common.sh
fi

echo Build ${PACKAGE} ${VERSION}

module purge
module load ${GCC_MODULE}
module list


# https://acdl.mit.edu/ESP/PreBuilts/
# https://acdl.mit.edu/ESP/archive/

rm -f ESPbeta.tgz
wget https://acdl.mit.edu/ESP/archive/ESPbeta.tgz
rm -rf EngSketchPad
tar xzf ESPbeta.tgz
( cd EngSketchPad/config && ./makeEnv )
( cd EngSketchPad/src && . ./makeEnv && make CC=gcc CXX=g++)

mkdir ${MODULE_DEST}
cp -r ${OCC_COPY_SOURCE} ${MODULE_DEST}
mkdir ${MODULE_DEST}/EngSketchPad
cp -r EngSketchPad/include EngSketchPad/lib EngSketchPad/bin ${MODULE_DEST}/EngSketchPad

mkdir -p ${MODFILE_BASE}
cat > ${MODFILE_DEST} << EOF
#%Module#
proc ModulesHelp { } { puts stderr "$PACKAGE $VERSION" }

set sys      [uname sysname]
set modname  [module-info name]
set modmode  [module-info mode]

set base    $MODULE_BASE
set version $VERSION

set logr "/bin"

if { \$modmode == "switch1" } {
  set modmode "switchfrom"
}
if { \$modmode == "switch2" } {
  set modmode "switchto"
}
if { \$modmode != "switch3" } {
  system  "\$logr/logger -p local2.info envmodule \$modmode \$modname"
}

setenv ESP_ROOT \$base/\$version/EngSketchPad
setenv CASROOT \$base/\$version/OpenCASCADE-7.3.1

prepend-path PATH \$base/\$version/EngSketchPad/bin

prepend-path LD_LIBRARY_PATH \$base/\$version/EngSketchPad/lib
prepend-path LD_LIBRARY_PATH \$base/\$version/OpenCASCADE-7.3.1/lib

EOF

echo Set group ownership and permssions
chgrp -R ${GROUP}  ${MODULE_DEST}
chmod -R g+rX      ${MODULE_DEST}
chmod -R g-w,o-rwx ${MODULE_DEST}

chgrp -R ${GROUP}  ${MODFILE_DEST}
chmod -R g+rX      ${MODFILE_DEST}
chmod -R g-w,o-rwx ${MODFILE_DEST}
