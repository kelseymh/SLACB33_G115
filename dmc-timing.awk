#!/usr/bin/awk -f
#
# Usage: dmc-timing.awk <log-file>
#
# Extract run-by-run timing information from log file to correlate with
# event (hit) sizes from dmc-test.dat.  Job must be run with one event
# per run.
#
# 20170801  Michael Kelsey

BEGIN {
    split("1 10 50 100 500 1000 5000 10000 50000",energy);
    print "E (eV)\tUser (s)\tClock (s)\tSys (s)\tPhonons (eV)";
 }

/GeneratePhonons/ { ephn = $2 * 1e6; }

/^Run/ {
    run=substr($2,1,length($2)-1);
    utime=substr($3,index($3,"=")+1,length($3)-6);
    rtime=substr($4,index($4,"=")+1,length($4)-6);
    stime=substr($5,index($5,"=")+1,length($5)-6);

    print energy[run+1] "\t" utime "\t" rtime "\t" stime "\t" ephn;
}
