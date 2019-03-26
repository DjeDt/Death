#!/bin/bash

set -x

if [ ! -f "war" ] || [ ! -f "patcher" ] ; then
	make re && sync
fi

if  [ ! -d /tmp/test ] ; then
	mkdir /tmp/test
fi

if  [ ! -d /tmp/test2 ] ; then
	mkdir /tmp/test2
fi


if [ $1 ] ; then

	if [ "$1" == "dump" ] ; then
		objdump -d -M intel war | egrep -e "<cypher_beg>:|<war>:|<locate>:|<inspect>:|<infect>:|<inject>:|<release>:|<cypher_end>:|<end>:"
		exit 0
	elif [ "$1" == "create" ] ; then
		rm -f /tmp/test/*
		rm -f /tmp/test2/*
		gcc tbin/main.c -o tbin/a.out

		cp tbin/a.out /tmp/test/a.out
		cp tbin/a.out /tmp/test/a.out2
		cp tbin/a.out /tmp/test/a.out3
		cp tbin/a.out /tmp/test/a.out4

		cp tbin/a.out /tmp/test2/a.out
		cp tbin/a.out /tmp/test2/a.out2
		cp tbin/a.out /tmp/test2/a.out3
		cp tbin/a.out /tmp/test2/a.out4

		cp /bin/{ls,echo} /tmp/test
		cp /bin/{ls,echo} /tmp/test2

	elif [ "$1" == "search" ] ; then

		for filename in /tmp/test{,2}/a.out* ; do
			RESULT=$(xxd $filename | head | grep "Hi!")
			if [[ "$RESULT" =~ "Hi!" ]] ; then
				echo "$filename OK"
			fi
		done
		exit 1
	elif [ "$1" == "run" ] ; then
		./war
	else
		echo "wrong command u stupid piece of shit"
		exit 1
	fi
fi
