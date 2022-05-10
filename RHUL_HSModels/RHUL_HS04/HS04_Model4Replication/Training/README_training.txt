HS04_Model4Replication_0521: Training

This folder contains the pattern files and C script required to train a single instantiation of the triangle model of reading in Mikenet for replication of HS04. Pattern files contain representations taken from 6k_AllReps_8Slot_NxF and are identical to those found in Chang, Monaghan & Welbourne, 2019. The training procedure closely follows that reported in Harm & Seidenberg, 2004 (HS04). As in HS04 the learning rate begins at 0.2, and is reduced to 0.1 after 500000 trials. The network is first trained on illiterate training tasks (i.e. phonology to phonology = 10%, semantics to semantics = 10%, phonology to semantics = 40%, semantics to phonology 40%) using the same frequencies as used in HS04. In a departure from HS04 the network is trained on these initial training tasks for 3 million trials, before performing a further 1 million trials that include literacy training tasks (orthography to semantics & phonology = 40%) which are randomly interleaved with illiterate tasks. Freezing weights as in HS04 did not lead to sufficiently high performance on the pre-literate tasks. The script outputs the error on each task every 5000 trials and outputs the trained weights to a file 'saveweights_read_TRIALNUMBER.gz' every 100000 trials. Weight changes are made following each trial using the recurrent backpropagation through time learning algorithm (see crbp.c in Mikenet).

Before compiling or running the code ensure Mikenet is recorded in the environment variables using the code: 
- For Macs: export MIKENET_DIR=~/Mikenet-v8.0
- Linux: first enter the tcsh environment: > tcsh; then set the environment variable using: > setenv MIKENET_DIR ${HOME}/Mikenet-v8.0

To compile the c file use the command: make

To train the network use the command: ./HS04_Train -seed 10 > NAME_OF_ERROR_REPORT_FILE

In the above command the seed can be any number. The error report file will be updated every 5000 trials with the networks current performance not each training task.