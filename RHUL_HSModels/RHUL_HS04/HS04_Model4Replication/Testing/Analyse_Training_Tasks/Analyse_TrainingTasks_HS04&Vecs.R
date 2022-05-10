# Set working directory
# setwd("...")

# Clear Memory
rm(list=ls())

# load packages
library(stringr)
library(devtools)
library(text2vec)
library(gdata)

# Name of results file (output of HS04_Test.c)
resultsFile = ""

# Name of output file
outputFile = ""

# Analysis method: 1 = Semantic layer comparing full vectors, 2 = Phonological layer comparing full vectors, 3 = Phonological layer comparing phoneme slots, 4 = Phonological method in HS04, 5 = Semantic menthod in HS04
taskType = 1


# Fixed Variables
corpSize = 6229
corp2Skip = 0
if((taskType==1)|(taskType==5)){
  nFeats = 2446  
}else{
  nFeats = 200
}


# Load corpus
if((taskType==1)|(taskType==5)){
  dict = read.table("semBin.vecs", skip = corp2Skip)
}else{
  dict = read.table("phoPBin.vecs", skip = corp2Skip)
  mapping = read.table("mapping")
  
  # Load Phoneme Mappings
  phonemes = mapping[,1]
  mapping = as.matrix(mapping[,2:26], nrow = dim(mapping)[1])
  mapping[mapping == 0] = 0.01
  rownames(mapping) = phonemes
  colnames(mapping) = 1:25
}

# Load Corpus
if((taskType==3)|(taskType==4)|(taskType==2)){
  colnames(dict) <- c("word","phonology")
  dictBin = dict[,3:(nFeats+2)]
  dictBin[dictBin==0] = 0.01
  dictBin[dictBin==1] = 0.99
  dict[,3:(nFeats+2)] = NULL
  dictBin = as.matrix(dictBin, nrow = dim(dict)[1])
  rownames(dictBin) <- dict[,1]
  colnames(dictBin) <- c(1:nFeats)
}else{
  colnames(dict) <- c("word")
  dictBin = dict[,2:(nFeats+1)]
  dictBin[dictBin==0] = 0.01
  dictBin[dictBin==1] = 0.99
  dict[,2:(nFeats+1)] = NULL
  dictBin = as.matrix(dictBin, nrow = dim(dict)[1])
  rownames(dictBin) <- dict[,1]
  colnames(dictBin) <- c(1:nFeats)
}

# Load results
testRes = read.table(resultsFile, skip = corp2Skip+1)
colnames(testRes) <- c("word")
resBin = testRes[,2:(nFeats+1)]
testRes[,2:(nFeats+1)] = NULL
resBin = as.matrix(resBin, nrow = dim(testRes)[1])
resBin[resBin <= 0.01] = 0.01
rownames(resBin) <- testRes[,1]
colnames(resBin) <- c(1:nFeats)

# Select analysis method

# Task type 3 or 4
if((taskType==3)|(taskType==4)){
  # Compare vectors (slotted phonemes)
  outTable = testRes
  outTable[,2] = ""
  outTable[,3] = ""
  outTable[,4] = 0
  outTable[,5] = 0
  colnames(outTable) <- c("word", "Target", "Output", "Acc", "LevDist")
  for(trial in 1:dim(outTable)[1]){
    slotVec = matrix(resBin[trial,], nrow = 8, ncol = 25, byrow = TRUE)
    colnames(slotVec) <- c(1:25)
    if(taskType==3){
      trialDists = sim2(slotVec, mapping, method = "cosine")
      outTable[trial,3] = paste(colnames(trialDists)[max.col(trialDists)],collapse = "")
    }else{
      trialDists = dist2(slotVec, mapping, method = "euclidean")
      outTable[trial,3] = paste(rownames(mapping)[max.col(-trialDists)],collapse = "")
    }
    outTable[trial,2] = substr(dict[dict[,1]==outTable[trial,1],2][1],1,8)
    outTable[trial,4] = outTable[trial,3]==outTable[trial,2]
    outTable[trial,5] = adist(outTable[trial,3], outTable[trial,2])
  }
  acc = mean(outTable[,4])
  outTable = rbind(data.frame(word = "ARITHMETIC MEAN", Target = "NA", Output = "NA", Acc = mean(outTable[,4]), LevDist = mean(outTable[,5])), outTable)

# Task type 5
}else if(taskType==5){
  resBin[resBin < 0.5] = 0.01
  resBin[resBin >= 0.5] = 0.99
  # Compare network output to target
  accVec = rowSums(resBin==dictBin)==nFeats
  acc = mean(accVec)
  outTable = cbind(testRes, accVec)
  outTable = rbind(data.frame(word = "ARITHMETIC MEAN", accVec = mean(accVec)), outTable)
  
# Task type 1 or 2
}else{
  # Compare vectors (full vectors)
  resVdict = sim2(resBin, dictBin, method = "cosine")
  maxD = apply(resVdict,1,FUN=max)
  wordD = diag(resVdict)
  accVec = maxD==wordD
  acc = mean(accVec)
  outTable = cbind(testRes, wordD, maxD, accVec)
  outTable = rbind(data.frame(word = "ARITHMETIC MEAN", wordD = mean(wordD), maxD = mean(maxD), accVec = mean(accVec)), outTable)
}


# Output Results
if(sum(dict[,1]==testRes)!=nrow(testRes)){
  print("!!!!!!!!!!!!! ROWS OF DICTIONARY AND TEST TRIALS DO NOT MATCH !!!!!!!!!!!")
}else{
  # acc
  write.table(outTable, file = outputFile, sep = "\t", col.names = TRUE, row.names = TRUE)
}
