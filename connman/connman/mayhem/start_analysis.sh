#!/bin/bash -e

pushd ..
image="$(mayhem docker-registry)/nathan/connman:latest"
docker build -t $image -f mayhem/Dockerfile .
docker push $image
popd

#mayhem validate .
mayhem run .
