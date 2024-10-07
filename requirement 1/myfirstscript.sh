#!/bin/bash
echo "Asmaa Mohamed Abozaid"

#2
rm -r Lab1

#3
mkdir Lab1

#4
cp words.txt numbers.txt Lab1

#5
cd ./Lab1
paste words.txt numbers.txt >> MergedContent.txt

#6
head -n 3 MergedContent.txt

#7
sort MergedContent.txt >> SortedMergedContent.txt

#8
echo "The sorted file is: "

#9
cat SortedMergedContent.txt

#10
chmod a-r SortedMergedContent.txt

#11
sort MergedContent.txt | uniq

#12
cat SortedMergedContent.txt | tr [a-z] [A-Z] > CapitalSortedMergedContent.txt

#13
echo "Read Permission is prevented from SortedMergedContent.txt from all users"

#14
chmod +r SortedMergedContent.txt 

#15
cat MergedContent.txt | grep --color=auto -n "^w.*[0-9]$"

#16
cat MergedContent.txt | tr i o > NewMergedContent.txt

#17
paste MergedContent.txt NewMergedContent.txt
