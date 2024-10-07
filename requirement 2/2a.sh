#!/bin/bash
function toBinary() {
    decimal=$1
    binary=""
    while [ $decimal -ne 0 ]
    do
        binary=$((decimal % 2))$binary
        decimal=$((decimal / 2))
    done
    echo $binary
}

function toDecimal () {
    binary=$1
    decimal=0
    length=${#binary}
    i=0
    while [ $i -lt $length ]; do
        if [ "${binary: -1}" == "1" ]; then
            decimal=$((decimal + 2**i))
        fi
        ((i++))
        binary=${binary:0:((length-i))}
    done
    echo "$decimal"
}

function convert() {
    if [ $# -eq 1 ]
    then
        toDecimal "$1"
    elif [ $1 -eq 2 ] 
    then
        toBinary $2
    else
        toDecimal "$2"
    fi
}