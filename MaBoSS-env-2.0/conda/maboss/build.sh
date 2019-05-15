cd engine/src
make install
make MAXNODES=128 install
make MAXNODES=256 install
mkdir -p ${PREFIX}/bin
mv ../pub/MaBoSS  ../pub/MaBoSS_*n ${PREFIX}/bin
mv ../pub/MaBoSS-server  ../pub/MaBoSS_*n-server ${PREFIX}/bin
mv ../pub/MaBoSS-client ${PREFIX}/bin
cd ../..
mkdir -p "${PREFIX}/share/MaBoSS"
#mv doc tutorial examples ${PREFIX}/share/MaBoSS/
mv tools/* ${PREFIX}/bin

