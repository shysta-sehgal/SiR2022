# Clear Memory
rm(list=ls())

# load packages
library(tidyr)
library(stringr)
library(caret)
library(text2vec)

#######################  SET ANALYSIS PARAMETERS  #######################
# Set working directory
setwd("PATH_TO_DIRECTORY")

# Name of pattern file: Trial name = *, where in pattern file TAG Word: *. If two trials have the same name (column 1 of results file and * in TAG Word: * of pattern file) then they are referenced by order of the input within the script. However, a trial will be classed as accurate if the results match the target of any trial with the same name. The 'DuplicateTrialNames' column will indicate whether this has occured.
patternFile = "NAME_OF_PATTERN_FILE.pat"

# Name of test results file (output of HS04_Test.c) If two trials have the same name (column 1 of results file and * in TAG Word: * of pattern file) then they are referenced by order in the script. However, a trial will be classed as accurate if the results match the target of any trial with the same name. The 'DuplicateTrialNames' column will indicate whether this has occured.
resultsFile = "NAME_OF_RESULTS_FILE.res"

# Name of outputted analysis file
analysisFile = "NAME_OF_OUTPUT_FILE.ana"

# Name of corpus file (e.g. 6k_AllReps_8Slot_NxF; Assumes same structure of '6k_AllReps_8Slot_NxF' file, number of rows can differ)
corpusFile = "NAME_OF_CORPUS_FILE"

# Name of Phoneme Mapping file (e.g. mapping; Assumes same structure of 'mapping' file)
mappingFile = "mapping"

# Output Layer (1 = Phonology or 2 = Semantic)
outputLayer = 2

# Analysis to Perform:
# 1 = Generate Phoneme slot-based output ONLY (cosine similarity)
# 2 = Compare Phoneme slot-based output to Target in pattern file [Assumes: In pattern file full coding of target for phonology]
# 3 = Compare Phoneme slot-based output to All corpus items [Assumes in corpus file phoneme slot based coding of phonology]
# 4 = Compare full vector to Target and all other Targets in pattern file [Assumes: In pattern file full coding of target for phonology and sparse coding of target for semantics]
# 5 = Compare full vector to All corpus items  [Assumes: In corpus file phoneme slot based coding of phonology and sparse coding of semantics]
# 6 = Compare Phoneme slot-based output to All corpus items & target on test trial [Assumes in corpus file phoneme slot based coding of phonology]
# 7 = Compare full vector to All corpus items & target on test trial  [Assumes: In corpus file phoneme slot based coding of phonology and sparse coding of semantics]

Ana2Per = 7

##########################################################################

#########################  LOAD RELEVANT DATA  ###########################

if(outputLayer == 1){
  
  # Load Phoneme Mappings
  mapping = read.table(mappingFile)
  phonemes = mapping[,1]
  mapping = as.matrix(mapping[,2:26], nrow = dim(mapping)[1])
  mapping[mapping == 0] = 0.01
  rownames(mapping) = phonemes
  colnames(mapping) = 1:25
  
  # Set number of units in Representation
  nFeats = 200
  
}else{
  # Set number of units in Representation
  nFeats = 2446  
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

# Load Pattern File
patterns <- read.table(patternFile,sep = "\n",stringsAsFactors = F)
# -> Extract Trial Name
trialLines = dplyr::filter(patterns, grepl('TAG Word:',V1))
trialNamesPat = unlist(lapply(str_split(trialLines$V1, patter = " "), `[[`, 3))
trialUniquePat = paste(trialNamesPat,1:length(trialNamesPat),sep="_")
# -> Check all trial names from ResultsFile match a trial name from PatternFile
if((sum(as.numeric(trialsNamesRes %in% trialNamesPat)))!=(length(trialsNamesRes))){
  stop("Not all results trials are in pattern file!!!")
}
# -> Extract Target Vector
if((Ana2Per==2)|(Ana2Per==4)|(Ana2Per==6)|(Ana2Per==7)){
  if(outputLayer==2){
    vecLines = which(str_locate(patterns$V1,"TARGET semantic")[,1]==1)
    targetVecs = lapply(lapply(lapply(lapply(str_split(patterns$V1[(vecLines+1)], " "),gsub, pattern = ",", replacement = ""),as.numeric), function(x) x[!is.na(x)]),function(x) x+1)
    targetVecsBin = t(matrix(unlist(lapply(targetVecs,function(x) index2vec(x,nFeats,sign = FALSE))),ncol = length(targetVecs),nrow = nFeats))
    targetVecsBin[targetVecsBin==0] = 0.01
    targetVecsBin[targetVecsBin==1] = 0.99
    rm(targetVecs)
  }else{
    vecLines = which(str_locate(patterns$V1,"TARGET phonology")[,1]==1)
    targetVecsBin = matrix(0,length(trialLines$V1),nFeats)
    for(i in 1:length(trialLines$V1)){
      targetVecsBin[i,] = unlist(lapply(lapply(str_split(patterns$V1[(vecLines[i]+1):(vecLines[i]+8)], " "), as.numeric), function(x) x[!is.na(x)]))
    }
    targetVecsBin[targetVecsBin==0] = 0.01
    targetVecsBin[targetVecsBin==1] = 0.99
  }
  rownames(targetVecsBin) <- trialUniquePat
  rm(patterns, trialLines, vecLines)
}

if((Ana2Per==3)|(Ana2Per>4)){
  # Load Corpus File
  # -> Extract Word Name & Phoneme Rep
  corpusIn <- read.table(corpusFile,sep = "\n",stringsAsFactors = F)
  corpusIn = str_split(corpusIn$V1," ")
  corpus = tibble(Word = unlist(lapply(corpusIn, `[[`, 1)), Freq = as.numeric(unlist(lapply(corpusIn, `[[`, 2))), Phonology = unlist(lapply(corpusIn, `[[`, 6)))
  if(outputLayer==2){
  # -> Extract Semantic Rep
    corpusVecsBin = matrix(0,length(corpusIn),nFeats)
    rownames(corpusVecsBin) <-corpus$Word
    for (i in 1:length(corpusIn)){
      idx = (unlist(lapply(corpusIn[[i]][8:length(corpusIn[[i]])], as.numeric)))+1
      corpusVecsBin[i,idx] = 1
    }
    corpusVecsBin[corpusVecsBin==0] = 0.01
    corpusVecsBin[corpusVecsBin==1] = 0.99    
    rm(idx)
    
  }else if(Ana2Per>4){
    # Generate Phonological Vectors for corpus
    corpusVecsBin = matrix(0,length(corpusIn),nFeats)
    rownames(corpusVecsBin) <-corpus$Word
    for (i in 1:length(corpusIn)){
      wordTable = mapping[unlist(str_split(corpus$Phonology[i],"")),]
      corpusVecsBin[i,1:25] = wordTable[1,]
      corpusVecsBin[i,26:50] = wordTable[2,]
      corpusVecsBin[i,51:75] = wordTable[3,]
      corpusVecsBin[i,76:100] = wordTable[4,]
      corpusVecsBin[i,101:125] = wordTable[5,]
      corpusVecsBin[i,126:150] = wordTable[6,]
      corpusVecsBin[i,151:175] = wordTable[7,]
      corpusVecsBin[i,176:200] = wordTable[8,]
    }
    corpusVecsBin[corpusVecsBin==0] = 0.01
    corpusVecsBin[corpusVecsBin==1] = 0.99    
    rm(wordTable)
  }
  rm(corpusIn)
}


##########################################################################

###########################  PERFORM ANALYSIS  ##########################

if((Ana2Per < 4)|(Ana2Per == 6)){
  #################### PHONEME SLOT BASED REPS  #################
  # Generate slot based phoneme reps from output
  finalTable = tibble(Trial = trialsNamesRes)
  finalTable$PhonologyOutput = ""
  for(i in 1:length(finalTable$Trial)){
    slotVec = matrix(unlist(outVecs[i,]), nrow = 8, ncol = 25, byrow = TRUE)
    colnames(slotVec) <- c(1:25)
    trialDists = sim2(slotVec, mapping, method = "cosine")
    finalTable$PhonologyOutput[i] = paste(colnames(trialDists)[max.col(trialDists)],collapse = "")
  }
  
  if (Ana2Per == 2){
    # Compare slot based phoneme rep to target 
    patPhonology = tibble(Trial = trialNamesPat)
    patPhonology$PhonologyTarget = ""
    for(i in 1:length(trialNamesPat)){
      slotVec = matrix(targetVecsBin[i,], nrow = 8, ncol = 25, byrow = TRUE)
      colnames(slotVec) <- c(1:25)
      trialDists = sim2(slotVec, mapping, method = "cosine")
      patPhonology$PhonologyTarget[i] = paste(colnames(trialDists)[max.col(trialDists)],collapse = "")
    }
    finalTable = merge(finalTable,patPhonology,by.x = "Trial", sort = FALSE)
    finalTable$LevDist = 0
    for(i in 1:length(finalTable$Trial)){
      finalTable$LevDist[i] = adist(finalTable[i,2],finalTable[i,3])
    }  
    finalTable$Accuracy = as.numeric(finalTable$LevDist==0)
    finalTable = rbind(data.frame(Trial = "ARITHMETIC MEAN",PhonologyOutput = "NA", PhonologyTarget = "NA", LevDist = mean(finalTable$LevDist), Accuracy = mean(finalTable$Accuracy)), finalTable)
    rm(patPhonology)
  }else if(Ana2Per == 3){
    # Compare slot based phoneme rep to all corpus reps
    finalTable$MinLevDist = 0
    finalTable$ClosestWordA = ""
    finalTable$ClosestWordB = ""
    finalTable$ClosestWordC = ""
    distMat = adist(as.list(finalTable$PhonologyOutput),as.list(corpus$Phonology))
    for(i in 1:length(finalTable$Trial)){
      finalTable$MinLevDist[i] = min(distMat[i,])
      ClosestWords = corpus$Phonology[distMat[i,]==min(distMat[i,])]
      finalTable$ClosestWordA[i] = ClosestWords[1]
      finalTable$ClosestWordB[i] = ClosestWords[2]
      finalTable$ClosestWordC[i] = ClosestWords[3]
    } 
    
  }else if(Ana2Per==6){
    # Compare slot based phoneme rep to target & corpus
    patPhonology = tibble(Trial = trialNamesPat)
    patPhonology$PhonologyTarget = ""
    for(i in 1:length(trialNamesPat)){
      slotVec = matrix(targetVecsBin[i,], nrow = 8, ncol = 25, byrow = TRUE)
      colnames(slotVec) <- c(1:25)
      trialDists = sim2(slotVec, mapping, method = "cosine")
      patPhonology$PhonologyTarget[i] = paste(colnames(trialDists)[max.col(trialDists)],collapse = "")
    }
    finalTable = merge(finalTable,patPhonology,by.x = "Trial", sort = FALSE)
    finalTable$LevDist2Target = 0
    for(i in 1:length(finalTable$Trial)){
      finalTable$LevDist2Target[i] = adist(finalTable[i,2],finalTable[i,3])
    }  
    finalTable$TargetAccuracy = as.numeric(finalTable$LevDist2Target==0)
    finalTable$LevDist2CorpusItem = 0
    finalTable$ClosestCorpusWordA = ""
    finalTable$ClosestCorpusWordB = ""
    finalTable$ClosestCorpusWordC = ""
    distMat = adist(as.list(finalTable$PhonologyOutput),as.list(corpus$Phonology))
    for(i in 1:length(finalTable$Trial)){
      finalTable$LevDist2CorpusItem[i] = min(distMat[i,])
      ClosestWords = corpus$Phonology[distMat[i,]==min(distMat[i,])]
      finalTable$ClosestCorpusWordA[i] = ClosestWords[1]
      finalTable$ClosestCorpusWordB[i] = ClosestWords[2]
      finalTable$ClosestCorpusWordC[i] = ClosestWords[3]
    } 
    finalTable = rbind(data.frame(Trial = "ARITHMETIC MEAN",PhonologyOutput = "NA", PhonologyTarget = "NA", LevDist2Target = mean(finalTable$LevDist2Target), TargetAccuracy = mean(finalTable$TargetAccuracy), LevDist2CorpusItem = mean(finalTable$LevDist2CorpusItem), ClosestCorpusWordA = "NA", ClosestCorpusWordB  = "NA", ClosestCorpusWordC = "NA"), finalTable)
    rm(patPhonology,distMat,slotVec)
  }

}else{
  ####################  COMPARE FULL VECTORS  ###################
  
  if (Ana2Per ==4){
    # Compare full output vector to target vector
    finalTable = tibble(Trial = trialsNamesRes)
    finalTable$Distance2Target = 0
    finalTable$Distance2Closest = 0
    finalTable$ClosestItemAmongstTargetPatterns = ""
    finalTable$DuplicateTrialNames = 0
    finalTable$Accuracy = 0
    for(i in 1:length(trialsNamesRes)){
      distMat = sim2(as.matrix(outVecs[i,]), as.matrix(targetVecsBin), method = "cosine")
      finalTable$Distance2Target[i] = max(distMat[1,trialNamesPat==trialsNamesRes[i]])
      if(sum(as.numeric(trialNamesPat==trialsNamesRes[i]))>1){
        finalTable$DuplicateTrialNames[i] = 1
      }
      finalTable$Distance2Closest[i] = max(distMat)
      finalTable$ClosestItemAmongstTargetPatterns[i] = trialNamesPat[distMat==max(distMat)][1]
    }
    finalTable$Accuracy = as.numeric(finalTable$Distance2Target==finalTable$Distance2Closest)
    finalTable = rbind(data.frame(Trial = "ARITHMETIC MEAN", Distance2Target = mean(finalTable$Distance2Target), Distance2Closest = mean(finalTable$Distance2Closest), ClosestItemAmongstTargetPatterns = "NA",DuplicateTrialNames = "NA", Accuracy = mean(finalTable$Accuracy)), finalTable)
    
  }else{# Compare full output vector to all corpus reps (& target)
    finalTable = tibble(Trial = trialsNamesRes)
    finalTable$DuplicateTrialNames = 0
    if(Ana2Per==7){# Condition in which compare to target in addition to whole corpus
      # Compare Output to Target
      finalTable$Distance2Target = 0
      for(i in 1:length(trialsNamesRes)){
        distMat = sim2(as.matrix(outVecs[i,]), as.matrix(targetVecsBin), method = "cosine")
        finalTable$Distance2Target[i] = max(distMat[1,trialNamesPat==trialsNamesRes[i]])
        if(sum(as.numeric(trialNamesPat==trialsNamesRes[i]))>1){
          finalTable$DuplicateTrialNames[i] = 1
        }
      }
    }
    # Compare Output to all Vectors in corpus
    finalTable$Distance2ClosestItemInCorpus = 0
    finalTable$ClosestItemInCorpus = ""
    if(Ana2Per!=7){
      finalTable$TrialNameIsClosest = 0
    }
    for(i in 1:length(trialsNamesRes)){
      distMat = sim2(as.matrix(outVecs[i,]), as.matrix(corpusVecsBin), method = "cosine")
      finalTable$Distance2ClosestItemInCorpus[i] = max(distMat)
      finalTable$ClosestItemInCorpus[i] = corpus$Word[distMat==max(distMat)][1]
      if(Ana2Per!=7){
        finalTable$TrialNameIsClosest[i] = as.numeric(finalTable$Trial[i] %in% corpus$Word[distMat==max(distMat)])
      }
    }
    if(Ana2Per==7){
      finalTable$TargetIsClosest = as.numeric(finalTable$Distance2Target>=finalTable$Distance2ClosestItemInCorpus)
      finalTable = rbind(data.frame(Trial = "ARITHMETIC MEAN", DuplicateTrialNames = "NA", Distance2Target = mean(finalTable$Distance2Target), Distance2ClosestItemInCorpus = mean(finalTable$Distance2ClosestItemInCorpus), ClosestItemInCorpus = "NA", TargetIsClosest = mean(finalTable$TargetIsClosest)), finalTable)
    }else{
      finalTable = rbind(data.frame(Trial = "ARITHMETIC MEAN", DuplicateTrialNames = "NA", Distance2ClosestItemInCorpus = mean(finalTable$Distance2ClosestItemInCorpus), ClosestItemInCorpus = "NA", TrialNameIsClosest = mean(finalTable$TrialNameIsClosest)), finalTable)
    }
  }
  
}

# Write results of analysis to analysiFile
write.table(finalTable, file = analysisFile, sep = "\t", col.names = TRUE, row.names = TRUE,quote = FALSE)
