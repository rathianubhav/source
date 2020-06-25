#!/bin/sh

source /lib/rsb/io.sh

for i in test/*.src ; do
	srcfile=$i
	outfile=$(echo $i | sed 's|.src|.output|g')
	
	output=$(src $srcfile)
	expectedoutput=$(cat $outfile)
	if [[ "$output" != "$expectedoutput" ]] ; then
		err "testcase $(basename $i) failed"
	else
		success "testcase $(basename $i) passed"
	fi
done
