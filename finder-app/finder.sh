#!/bin/bash
filesdir=$1
searchstr=$2
if [ $# -lt 2 ]; then
	exit 1
fi
if [ ! -d $filesdir ]; then
	echo $filesdir "is an invalid directory"
	exit 1
fi
count=$(find $filesdir -type f | wc -l)
countr=$(grep -rl $searchstr $filesdir | wc -l)
echo "The number of files are $count and the number of matching lines are $countr"
exit 0
