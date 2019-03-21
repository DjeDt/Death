#!/bin/bash

set +x

if [ ! -f "war" ] || [ ! -f "patcher" ] ; then
	make re && sync
fi

if [ $1 ] ; then

	if [ "$1" == "dump" ] ; then
		objdump -d -M intel war | egrep -e "<cypher_beg>:|<war>:|<locate>:|<inspect>:|<infect>:|<inject>:|<release>:|<cypher_end>:|<end>:"
		exit 0
	else
		echo "wrong command u stupid piece of shit"
		exit 1
	fi
fi

if  [ ! -d /tmp/test ] ; then
	mkdir /tmp/test
fi

if  [ ! -d /tmp/test2 ] ; then
	mkdir /tmp/test2
fi

cp tbin/a.out /tmp/test
cp tbin/a.out /tmp/test2

make re
./patcher
./war
