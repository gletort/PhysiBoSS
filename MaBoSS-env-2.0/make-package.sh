#!/bin/bash
#
# make-package.sh
#
# Author: Eric Viara Institut Curie(c) copyright 2017
# Date: Jan 2017
#

packname=MaBoSS-env-2.0
packdir=/tmp/$packname

set -e
mkdir $packdir

trap "rm -r $packdir" 0 1 2 3

topdir=$(pwd)
cd engine/src
package=$(make package | grep "^Package:" | awk '{print $2}')

cd $packdir
tar xvfz $package
mv MaBoSS-*/ engine
rm engine/README
mkdir -p engine/pub
mv engine/binaries binaries
mv engine/doc doc
mv engine/examples examples

cd $topdir
cp MaBoSS.env README check-requirements.sh $packdir

set +e
find tools ! -name \*~ | grep -v "/\." | grep -v /doc/ | cpio -pdmv $packdir

cd $topdir/tools
find doc ! -name \*~ | egrep -v "/\.|\.docx|\.tex|\.ppt" | cpio -pdmv $packdir

cd $topdir
find tutorial ! -name \*~ | egrep -v "/\.|\.docx|\.tex|\.ppt" | cpio -pdmv $packdir
set -e

cd $packdir
rmdir tools/doc

cd /tmp
tar cvfz ${packname}.tgz $packname

echo
echo "MaBoSS-2.0 package: $package"
echo "MaBoSS-env-2.0 package: /tmp/${packname}.tgz"
