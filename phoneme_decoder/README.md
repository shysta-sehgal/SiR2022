# Word Decoder
## Overview of the code
This code aims to decode words into International Phonetics Alphabet transcription and check the accuracy of rules taught in certain curriculum with the rules taught in another to see what curriculum might be better for learning of the students.  
Decoding is primarily done in `decoder.py` and the analysis for accuracy of different curricula is conducted in `curricula_analysis.py`
We have used Jolly Phonics and Letters and Sounds curricula, which can be found in this folder.
## Instructions for running the code
1. It is recommended to install the latest version of Python and standard libraries like NumPy and pandas before interacting with the module.
2. Run the module `curricula_analysis.py`. You can change the input files for which you need to find the accuracy for in the `if __name == __main__` section at the end of the module.
3. The results can be viewed through a simple print statement. 