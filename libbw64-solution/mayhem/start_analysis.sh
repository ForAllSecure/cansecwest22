#!/bin/bash -e

pushd ../

IMAGE="$(mayhem docker-registry)/nathan/libbw64:latest"

docker build -t $IMAGE -f mayhem/Dockerfile .

docker push $IMAGE

popd

mayhem validate .

mayhem run .

