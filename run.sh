#!/bin/bash

#use: ./run.sh <MODEL>
NAME=$1
#Clean the environment
#    0 calls the 'Exit' option
#    6 means to erase everything, e.g., auxiliary files and folders
echo "06" | ./gtaexpress
#Compile and run
#    1 means 'Compile a model' option, another 1 means 'Standard compilation'
#    2 means 'Solve a compiled model' option, another 1 means 'Power Method'
#    0 means 'Exit' option, and another 1 means 'keep all files' (including the .tim files where the results are located)
echo "11 $NAME 21 $NAME 01" | ./gtaexpress
#Just echoes what it is doing...
echo "11 $NAME 21 $NAME 01"
#That's it!
echo "----done----"
