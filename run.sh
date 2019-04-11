#!/bin/bash


#=============================================
#	DEFINE
#=============================================

USAGE=" \
Error: usage : ./run.sh <arg>
\thelp              : print this help
\tcreate [name:tag] : create tmp dir and cpy bin
\tsearch            : search for infected binaries
\tdump              : look for war func address
"

RESET="\e[39m"
RED="\e[31m"
GREEN="\e[32m"

#=============================================
#	BUILTIN
#=============================================

builtin_usage()
{
	printf "$USAGE"
	exit 0
}

builtin_create()
{
	set -x

	if [ ! -d /tmp/test ] ; then
		mkdir "/tmp/test"
	fi
	if [ ! -d /tmp/test2 ] ; then
		mkdir "/tmp/test2"
	fi

	if [ -f /tmp/war.log ] ; then
		rm -f /tmp/war.log
	fi

	rm -f /tmp/test/*
	rm -f /tmp/test2/*

	gcc tbin/main.c -o tbin/a.out
	gcc tbin/antivirus.c -o tbin/antivirus

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

	touch /tmp/war.log
	set +x
}

builtin_search()
{
	for filename in /tmp/test{,2}/* ; do
		printf "$filename -> "
		RESULT=$(xxd $filename | head | grep "Hi!")
		if [[ "$RESULT" =~ "Hi!" ]] ; then
			printf "$RED INFECTED $RESET\n"
		else
			printf "$GREEN CLEAN $RESET\n"
		fi
	done | column -t
}

builtin_dump()
{
	objdump -d -M intel war | egrep -e "<start>:|<opening>:|<war>:|<locate>:|<inspect>:|<infect>:|<inject>:|<release>:|<cypher_end>:|<end>:|<end_of_data>:|cafeba"

	TEST=$(grep "//# define DEBUG" includes/war.h)
	if [ -z "$TEST" ] ; then
		echo "DEBUG"
		JMP=$(objdump -d -M intel war | egrep -e "e9 a0 05 00 00" | awk {'print $1'} | tr -dc '[0-9][a-f]')
	else
		echo " NO DEBUG"
		JMP=$(objdump -d -M intel war | egrep -e "e9 8c 05 00 00" | awk {'print $1'} | tr -dc '[0-9][a-f]')
	fi


	END_OF_DATA=$(objdump -d -M intel war | egrep -e "<end_of_data>:" | awk {'print $1'} | tr -dc '[0-9][a-f]')
	printf "ENTRY_OFF :\t"
	python -c "print ((0x$END_OF_DATA - 0x$JMP) - 1)"

	printf "END ENCRYPT (_rc4) :\t"
	objdump -d -M intel war | grep "<_rc4>:" | awk {'print 0x$1'}
}

builtin_test()
{
	# only test for antivirus
	sleep 20 &
	./war
}

main()
{
	if [ $# -lt 1 ] ; then
		builtin_usage
	fi

	case "$1" in
		"create")
			builtin_create $@
			;;
		"search")
			builtin_search $@
			;;
		"dump")
			builtin_dump $@
			;;
		"test")
			builtin_test $@
			;;
		"help"|*)
			builtin_usage
			;;
	esac
}

main $@
