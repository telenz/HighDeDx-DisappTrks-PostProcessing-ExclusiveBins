#!/bin/bash


ini5
cmsenv



ptTable=("30" "40" "50")              #please specifiy in GeV
iasTable=("05" "10" "20" "30" "40")   #please specifiy in *100


for ptCut in "${ptTable[@]}"
  do

  for iasCutTimes100 in "${iasTable[@]}"
    do

    source runAllLimits.sh

  done


done