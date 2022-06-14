function printPhon(phon){
	if(phon=="_")print -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 " ";
	if(phon=="p")print -1,1,-1,-1,1,1,0,-1,1,0,0 " ";
	if(phon=="b")print -1,1,0,-1,1,1,0,-1,1,0,0 " ";
	if(phon=="t")print -1,1,-1,-1,1,-1,1,-1,-1,1,0 " ";
	if(phon=="d")print -1,1,0,-1,1,-1,1,-1,-1,1,0 " ";
	if(phon=="k")print -1,1,-1,-1,1,-1,-1,-1,-1,-1,0 " ";
	if(phon=="g")print -1,1,0,-1,1,-1,-1,-1,-1,-1,0 " ";
	if(phon=="f")print -0.5,1,-1,-1,0,-1,1,-1,1,0,0 " ";
	if(phon=="v")print -0.5,1,0,-1,0,-1,1,-1,1,0,0 " ";
	if(phon=="T")print -0.5,1,-1,-1,0,-1,1,-1,-1,0,0 " ";
	if(phon=="D")print -0.5,1,0,-1,0,-1,1,-1,-1,0,0 " ";
	if(phon=="s")print -0.5,1,-1,-1,0,-1,1,-1,-1,1,0 " ";
	if(phon=="z")print -0.5,1,0,-1,0,-1,1,-1,-1,1,0 " ";
	if(phon=="S")print -0.5,1,-1,-1,0,-1,0,-1,-1,0,0 " ";
	if(phon=="Z")print -0.5,1,0,-1,0,-1,0,-1,-1,0,0 " ";
	if(phon=="h")print -0.5,1,0,-1,0,-1,-1,1,-1,-1,-1 " ";
	if(phon=="C")print -0.8,1,-1,-1,1,-1,0,-1,-1,0,0 " ";
	if(phon=="J")print -0.8,1,0,-1,1,-1,0,-1,-1,0,0 " ";
	if(phon=="m")print 0,0,1,1,1,1,0,-1,1,0,0 " ";
	if(phon=="n")print 0,0,1,1,1,-1,1,-1,-1,1,0 " ";
	if(phon=="l")print 0.5,0,1,0,-1,-1,1,-1,-1,1,0 " ";
	if(phon=="r")print 0.5,0,1,0,-1,-1,-1,1,1,-1,-1 " ";
	if(phon=="w")print 0.8,0,1,0,0,1,-1,-1,1,-1,0 " ";
	if(phon=="y")print 0.8,0,1,0,0,-1,0,-1,-1,0,1 " ";
	if(phon=="i")print 1,-1,1,0,0,-1,0,-1,-1,0,1 " ";
	if(phon=="I")print 1,-1,1,0,0,-1,0,-1,-1,0,-1 " ";
	if(phon=="e")print 1,-1,1,0,-1,-1,0,-1,-1,-1,1 " ";
	if(phon=="E")print 1,-1,1,0,-1,-1,0,-1,-1,-1,-1 " ";
	if(phon=="a")print 1,-1,1,0,-1,-1,-1,1,-1,-1,-1 " ";
	if(phon=="@")print 1,-1,1,0,-1,-1,0,1,-1,-1,1 " ";
	if(phon=="u")print 1,-1,1,0,0,-1,-1,-1,1,0,1 " ";
	if(phon=="U")print 1,-1,1,0,0,-1,-1,-1,1,0,-1 " ";
	if(phon=="o")print 1,-1,1,0,-1,-1,-1,-1,1,-1,1 " ";
	if(phon=="^")print 1,-1,1,0,-1,-1,-1,-1,-1,-1,-1 " ";
	if(phon=="c")print 1,-1,1,0,-1,-1,-1,-1,1,-1,-1 " ";
}

function getorth(letter){
  if(letter=="_") print 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 " ";
  if(letter=="a") print 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 " ";
  if(letter=="b") print 0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 " ";
  if(letter=="c") print 0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 " ";
  if(letter=="d") print 0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 " ";
  if(letter=="e") print 0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 " ";
  if(letter=="f") print 0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 " ";
  if(letter=="g") print 0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 " ";
  if(letter=="h") print 0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 " ";
  if(letter=="i") print 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 " ";
  if(letter=="j") print 0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 " ";
  if(letter=="k") print 0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 " ";
  if(letter=="l") print 0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0 " ";
  if(letter=="m") print 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0 " ";
  if(letter=="n") print 0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 " ";
  if(letter=="o") print 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0 " ";
  if(letter=="p") print 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0 " ";
  if(letter=="q") print 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0 " ";
  if(letter=="r") print 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0 " ";
  if(letter=="s") print 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0 " ";
  if(letter=="t") print 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0 " ";
  if(letter=="u") print 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0 " ";
  if(letter=="v") print 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0 " ";
  if(letter=="w") print 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0 " ";
  if(letter=="x") print 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0 " ";
  if(letter=="y") print 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0 " ";
  if(letter=="z") print 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 " ";
}

function vowel(pho){
	ret = 0;
	if(pho=="i"||pho=="I"||pho=="e"||pho=="E"||pho=="a"||pho=="@"||pho=="u"||pho=="U"||pho=="o"||pho=="^"||pho=="c"){ ret = 1;
	}
	return ret;
}

{
	len = length($3);
	firstV = -1;
	conV = 0;
	fil = "_";
	
	#Get necessary variables
	for(i=1;i<=len;i++){
		if((firstV==-1)&&(vowel(substr($3,i,1))==1)){
			firstV = i;
		}
		if((firstV==(i-1))&&(vowel(substr($3,i,1))==1)){
			conV = 1;
		}
	}
	
	print "TAG","Word:",$1,"Pho:",$3,"Ortho",$2;
	printf "PROB %.2f\n",$4;
	print "CLAMP Ortho ALL FULL";
	for(i=2;i<=11;i++) getorth( substr($2,i,1) );
	print "\nTARGET Phono 2-4 FULL";
  
		#Print any initial blank slots
	for(i=1;i<=(3-firstV);i++){
		printPhon(fil);
	}
	
	#Print letters up to vowel & Vowel
	for(i=1;i<firstV;i++){
		printPhon(substr($3,i,1));
	}
	printPhon(substr($3,firstV,1));
	
	#Print additional vowels or blank
	if(conV==1)
		printPhon(substr($3,firstV+1,1));
	else
		printPhon(fil);
	
	#Print final phonemes
	coda = len-firstV-conV;
	for(i=1;i<=coda;i++){
		printPhon(substr($3,firstV+conV+i,1));
	}
	
	#Print any final blanck slots
	for(i=1;i<=(6-4-coda);i++){
		printPhon(fil);
	}

    print ";";
}