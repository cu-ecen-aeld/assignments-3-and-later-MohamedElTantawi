#!/bin/bash
writerfile=$1
writerstr=$2
if [ $# -lt 2 ]; then
	exit 1
fi
mkdir -p $writerfile
touch "$writerfile.txt"
echo $writerstr > "$writerfile.txt"
exit 0
