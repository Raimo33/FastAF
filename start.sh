#!/bin/bash

taskset -c 1 ./producer btcusdc &
taskset -c 2 ./producer btceur &
taskset -c 3 ./producer eurusdc &

wait