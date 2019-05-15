#!/bin/sh
#
# test1.sh
#

. `dirname $0`/share.sh

if [ ! -x $MABOSS ]
then
    echo $MABOSS not found
    exit 1
fi

check_file()
{
    echo
    if [ $? = 0 ]; then
	echo "File $1 OK"
    else
	echo "File $1 ** error: differences found **"
    fi
}

echo
echo "Non regression test: Cell Cycle one thread"
$LAUNCHER /usr/bin/time -p $MABOSS cellcycle.bnd -c cellcycle_runcfg.cfg -c cellcycle_runcfg-thread_1.cfg -o tmp/Cell_cycle_thread_1
if [ $? != 0 ]; then exit 1; fi
diff refer/Cell_cycle_thread_1_probtraj.csv tmp/
check_file "projtraj"

diff refer/Cell_cycle_thread_1_statdist.csv tmp/ # || echo '**** error test #1.b (non regression) ****'
check_file "statdist"

if [ "$ONE_THREAD_ONLY" != "" ]; then exit 0; fi

echo
echo "Non regression test: Cell Cycle 6 threads"
$LAUNCHER $MABOSS cellcycle.bnd -c cellcycle_runcfg.cfg -c cellcycle_runcfg-thread_6.cfg -o tmp/Cell_cycle_thread_6

diff refer/Cell_cycle_thread_6_probtraj.csv tmp/ # || echo '**** error test #2.a (non regression) ****'
check_file "projtraj"

diff refer/Cell_cycle_thread_6_statdist.csv tmp/ # || echo '**** error test #2.b (non regression) ****'
check_file "statdist"

echo
echo "Non regression test: checking differences between one and 6 threads results"
diff tmp/Cell_cycle_thread_1_statdist.csv tmp/Cell_cycle_thread_6_statdist.csv # || echo '**** error test #3 (multi threads) ****'
check_file "statdist"
