#!/bin/bash

function print_info {
grep -i 'BCM4a' REPORT_OUTPUT/COIN/PRODUCTION/replay_coin_production_${1}_-1.report | head -n 2 | tail -n 1
grep -i 'Trig6' REPORT_OUTPUT/COIN/PRODUCTION/replay_coin_production_${1}_-1.report | head -n 1
grep -i 'eff' UTIL_SIDIS/REPORT/sidis_report_${1}.report 
grep -i 'Live Time' UTIL_SIDIS/REPORT/sidis_report_${1}.report 
grep -i 'HMS BCM1 Current' REPORT_OUTPUT/COIN/PRODUCTION/replay_coin_production_${1}_-1.report  | head -n 1
}

print_info ${1} | column -t -s ':'
