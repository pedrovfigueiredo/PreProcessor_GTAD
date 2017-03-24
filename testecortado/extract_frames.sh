#!/bin/bash

EXPECTED_ARGS=3
E_BADARGS=65

if [ $# -lt $EXPECTED_ARGS ]
then
  echo "Usage: <inputPath> <outputPath> video frames/sec [size=256]"
  exit $E_BADARGS
fi

NAME=${1%.*}
FRAMES=$3
BNAME=`basename $NAME`
mkdir -m 755 $2$BNAME'_keyframes'

ffmpeg -i $1 -qscale:v 2 -r $FRAMES $2$BNAME'_keyframes'/keyframe%d.jpg
