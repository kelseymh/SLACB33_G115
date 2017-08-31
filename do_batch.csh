#!/bin/csh -f
#
# Usage: do_batch.csh
#
# Submits G115 and G124 jobs to SLAC batch system

set time = "-W 1440"
set mem = "" ### "-n 5"

bsub $time $mem -oo G115_DMC.log CDMS_G4DMC G115_DMC.mac
bsub $time $mem -oo G124_DMC.log CDMS_G4DMC G124_DMC.mac
