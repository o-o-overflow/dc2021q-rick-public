#!/bin/bash

set -x

IMAGE="rick-build"
CONT="rick-build-cont"

docker kill $IMAGE &> /dev/null
docker rm $IMAGE &> /dev/null

docker build -t $IMAGE -f Dockerfile-build .

docker run --name $CONT -i --rm -d $IMAGE

docker cp $CONT:/build/rick rick

docker kill $CONT
