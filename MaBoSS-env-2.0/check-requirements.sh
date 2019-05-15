#!/bin/bash
#
# MaBoSS (Markov Boolean Stochastic Simulator)
# Copyright (C) 2011-2017 Institut Curie, 26 rue d'Ulm, Paris, France
#   
# MaBoSS is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#   
# MaBoSS is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#   
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA 
#
# Module: check-requirements.sh
# Author: Eric Viara
# Date: Jan 2017
#
# Check MaBoSS-env-2.0 requirements
#

typeset -i error=0

RED="\033[0;31m"
NOCOLOR="\033[0m"

display_error()
{
    echo -e "  ${RED}$*${NOCOLOR}"
    error=${error}+1
}

display_ok()
{
    echo "  $*: OK"
}

check_prog()
{
    type -p $1 > /dev/null 2>&1
    if [ $? != 0 ]; then
	display_error "$1: MISSING"
	return 1
    else
	display_ok $1
    fi
}

tbchecked="MaBoSS engine 2.0 requirements"
echo Checking ${tbchecked}...
echo

check_prog flex
check_prog bison
check_prog gcc
check_prog g++

tmpfile=/tmp/MaBoSS-env-2-0
trap "rm -f $tmpfile ${tmpfile}.cpp ${tmpfile}.py" 0 1 2 3

cat > ${tmpfile}.cpp <<EOF
#include <iostream>
int main() 
{
  std::cout << "hello world\n";
  return 0;
}
EOF

g++ -o $tmpfile ${tmpfile}.cpp > /dev/null 2>&1
if [ $? != 0 ]; then
    display_error "bad g++ installation"
fi

$tmpfile > /dev/null 2>&1
if [ $? != 0 ]; then
    display_error "bad g++ installation"
fi

echo

if [ $error = 0 ]; then
    echo ${tbchecked}: OK
else
    echo ${tbchecked}: $error errors found
fi

echo

error=0
tbchecked="MaBoSS-env-2.0 tools requirements"
echo Checking ${tbchecked}...
echo

check_python_module()
{
    cat > ${tmpfile}.py <<EOF
import $1
EOF
    python ${tmpfile}.py > /dev/null 2>&1
    if [ $? != 0 ]; then
	display_error "python module $1: MISSING"
    else
	display_ok "python module $1"
    fi
}

check_prog perl

check_prog python

if [ $? = 0 ]; then
    for module in matplotlib matplotlib.gridspec matplotlib.patches matplotlib.pylab numpy pandas seaborn xlsxwriter
    do
	check_python_module $module
    done
fi

echo

if [ $error = 0 ]; then
    echo ${tbchecked}: OK
else
    echo ${tbchecked}: $error errors found
fi
