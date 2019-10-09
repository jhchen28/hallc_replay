#!/bin/bash
#set -u 

function print_the_help {
  echo "USAGE: $0 -r <run_number> -n <num> "
  echo "  OPTIONS: "
  echo "            -r,--run           Run number (required)"
  echo "            -n,--n-events      Number of events (default: all)"
  echo "            -s,--skip          Skip N first events (default: 0)"
  echo "            -a,--all           Store ALL variables (default: not set)"
  echo "            -c,--coin          Run in COIN mode (default: SHMS single-arm mode)"
  exit 
}


./SCRIPTS/replay.sh -m HMS $@ || print_the_help
