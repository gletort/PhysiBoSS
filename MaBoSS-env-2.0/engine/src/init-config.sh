#!/bin/sh
#
# init-config.sh
#
# 2011-03-28
#
# Eric Viara for Institut Curie copyright 2011
#

CONFIG_NAME=maboss-config
CONFIG_H=${CONFIG_NAME}.h

if [ $# = 0 -a -r $CONFIG_H ]; then exit 0; fi

tmpfile=/tmp/${CONFIG_NAME}$$.c

cat > $tmpfile <<EOF
#include <stdlib.h>
int main()
{
  struct drand48_data data;
  return 0;
}
EOF

$CXX -c $tmpfile > /dev/null 2>&1

if [ $? = 0 ]
then
   echo "#define HAS_RAND48_T" > $CONFIG_H
else
   echo "//#define HAS_RAND48_T" > $CONFIG_H
fi

cat > $tmpfile <<EOF
#include <stdlib.h>
int main()
{
  lrand48();
  drand48();
  return 0;
}
EOF

$CXX -c $tmpfile > /dev/null 2>&1

if [ $? = 0 ]
then
   echo "#define HAS_RAND48" >> $CONFIG_H
else
   echo "//#define HAS_RAND48" >> $CONFIG_H
fi

rm -f $tmpfile ${CONFIG_NAME}*.o

cat > $tmpfile <<EOF
#include <unordered_map>
#include <stdlib.h>
int main()
{
  return 0;
}
EOF

$CXX -c $tmpfile -std=c++0x > /dev/null 2>&1

if [ $? = 0 ]
then
   echo "// @HAS_UNORDERED_MAP"  >> $CONFIG_H
   echo "#define HAS_UNORDERED_MAP" >> $CONFIG_H
   echo "#include <unordered_map>" >> $CONFIG_H
   echo "#define STATE_MAP std::unordered_map" >> $CONFIG_H
   echo "#define HASH_STRUCT hash" >> $CONFIG_H
else
    cat > $tmpfile <<EOF
#include <tr1/unordered_map>
#include <stdlib.h>
int main()
{
  return 0;
}
EOF

    $CXX -c $tmpfile > /dev/null 2>&1
    if [ $? = 0 ]
    then
	echo "// @HAS_UNORDERED_MAP"  >> $CONFIG_H
	echo "#define HAS_UNORDERED_MAP" >> $CONFIG_H
	echo "#include <tr1/unordered_map>" >> $CONFIG_H
	echo "#define STATE_MAP std::tr1::unordered_map" >> $CONFIG_H
	echo "#define HASH_STRUCT std::tr1::hash" >> $CONFIG_H
    else
	echo "//#define HAS_UNORDERED_MAP" >> $CONFIG_H
	echo "#define STATE_MAP std::map" >> $CONFIG_H
    fi
fi
rm -f $tmpfile ${CONFIG_NAME}*.o



