ini6
cmsenv

massTable=("100" "200" "300" "400" "500" "600")   #please specifiy in GeV
decaywidthTable=("1" "2" "3" "4" "5" "6" "7" "8" "9" "10" "20" "30" "40" "50" "60" "70" "80" "90" "100" "200" "300" "400" "500" "600" "700" "800" "900" "1000" "2000" "3000" "4000" "5000" "6000" "7000" "8000" "9000" "10000")   #please specifiy in cm

#massTable=("100")   #please specifiy in GeV
#decaywidthTable=("1")   #please specifiy in cm


filenameIn="datacardsCombined"
mkdir logFiles/${filenameIn}
mkdir limits/${filenameIn}
mkdir limits/${filenameIn}_xsec
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





