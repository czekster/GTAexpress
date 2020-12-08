# GTAexpress: A software package to handle Kronecker descriptors

This software will solve a SAN (Stochastic Automata Networks) model. It works with Kronecker Descriptors. The idea is to use a component based formalism to extract performance indices such as throughput, utilization, or number in the system. It is widely used due to its memory storage schema.

Abstract: This paper presents a software package, called GTAexpress, to handle structured continuous-time Markovian models expressed using Generalized Tensor Algebra, also known as, Kronecker descriptors. The proposed software package has the most advanced methods to provide stationary and transient solutions as well as some basic structural properties of models represented as a sum of generalized tensor products. Other software tools already provide some approaches based on tensor representation, like, PEPS and SMART. However, such tools are bounded to a specific modeling formalism. The basic idea of GTAexpress is to provide Kronecker descriptor-based solutions that can be easily used as a package in new tools or as a library in the existing high-level formalisms tools.
Links
[IEEExplore](http://ieeexplore.ieee.org/abstract/document/5290659/)
[Academia.edu](http://www.academia.edu/download/46581681/gtaexpress_QEST09.pdf)
[Research Gate](https://www.researchgate.net/publication/224605920_GTAexpress_A_Software_Package_to_Handle_Kronecker_Descriptors)

## How to cite GTAexpress?
R. M. Czekster, P. Fernandes and T. Webber GTAexpress: A Software Package to Handle Kronecker Descriptors 2009 Sixth International Conference on the Quantitative Evaluation of Systems (QeST), Budapest, 2009, pp. 281-282. doi: 10.1109/QEST.2009.26

## Companion paper describing the main technique
R. M. Czekster, P. Fernandes and T. Webber Efficient Vector-Descriptor Product Exploiting Time-Memory Tradeoffs. Performance Evaluation Review, v. 39, p. 2-9, 2011.

*The models are located in the /bin folder.*

## Installation
- unzip the file: unzip gtaexpress.zip (notice some example model files in bin/ folder)
- change folder: cd gtaexpress
- make the project: make
- if no errors were found, then execute the tool: cd bin ; ./gtaexpress

## A bash script to run several models
If you are feeling lucky and confident, take a look at this following script (run.sh):

```
#!/bin/bash

#use: ./run.sh 
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
```

## Steps to use the script:
- put this script in the bin/ folder: cp run.sh ./bin
- change permissions: chmod a+x run.sh
- run 'printers3c.san' model (for example): ./run.sh printers3c
- analyze results for the model: vi printers3c.tim

# Support
Please, do contact me if you encounter any sort of trouble: rczekster at gmail is my electronic address. Do not hesitate to send me an e-mail!

