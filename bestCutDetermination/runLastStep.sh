#!bin/bash

input="datacardsUnblinded"
output="datacardsSelectedUnblinded"

cp -rp ../systematics/signal/datacards/* ${input}/.
python selectDatacards.py ${input} ${output}
