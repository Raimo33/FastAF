#!/bin/bash

# consumer needs to be started first
taskset -c 4 ./consumer btcusdc btceur eurusdc &

taskset -c 1 ./producer btc usdc &
taskset -c 2 ./producer btc eur &
taskset -c 3 ./producer eur usdc &


wait