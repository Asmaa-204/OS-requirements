#!/bin/bash
function checkPal() {
    str=$(echo $1 | tr [a-z] [A-z])
    length=${#str}
    i=0
    isPalindrome=1

    while [ $i -lt $((length/2)) ]
    do
        if [ ${str:$i:1} != ${str:$(($length-$i-1)):1} ];
        then
            isPalindrome=0
            break
        fi

        ((i++))
    done
    echo $isPalindrome
}