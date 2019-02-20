#!/bin/bash

docker stop piof-instance
docker rm piof-instance
docker build -t piof .

docker run -d --name piof-instance piof

docker exec -i -t piof-instance "/opt/piof/scripts/tests.sh"
docker exec -i -t piof-instance "/bin/bash"