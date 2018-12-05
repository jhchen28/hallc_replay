#! /bin/bash

#ulimit -v 8000000
ulimit -v 10000000

SPEC="COIN"

# What is the last run number for the spectrometer.
# The pre-fix zero must be stripped because ROOT is ... well ROOT
#lastRun=$( \
#    ls raw/"${spec}"_all_*.dat raw/../raw.copiedtotape/"${spec}"_all_*.dat -R 2>/dev/null | perl -ne 'if(/0*(\d+)/) {prin#t "$1\n"}' | sort -n | tail -1 \
#)
lastRun=$( \
    ls raw/coin_all_*.dat raw/../raw.copiedtotape/coin_all_*.dat cache/coin_all_*.dat -R 2>/dev/null | perl -ne 'if(/0*(\d+)/) {print "$1\n"}' | sort -n | tail -1 \
)

# Which run to analyze.
runNum=$1
if [ -z "$runNum" ]; then
  runNum=$lastRun
fi

# How many events to analyze.
numEvents=-1

# Which scripts to run.
script="./UTIL_SIDIS/scripts/replay/replay_production_coin_sidis.C"
#config="CONFIG/${SPEC}/PRODUCTION/${spec}_coin_production.cfg"
#charge_script="./UTIL_ED/online/Macros/charge_counter.C"

# Which commands to run.
runHcana="./hcana -q \"${script}(${runNum}, ${numEvents})\""
runReportMon="./UTIL_SIDIS/scripts/get_sidis_report.py ${runNum} ${numEvents}"
#runCharge="root -l \"${charge_script}(\\\"${exp}\\\", ${runNum}, ${numEvents})\""


#Define smalle e12-10-003 report file directory
reportDir="UTIL_SIDIS/REPORT/sidis_report_${runNum}.report"
#Define command to open report file
openReport="emacs ${reportDir} &"

#runOnlineGUI="./online -f ${config} -r ${runNum}"
#saveOnlineGUI="./online -f ${config} -r ${runNum} -P"

# Name of the replay ROOT file
rootFileDir="./ROOTfiles"
replayFile="coin_replay_production_hElec_pProt_${runNum}"
rootFile="${replayFile}_${numEvents}.root"
latestRootFile="${rootFileDir}/${replayFile}_latest.root"

# Names of the monitoring file
#monRootDir="./HISTOGRAMS/${SPEC}/ROOT"
#monPdfDir="./HISTOGRAMS/${SPEC}/PDF"
#monRootFile="${spec}_coin_production_${runNum}.root"
#monPdfFile="${spec}_coin_production_${runNum}.pdf"
#latestMonRootFile="${monRootDir}/${spec}_coin_production_latest.root"
#latestMonPdfFile="${monPdfDir}/${spec}_coin_production_latest.pdf"

# Where to put log.
reportFile="REPORT_OUTPUT/${SPEC}/PRODUCTION/replay_coin_production_${runNum}_${numEvents}.txt"
summaryFile="REPORT_OUTPUT/${SPEC}/PRODUCTION/summary_production_${runNum}_${numEvents}.txt"

# What is base name of onlineGUI output.
#outFile="${spec}_coin_production_${runNum}"

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

  # Link the ROOT file to latest for online monitoring
 # ln -fs ${rootFile} ${latestRootFile}
  
  echo ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:="
  echo ""
  echo "Running get_sidis_report.py for analyzed ${SPEC} COIN run ${runNum}:"
  echo " -> REPORT OUTPUT:  ${reportDir}"
  echo ""
  echo ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:="

  sleep 2
  eval ${runReportMon}   #execute ./get_heep_report.py
  #eval ${runCharge}      #execute charge_counter.C script
  #eval "evince ./UTIL_ED/temp.pdf &"     #open pdf containing monitoring histograms 
  eval ${openReport}     #open report file


#  echo ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:="
#  echo ""
#  echo "Running onlineGUI for analyzed ${SPEC} COIN run ${runNum}:"
#  echo " -> CONFIG:  ${config}"
#  echo " -> RUN:     ${runNum}"
#  echo " -> COMMAND: ${runOnlineGUI}"
#  echo ""
#  echo ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:="

#  cd onlineGUI
#  eval ${runOnlineGUI}
#  eval ${saveOnlineGUI}
#  mv "${outFile}.pdf" "../HISTOGRAMS/${SPEC}/PDF/${outFile}.pdf"
#  mv "${outFile}.root" "../HISTOGRAMS/${SPEC}/ROOT/${outFile}.root"
#  cd ..
#  ln -fs ${monRootFile} ${latestMonRootFile}
#  ln -fs ${monPdfFile} ${latestMonPdfFile}

  echo ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:="
  echo ""
  echo "Done analyzing ${SPEC} run ${runNum}."
  echo ""
  echo ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:="

} 
#2>&1 | tee "${reportFile}"
#2>&1 | tee "${summaryFile}"
