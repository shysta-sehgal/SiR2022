# Phoneme confusability Analysis
## Overview of the code
This code aims to find the confusion matrices for different phonemes and correlate how similar such confusion values are
for different datasets. For example, the phonemes /p/ and /b/ are more similar (and hence confusable) than phonemes /p/ and /s/.\
All analysis is conducted in the module `confusion_matrix.py`. \
We have used the Chomsky Halle 1968, the Harm and Seidenberg 1999, and other datasets for our analysis,
which can be found in this folder.
## Instructions for running the code
1. It is recommended to install the latest version of Python and standard libraries like NumPy and pandas before interacting with the module.
2. Run the module `confusion_matrix.py`. You can change the input files for which you need to find the confusion matrices and correlation in the `if __name == __main__` section at the end of the module.
3. The results will be outputted to the Results directory. 