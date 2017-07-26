#!/bin/csh -f
#
# Usage: raw_gammas.csh
#
# Specify ROOT file with recorded gamma spectrum at G115 surface.  Output
# text file can be used for rethrowing from housing for higher statistics
#
# 20170724  Michael Kelsey

# Run the SuperSim job to generate spectra

echo "Generating B33 floor spectrum through shield and cryostat"
CDMS_SimDraw raw_gammas.mac >&! raw_gammas.log

# Extract the ROOT filename from the log file

set file = `awk '/Writing simulation /{ print $5}' raw_gammas.log`
if (! -r "$file") then
   echo "ERROR: SuperSim output file $file not found."
   exit 2
endif

echo "Processing spectrum data from $file ..."

set root = $CDMS_SUPERSIM/CDMSscripts/sources/CDMSspectrum.C
root -b -l -q ${root}'("'$file'","gamma",0.003)' |& tee raw_gammas_root.log
