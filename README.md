# Phoneme confusability Analysis
## Overview of the research project
This project is undertaken as work done for the [Jackman Scholars-in-Residence 2022](https://vic.utoronto.ca/academic-programs/scholars-in-residence) 
research program, supervised by Dr Blair Armstrong at the [Computational and Psycholinguistics Lab](https://www.utsc.utoronto.ca/labs/caplab/personnel/) at the University of Toronto. \
The specific research focuses on improving reading curricula by developing an explicit theory of reading instruction. Broadly, the 
research is undertaken to understand how children learn to read and what kind of teaching helps pre-literate children 
acquire reading rules. 
## Overview of the code
This code aims to find the confusion matrices for different phonemes and correlate how similar such confusion values are
for different datasets. For example, the phonemes /p/ and /b/ are more similar (and hence confusable) than phonemes /p/ and /s/.\
All analysis is conducted in the module `confusion_matrix.py`. \
We have used the Chomsky Halle 1968 and the Harm and Seidenberg 1999 datasets for our analysis,
which can be found in this repository.
## Instructions for running the code
1. It is recommended to install the latest version of Python and standard libraries like NumPy and pandas before interacting with the module.
2. Run the module `confusion_matrix.py`. You can change the input files for which you need to find the confusion matrices and correlation in the `if __name == __main__` section at the end of the module.
3. The results will be outputted to the Results directory. 