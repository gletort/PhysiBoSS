
if [ -z "$MABOSS" ] ; then MABOSS=../src/MaBoSS; fi
if [ -z "$MABOSS_CLIENT" ] ; then MABOSS_CLIENT=../src/MaBoSS-client; fi
export DYLD_LIBRARY_PATH=../src:$DYLD_LIBRARY_PATH
