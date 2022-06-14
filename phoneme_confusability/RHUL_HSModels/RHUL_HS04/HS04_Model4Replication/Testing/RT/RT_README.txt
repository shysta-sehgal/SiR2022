HS04_Test_RT.c

This script generates settling times for HS04 networks [as defined in README_HS04_M4R.txt] performing phonological and/or semantic mapping tasks. The script builds a network, imports weights [weights] into the given network, tests the network on a set of predefined test trials [patterns] and outputs the time it takes for the network's phonological or semantic layer [layer] to settle on it's given output on each trial in the test set. Settling time is taken to be the number of time steps [0 -> noSteps-1] that pass from the given initial time step (defined by startTS) until the change in activation between time steps in each unit's activation in the given output layer is below a pre-defined threshold (setThresh). For example should the settling time result be 7, with startTS = 1 and setThresh = 0.1, this means that for no unit in the output layer the change in activation between time steps 7 and 8 exceeded 0.1.

The script can be run using the following line of code (replacing * with the relevant variables):
./HS04_Test_RT -seed * -weights * -patterns * -task * -layer * -setThresh * -startTs * -noSteps * > NAME_OF_OUPUT_FILE

Where:
- seed = random seed integer
- weights = name of weight file (output from HS04_Train.c)
- patterns = name of pattern file containing set of test trials
- task = mapping task the network should perform during the test trials (p2p = phonology to phonology; s2s = semantic to semantic; p2s = phonology to semantic; s2p = semantic to phonology; o2ps = orthography to semantics & phonology)
- layer = the layer for which settling times should be calculated (semantic or phonology)
- setThresh = value indicating level of activation change between time steps under which network classed as settled (default = 0.1)
- startTs = time step from which point the settling time is calculated (e.g. if target necessarily clamped for longer, then can move start time so this period is not included in the settling time) [default = 1].
- noSteps = total number of time steps the network should run for on each trial, this MUST BE A MULTIPLE OF 4!!!! (default = 12)

The script outputs information in the following format. Each row reports the results of a single trial as defined in the pattern file. Columns are organised as follows:
- Trial:The trial name as defined in the pattern file
- Layer: the name of the layer for which the settling time has been calculated
- MaxUnitChange: the maximum change in unit activation within the output layer on the time step at which the network was classified as settled (if the network does not settle before the end of the trial this will be the maximum activation change on the final time step)
- RT: the settling time, the number of time steps between startTs and the time step at which the difference in unit activation between time steps does not exceed setThresh. If the network does not settle before the end of the trial this is the difference between the startTs and the final time step of the trial
- Unit_Activation: the activation of each unit in the output layer on the time step at which the network has settled, if the network does not settle before the end of the trial then this is the output at the end of the trial (e.g. this can be used to check whether the network is producing the target output).
