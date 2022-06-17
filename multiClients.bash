#!/bin/bash

for N in {1..50}
do
    time ruby ./client/mainClient.rb &
done
wait