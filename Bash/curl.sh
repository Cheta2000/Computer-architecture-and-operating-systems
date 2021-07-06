#!/bin/sh
curl -s $(curl -s https://api.thecatapi.com/v1/images/search | jq -r ".[0].url" ) > /tmp/cat.jpg
catimg /tmp/cat.jpg
curl -s https://api.icndb.com/jokes/random/1 | jq ".value[0].joke"
