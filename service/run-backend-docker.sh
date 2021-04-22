#!/bin/bash

IMAGE="dc2021q-rick"
CONT="dc2021q-rick-cont"

docker kill $IMAGE &> /dev/null
docker rm $IMAGE &> /dev/null

docker build -t $IMAGE -f Dockerfile .

docker run --name $CONT -it --rm -d $IMAGE

echo -ne "CONTAINER IP: "
docker inspect -f '{{range.NetworkSettings.Networks}}{{.IPAddress}}{{end}}' $CONT

echo "press enter to exit"
read line

docker kill $CONT
