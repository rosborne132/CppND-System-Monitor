#!/bin/bash

echo -n "Running process $1"

case $1 in
  start)
    make build

    ./build/monitor
    ;;

  debug)
    make debug

    ./build/monitor
    ;;

  *)
    echo -n "unknown"
    ;;
esac

echo "Process complete!"