#!/bin/bash

#TODO hugepages

if [ -z "$BINANCE_API_KEY" ]; then
  echo "Error: BINANCE_API_KEY environment variable is not set"
  exit 1
fi

echo "Starting producers..."

taskset -c 0 ./producer btc-usdc &
taskset -c 1 ./producer btc-eur &
taskset -c 2 ./producer eur-usdc &

echo "Starting consumer..."

taskset -c 3 ./consumer btc-usdc btc-eur eur-usdc &

wait