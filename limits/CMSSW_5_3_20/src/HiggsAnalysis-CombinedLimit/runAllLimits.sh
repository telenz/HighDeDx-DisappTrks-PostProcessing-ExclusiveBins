ini6
cmsenv

massTable=("100" "200" "300" "400" "500" "600")   #please specifiy in GeV
decaywidthTable=("1" "2" "3" "4" "5" "6" "7" "8" "9" "10" "20" "30" "40" "50" "60" "70" "80" "90" "100" "200" "300" "400" "500" "600" "700" "800" "900" "1000" "2000" "3000" "4000" "5000" "6000" "7000" "8000" "9000" "10000")   #please specifiy in cm




filenameIn="datacardsCombined_10_11"
mkdir logFiles/${filenameIn}
mkdir limits/${filenameIn}

for ctau in "${decaywidthTable[@]}"
do
    for mass in "${massTable[@]}"
    do
	if [ "$ctau" == "1" ] && ([ "$mass" == "300" ] || [ "$mass" == "400" ] || [ "$mass" == "500" ] || [ "$mass" == "600" ])
	then
	    continue
	fi

	datacardName=$(find ${filenameIn}/ -name "*mass_${mass}GeV_ctau_${ctau}cm.txt" )
	echo $datacardName
	#combine -M Asymptotic --run expected -C 0.95 -t -1 --minimizerStrategy 2 ${datacardName} > logFiles/${filenameIn}/res_mass_${mass}GeV_ctau_${ctau}cm.log
	#combine -M Asymptotic -C 0.95   --minimizerStrategy 2  --rMax=10000 ${datacardName} > logFiles/${filenameIn}/res_mass_${mass}GeV_ctau_${ctau}cm.log
	combine -M Asymptotic -C 0.95   --minimizerStrategy 2  ${datacardName} > logFiles/${filenameIn}/res_mass_${mass}GeV_ctau_${ctau}cm.log&
	#if grep -ql 'nan' logFiles/${filenameIn}/res_mass_${mass}GeV_ctau_${ctau}cm.log 
	#    then 
	#    echo 'redo limit setting!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!'
	#    combine -M Asymptotic -C 0.95   --minimizerStrategy 2  --rMax 2 ${datacardName} > logFiles/${filenameIn}/res_mass_${mass}GeV_ctau_${ctau}cm.log
	#fi
	#if [[ ! -s logFiles/${filenameIn}/res_mass_${mass}GeV_ctau_${ctau}cm.log ]]
	#    then
	#    combine -M Asymptotic -C 0.95   --minimizerStrategy 2  --rMax=10000 ${datacardName} > logFiles/${filenameIn}/res_mass_${mass}GeV_ctau_${ctau}cm.log
	#fi
	#cat logFiles/${filenameIn}/res_mass_${mass}GeV_ctau_${ctau}cm.log

	#echo ""
	#echo ""

    done
    sleep 25
    echo "after wait"
    for mass in "${massTable[@]}"
    do
	if [ "$ctau" == "1" ] && ([ "$mass" == "300" ] || [ "$mass" == "400" ] || [ "$mass" == "500" ] || [ "$mass" == "600" ])
	then
	    continue
	fi

	datacardName=$(find ${filenameIn}/ -name "*mass_${mass}GeV_ctau_${ctau}cm.txt" )
	if grep -ql 'nan' logFiles/${filenameIn}/res_mass_${mass}GeV_ctau_${ctau}cm.log 
	    then 
	    echo 'redo limit setting!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!'
	    combine -M Asymptotic -C 0.95   --minimizerStrategy 2  --rMax 2 ${datacardName} > logFiles/${filenameIn}/res_mass_${mass}GeV_ctau_${ctau}cm.log
	fi
	if [[ ! -s logFiles/${filenameIn}/res_mass_${mass}GeV_ctau_${ctau}cm.log ]]
	    then
	    combine -M Asymptotic -C 0.95   --minimizerStrategy 2  --rMax=10000 ${datacardName} > logFiles/${filenameIn}/res_mass_${mass}GeV_ctau_${ctau}cm.log
	fi
	cat logFiles/${filenameIn}/res_mass_${mass}GeV_ctau_${ctau}cm.log

	echo ""
	echo ""

    done
    #root -l -b -q plot_limit_WithCorrectCuts.cc+"(\"${filenameIn}\", ${ctau})"
    #mv test_ctau${ctau}cm.pdf limits/${filenameIn}/LimitPlot_ctau${ctau}cm.pdf
    root -l -b -q plot_limit_xsec.cc+"(\"${filenameIn}\", ${ctau})"
    mv test_ctau${ctau}cm.pdf limits/${filenameIn}/LimitPlot_ctau${ctau}cm.pdf
done

root -l -b -q plot_limit_2d.cc+"(\"${filenameIn}\")"
mv LimitPlot_2d_log.pdf limits/${filenameIn}/LimitPlot_2d_log.pdf
mv LimitPlot_2d.pdf limits/${filenameIn}/LimitPlot_2d.pdf
mv LimitPlot_2d_log_cm.pdf limits/${filenameIn}/LimitPlot_2d_log_cm.pdf
mv LimitPlot_2d_cm.pdf limits/${filenameIn}/LimitPlot_2d_cm.pdf

rm *.root
rm *.~





