#!/bin/bash

taskset -c 1 ./producer btc-usdc &
taskset -c 2 ./producer btc-eur &
taskset -c 3 ./producer eur-usdc &

taskset -c 4 ./consumer btc-usdc btc-eur eur-usdc &

wait