#!/bin/sh
# ./test.sh [Path_to_input_file]
# return: success or fail+diff

HASH=$(cat $1 | sha256sum)
cat $1 | ./hashpipe $HASH > tmp.out
DIFF=$(diff tmp.out $1)
rm tmp.out

if test "x$DIFF" == "x"
then
	echo "success"
else
	echo "fail"
	echo $DIFF
fi
