ini6
cmsenv

massTable=("100" "200" "300" "400" "500")   #please specifiy in GeV
decaywidthTable=("1" "2" "5" "7" "10" "20" "50" "70" "100" "200" "500" "700" "1000" "2000")   #please specifiy in cm

#massTable=("300")   #please specifiy in GeV
#decaywidthTable=("1")   #please specifiy in cm


filenameIn="datacardsCombinedALL"
mkdir logFiles/${filenameIn}
mkdir limits/${filenameIn}
mkdir limits/${filenameIn}_xsec
for ctau in "${decaywidthTable[@]}"
do
    echo "ctau = " $ctau
    for mass in "${massTable[@]}"
    do
	echo "mass = " $mass

	if [ "$ctau" == "1" ] && ([ "$mass" == "300" ] || [ "$mass" == "400" ] || [ "$mass" == "500" ])
	then
	    continue
	fi

	datacardName=$(find ${filenameIn}/ -name "*mass_${mass}GeV_ctau_${ctau}cm.txt" )
	#combine -M Asymptotic --run expected -C 0.95 -t -1 --minimizerStrategy 2 ${datacardName} > logFiles/${filenameIn}/res_mass_${mass}GeV_ctau_${ctau}cm.log
	combine -M Asymptotic -C 0.95   --minimizerStrategy 2  ${datacardName} > logFiles/${filenameIn}/res_mass_${mass}GeV_ctau_${ctau}cm.log
	if grep -ql 'nan' logFiles/${filenameIn}/res_mass_${mass}GeV_ctau_${ctau}cm.log 
	    then 
	    echo 'redo limit setting!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!'
	    combine -M Asymptotic -C 0.95   --minimizerStrategy 2  --rMax 2 ${datacardName} > logFiles/${filenameIn}/res_mass_${mass}GeV_ctau_${ctau}cm.log
	fi
	cat logFiles/${filenameIn}/res_mass_${mass}GeV_ctau_${ctau}cm.log

	echo ""
	echo ""

    done
    #root -l -b -q plot_limit_WithCorrectCuts.cc+"(\"${filenameIn}\", ${ctau})"
    #mv test_ctau${ctau}cm.pdf limits/${filenameIn}/LimitPlot_ctau${ctau}cm.pdf
    root -l -b -q plot_limit_xsec.cc+"(\"${filenameIn}\", ${ctau})"
    mv test_ctau${ctau}cm.pdf limits/${filenameIn}_xsec/LimitPlot_ctau${ctau}cm.pdf
done

rm *.root
rm *.~





