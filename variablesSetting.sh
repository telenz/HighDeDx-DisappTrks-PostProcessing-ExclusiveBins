#!/bin/bash

metCut=100
jetptCut=110
ecaloCut=5
ptCutMin=30
ptCutMax=500000000000000000000
iasCutMin=0.05
iasCutMax=0.99
region="SR"

echo "config file is read"

echo "jetPt cut = " $jetptCut
echo "MET cut   = " $metCut
echo "Ecalo cut = " $ecaloCut
echo "Pt cut    = " $ptCutMin - $ptCutMax
echo "Ias cut   = " $iasCutMin - $iasCutMax
echo "region    = " $region





