#!/bin/bash

# Server address and port
SERVER_ADDRESS="localhost"
SERVER_PORT=3131

# Message to send
MESSAGE="hello"

# Number of clients to spawn
NUM_CLIENTS=10000000

# Loop to spawn clients
for ((i=1; i<=$NUM_CLIENTS; i++))
do
    # Send the message to the server in the background
    (echo "$i" > /dev/tcp/$SERVER_ADDRESS/$SERVER_PORT) &
    sleep 0.01
done

echo "Sent '$MESSAGE' from $NUM_CLIENTS clients"
