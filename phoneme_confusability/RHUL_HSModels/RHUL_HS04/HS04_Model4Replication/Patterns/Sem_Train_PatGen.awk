{
	name[NR] = $1;
	orth[NR] = $3;
	pho[NR] = $4;
	freq[NR] = $2;
	noFeat[NR] = NF - 4;
	for(i=5;i<=NF;i++)sem[NR,i-4]=$i;
}  

END{
	for(i=1;i<=NR;i++){
		print "TAG","Word:",name[i];
		print "PROB", freq[i];
		print "CLAMP semantic 0-6 SPARSE";
		for(j=1;j<=noFeat[i];j++){
			printf sem[i,j] " ";
		}		
		printf ",\n"
		print "TARGET semantic 7-ALL SPARSE";
		for(j=1;j<=noFeat[i];j++){
			printf sem[i,j] " ";
		}
		printf ",\n;\n";
	}
}
