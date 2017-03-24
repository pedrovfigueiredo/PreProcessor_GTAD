#!/bin/bash

EXPECTED_ARGS=4
E_BADARGS=65

if [ $# -lt $EXPECTED_ARGS ]
then
echo "Usage: <inputPath> <outputPath> <initialTime> <duration> [size=256]"
exit $E_BADARGS
fi

ffmpeg -y -i $1 -ss $3 -t $4 -strict -2 $2
