# Clear Memory
rm(list=ls())

# load packages
library(tidyr)
library(stringr)
library(caret)
library(text2vec)

#######################  SET ANALYSIS PARAMETERS  #######################
# Set working directory
setwd("Path_to_Files")

# Name of test results file (output of HS04_Test.c) If two trials have the same name (column 1 of results file and * in TAG Word: * of pattern file) then they are referenced by order in the script. 
resultsFile = "RS_sphidden.res"

# Name of outputted analysis file
analysisFile = "RS_sphidden.ana"

# Layer (1 = oshidden, 2 = ophidden, 3 = pshidden, 4 = sphidden)
layer = 4


##########################################################################

#########################  LOAD RELEVANT DATA  ###########################

if(layer == 2){
  
  # Set number of units
  nFeats = 200
  
}else{
  # Set number of units
  nFeats = 500  
}

# Load Results File
testRes = read.table(resultsFile, skip = 1)
# -> Extract Trial Names
trialsNamesRes = testRes[,1]
trialsUniqueRes = paste(trialsNamesRes,1:length(trialsNamesRes),sep="_")
# -> Extract Output Vectors
outVecs = testRes[,-1]
outVecs[outVecs <= 0.01] = 0.01
rownames(outVecs) <- trialsUniqueRes
colnames(outVecs) <- c(1:nFeats)
rm(testRes)


##########################################################################

###########################  PERFORM ANALYSIS  ##########################

distMat = sim2(as.matrix(outVecs), as.matrix(outVecs), method = "cosine")

# Write results of analysis to analysiFile
write.table(distMat, file = analysisFile, sep = "\t", col.names = TRUE, row.names = TRUE,quote = FALSE)
