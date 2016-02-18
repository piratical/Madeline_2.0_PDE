#!/bin/bash
parse_file="../index.html"
regex1="COMMAND LINE:<\/span><input type=\"text\" value=\'(.+)'>"
regex2="madeline2(.*)? ([a-z0-9_]+\.(data|ods|html)(\.(zip|gz|bz2))?)"
#
# Add initial line to commands file:
#
echo '#!/bin/bash' > run_madeline.sh
chmod u+x ./run_madeline.sh
#
# Find and process all the command lines referenced in index.html
#
echo '********************************'
echo '* Setting up command lines ... *'
echo '********************************'
while read -r line
do
	if [[ $line =~ $regex1 ]]
	then
		raw_command="${BASH_REMATCH[1]}" # The raw command before amelioration
		if [[ $raw_command =~ $regex2 ]]
		then
			# DEBUG:
			#echo "M1: ${BASH_REMATCH[1]}"
			#echo "M2: ${BASH_REMATCH[2]}"
			#echo "M3: ${BASH_REMATCH[3]}"
			#echo "M4: ${BASH_REMATCH[4]}"
			#echo "M5: ${BASH_REMATCH[5]}"
			#echo "M6: ${BASH_REMATCH[6]}"
			
			flags="${BASH_REMATCH[1]}"
			data_file="../data/${BASH_REMATCH[2]}" # Add the relative path to the data
			command_string="madeline2 -S${flags} ${data_file}" # We add the -S flag too
			echo "ADDING: $command_string"
			echo $command_string >> run_madeline.sh
		fi
	fi
done < "$parse_file"
#
# Now run the commands:
#
echo '********************************'
echo '* Running command lines ...    *'
echo '********************************'
./run_madeline.sh
echo '********************************'
echo '* Comparing to expected ...    *'
echo '********************************'
diff -r -q -s ./ ../expected/ > diff_report

regex_identical="Files ./([a-z0-9_]+\.svg) and ../expected/([a-z0-9_]+\.svg) are identical"
regex_different="Files ./([a-z0-9_]+\.svg) and ../expected/([a-z0-9_]+\.svg) differ"
echo "" > summary.report
while read -r line
do
	if [[ $line =~ $regex_identical ]]
	then
		echo -e "\033[1;32m PASS: ${BASH_REMATCH[1]}"
		echo "<p style='font-family:Ubuntu;font-size:20px;font-style:italic;color:#0d0;'>PASS: ${BASH_REMATCH[1]}</p>" >> summary.report
	elif [[ $line =~ $regex_different ]]
	then
		echo -e "\033[1;31m DIFFER: ${BASH_REMATCH[1]}"
		echo "<p style='font-family:Ubuntu;font-size:20px;font-style:italic;color:#f00;'>DIFFER: ${BASH_REMATCH[1]}</p>" >> summary.report
	fi
done < diff_report
echo -n -e "\033[0m" # reset terminal color
date "+<p style='font-family:Ubuntu;font-size:20px;font-style:italic'>Test results in this document were last run on %Y.%m.%d at %H:%Mh.</p>" > report.date
