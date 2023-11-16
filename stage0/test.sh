#!/usr/local/bin/bash

#Pascallite Compiler test script
#2023 - Robert Burnett

#get full path to the compiler
COMPILER="$(realpath ${1})"
DATAPATH="./dat/"

#ANSI COLOR CODES
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

#make sure arguments are passed correctly
if [[ "$#" -ne 1 ]]; then
   echo -e "[ERROR] compiler name must be passed as argument"
   exit
fi

if [[ ! -e $COMPILER ]]; then 
   echo -e "[ERROR] compiler \"$COMPILER\" does not exist"
   exit
fi

#collect the id number of all datasets
for file in ${DATAPATH}*.dat
do
   SET=${file:${#DATAPATH}:3}

      #   #if the dataset has an assembly file, the compiler should generate an assembly file that passes the diff
      if test -f ${DATAPATH}${SET}.asm; then
         $COMPILER ${DATAPATH}${SET}.dat results/${SET}.lst results/${SET}.asm
         if diff -q ${DATAPATH}${SET}.asm results/${SET}.asm 1>/dev/null; then 
            echo -e "${GREEN}[$SET SUCCESS]${NC} generated assembly passed the diff"
         else
            echo -e "${RED}[$SET    FAIL]${NC} generated assembly did not pass the diff"
         fi

         #if the dataset has no assembly file, we should make sure that the compiler exits with a failure code indicating it caught the error
      else
         $COMPILER ${DATAPATH}${SET}.dat results/${SET}.lst results/${SET}.asm
         if [[ $? -eq 0 ]]; then 
            echo -e "${RED}[$SET    FAIL]${NC} compiler erroneously compiled a failing test case"
         else
            echo -e "${GREEN}[$SET SUCCESS]${NC} compiler did not compile failing test case"
         fi
      fi
   done