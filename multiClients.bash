#!/bin/bash

for N in {1..10}
do
    #for ruby clients
    time ruby ./client/mainClient.rb test_file
    #cpp client
    # ./client/client
    #above will make concurrent connections with server with diffrent process.
done
wait