#! /bin/bash


SPEC="COIN"


# Which run to analyze.
runNum=$1
if [ -z "$runNum" ]; then
  runNum=$lastRun
fi

# How many events to analyze.
numEvents=-1

# Which scripts to run.
script="./UTIL_SIDIS/scripts/replay/replay_coin_scalers.C"

# Which commands to run.
runHcana="./hcana -q \"${script}(${runNum}, ${numEvents})\""
runReportMon="./UTIL_SIDIS/scripts/get_sidis_charge_report.py ${runNum}"


#Define smalle e12-10-003 report file directory
reportDir="UTIL_SIDIS/REPORT/sidis_charge_report_${runNum}.report"
#Define command to open report file


# Name of the replay ROOT file
rootFileDir="./ROOTfiles"
replayFile="coin_replay_production_hElec_pProt_${runNum}"
rootFile="${replayFile}_${numEvents}.root"


# Where to put log.
reportFile="REPORT_OUTPUT/${SPEC}/PRODUCTION/replay_coin_scalers_${runNum}_${numEvents}.txt"
summaryFile="REPORT_OUTPUT/${SPEC}/PRODUCTION/summary_scalers_${runNum}_${numEvents}.txt"


# Start analysis and monitoring plots.
{
  echo ""
  echo ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:="
  echo "" 
  date
  echo ""
  echo "Running ${SPEC} COIN analysis on the run ${runNum}:"
  echo " -> SCRIPT:  ${script}"
  echo " -> RUN:     ${runNum}"
  echo " -> NEVENTS: ${numEvents}"
  echo " -> COMMAND: ${runHcana}"
  echo ""
  echo ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:="

  sleep 2
  eval ${runHcana}

  
  echo ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:="
  echo ""
  echo "Running get_sidis_report.py for analyzed ${SPEC} COIN run ${runNum}:"
  echo " -> REPORT OUTPUT:  ${reportDir}"
  echo ""
  echo ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:="


  sleep 2
  eval ${runReportMon}   #execute ./get_heep_report.py

 cat ${reportDir}


} 
#2>&1 | tee "${reportFile}"
#2>&1 | tee "${summaryFile}"
