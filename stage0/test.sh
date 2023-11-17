#!/bin/bash

#Pascallite Compiler test script
#2023 - Robert Burnett

#get full path to the compiler
COMPILER="$(realpath "${1}")"
DATAPATH="$2"
IGNORE_DATASETS="051 052"

#ANSI COLOR CODES
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

#make sure arguments are passed correctly
if [[ "$#" -ne 2 ]]; then
   echo -e "[ERROR] compiler name, then dataset location must be passed as argument"
   exit
fi

if [[ ! -e $COMPILER ]]; then 
   echo -e "[ERROR] compiler \"$COMPILER\" does not exist"
   exit
fi

if [[ ! -e results ]]; then
   mkdir results
fi

function output {
   if [[ $1 == 0 ]]; then 
      echo -e "${GREEN}[$SET SUCCESS]${NC} $2"
   else
      echo -e "${RED}[$SET    FAIL]${NC} $2"
   fi
}

function differences {
   return $(($(diff "$1" "$2" | wc -l) - 3))
}

#collect the id number of all datasets
for file in "${DATAPATH}"*.dat
do
   SET=${file:${#DATAPATH}:3}
   
   if echo "$IGNORE_DATASETS" | grep -q "$SET"; then
      echo "[${SET} SKIPPED] dataset should compile, but is missing assembly file to check the diff"

   #   #if the dataset has an assembly file, the compiler should generate an assembly file that passes the diff
   elif test -f "${DATAPATH}${SET}.asm"; then

      "$COMPILER" "${DATAPATH}${SET}.dat" "results/${SET}.lst" "results/${SET}.asm"

      if [[ $(differences "${DATAPATH}${SET}.lst" "results/${SET}.lst" ) -le 1 ]]; then 
         output 0 "generated lst passed the diff"
      else
         output 1 "generated lst did not pass the diff"
      fi

      if [[ $(differences "${DATAPATH}${SET}.asm" "results/${SET}.asm" ) -le 1 ]]; then 
         output 0 "generated assembly passed the diff"
      else
         output 1 "generated assembly did not pass the diff"
      fi

      #if the dataset has no assembly file, we should make sure that the compiler exits with a failure code indicating it caught the error
   else
      "$COMPILER" "${DATAPATH}${SET}.dat" "results/${SET}.lst" "results/${SET}.asm"
      if [[ $? -eq 0 ]]; then 
         output 1 "compiler did not catch error in source file"
      else
         output 0 "compiler caught error in source file"
      fi
   fi
done
