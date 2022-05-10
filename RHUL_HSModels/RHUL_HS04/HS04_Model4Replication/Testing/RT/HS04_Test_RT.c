#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <mikenet/simulator.h>

#define SAMPLES 12
#define TICKS SAMPLES
#define SECONDS 4
#define RANDOM_NUMBER_RANGE 0.1

Net *read, *illit;
Group *phonology, *orthography, *semantic, *bias, *semCU, *phoCU, *oshidden, *ophidden, *comphidden, *prodhidden;
Connections *sem2cu, *cu2sem, *sem2prodhidden, *prodhidden2pho, *pho2cu, *cu2pho, *pho2comphidden, *comphidden2sem, *orth2oshidden, *oshidden2sem, *orth2ophidden, *ophidden2pho, *bias2sem, *bias2semCU, *bias2prodhidden, *bias2pho, *bias2phoCU, *bias2comphidden, *bias2orth, *bias2oshidden, *bias2ophidden, *orth2sem, *orth2pho;
ExampleSet *test_trials;

float random_number_range=RANDOM_NUMBER_RANGE;

// Method for calculating difference between given vectors
float get_Diff(Real *vec1, Real *vec2, int units){
	float diffMax = 0;
	float diff = 0;
	int i;
	for(i=0;i<units;i++){
		diff = fabs(vec1[i]-vec2[i]);
		if(diff>diffMax){diffMax = diff;}
	}
	return diffMax;
}

// Method for printing layer activation
void print_Layer(Real *vec, int units){
	int i;
	for(i=0;i<units;i++){
		printf("%f\t",vec[i]);
	}
}

//Method for extracting trial name
int get_word(char *tag, char *name)
{
  char *p;
  p=strstr(tag,"Word:");
  p+= 5;
  p=strtok(p," \t\n\r");
  strcpy(name,p);
}

// Builds HS04 network
void build_reading_model(int noSteps)
{

  int i,j;
  float range;

  range = random_number_range;
  default_errorRamp=RAMP_ERROR;
  default_tai=1;

  default_activationType=LOGISTIC_ACTIVATION;
  default_errorComputation=CROSS_ENTROPY_ERROR;
  default_errorRadius=0.1;

  default_scaling=SCALE_PROB;

  /* Build a network (and sub-networks), with noSteps number of time noSteps */
  read=create_net(noSteps);
  illit=create_net(noSteps);
  
  read->integrationConstant=(float)SECONDS/(float)noSteps;
  illit->integrationConstant=(float)SECONDS/(float)noSteps;
  
  // Define layers within network
  phonology=init_group("phonology",200,noSteps);
  orthography=init_group("orthography",364,noSteps);
  semantic=init_group("semantic",2446,noSteps);
  semCU=init_group("semCU",100,noSteps);
  phoCU=init_group("phoCU",50,noSteps);
  oshidden=init_group("OSHidden",500,noSteps);
  ophidden=init_group("OPHidden",200,noSteps);
  comphidden=init_group("CompHidden",500,noSteps);
  prodhidden=init_group("ProdHidden",500,noSteps);
  bias=init_bias(1.0,noSteps);

  /* Add our groups to the network object */
  bind_group_to_net(read,phonology);
  bind_group_to_net(read,orthography);
  bind_group_to_net(read,semantic);
  bind_group_to_net(read,semCU);
  bind_group_to_net(read,phoCU);
  bind_group_to_net(read,oshidden);
  bind_group_to_net(read,ophidden);
  bind_group_to_net(read,comphidden);
  bind_group_to_net(read,prodhidden);
  bind_group_to_net(read,bias);

  bind_group_to_net(illit,phonology);
  bind_group_to_net(illit,semantic);
  bind_group_to_net(illit,semCU);
  bind_group_to_net(illit,phoCU);
  bind_group_to_net(illit,comphidden);
  bind_group_to_net(illit,prodhidden);
  bind_group_to_net(illit,bias);
  

  /* Connect groups */
  sem2cu=connect_groups(semantic,semCU);
  cu2sem=connect_groups(semCU,semantic);
  sem2prodhidden=connect_groups(semantic,prodhidden);
  prodhidden2pho=connect_groups(prodhidden,phonology);
  pho2cu=connect_groups(phonology,phoCU);
  cu2pho=connect_groups(phoCU,phonology);
  pho2comphidden=connect_groups(phonology,comphidden);
  comphidden2sem=connect_groups(comphidden,semantic);
  orth2oshidden=connect_groups(orthography,oshidden);
  orth2ophidden=connect_groups(orthography,ophidden);
  oshidden2sem=connect_groups(oshidden,semantic);
  ophidden2pho=connect_groups(ophidden,phonology);
  bias2sem=connect_groups(bias,semantic);
  bias2semCU=connect_groups(bias,semCU);
  bias2prodhidden=connect_groups(bias,prodhidden);
  bias2pho=connect_groups(bias,phonology);
  bias2phoCU=connect_groups(bias,phoCU);
  bias2comphidden=connect_groups(bias,comphidden);
  bias2orth=connect_groups(bias,orthography);
  bias2oshidden=connect_groups(bias,oshidden);
  bias2ophidden=connect_groups(bias,ophidden);
  orth2sem=connect_groups(orthography,semantic);
  orth2pho=connect_groups(orthography,phonology);

  // Add connections to the network
  bind_connection_to_net(read,sem2cu);
  bind_connection_to_net(read,cu2sem);
  bind_connection_to_net(read,sem2prodhidden);
  bind_connection_to_net(read,prodhidden2pho);
  bind_connection_to_net(read,pho2cu);
  bind_connection_to_net(read,cu2pho);
  bind_connection_to_net(read,pho2comphidden);
  bind_connection_to_net(read,comphidden2sem);
  bind_connection_to_net(read,orth2oshidden);
  bind_connection_to_net(read,orth2ophidden);
  bind_connection_to_net(read,oshidden2sem);
  bind_connection_to_net(read,ophidden2pho);
  bind_connection_to_net(read,bias2sem);
  bind_connection_to_net(read,bias2semCU);
  bind_connection_to_net(read,bias2prodhidden);
  bind_connection_to_net(read,bias2pho);
  bind_connection_to_net(read,bias2phoCU);
  bind_connection_to_net(read,bias2comphidden);
  bind_connection_to_net(read,bias2orth);
  bind_connection_to_net(read,bias2oshidden);
  bind_connection_to_net(read,bias2ophidden);
  bind_connection_to_net(read,orth2sem);
  bind_connection_to_net(read,orth2pho);

  bind_connection_to_net(illit,sem2cu);
  bind_connection_to_net(illit,cu2sem);
  bind_connection_to_net(illit,sem2prodhidden);
  bind_connection_to_net(illit,prodhidden2pho);
  bind_connection_to_net(illit,pho2cu);
  bind_connection_to_net(illit,cu2pho);
  bind_connection_to_net(illit,pho2comphidden);
  bind_connection_to_net(illit,comphidden2sem);
  bind_connection_to_net(illit,bias2sem);
  bind_connection_to_net(illit,bias2semCU);
  bind_connection_to_net(illit,bias2prodhidden);
  bind_connection_to_net(illit,bias2pho);
  bind_connection_to_net(illit,bias2phoCU);
  bind_connection_to_net(illit,bias2comphidden);

  // Randomise connection weights
  randomize_connections(sem2cu,1.0);
  randomize_connections(cu2sem,1.0);
  randomize_connections(sem2prodhidden,1.0);
  randomize_connections(prodhidden2pho,1.0);
  randomize_connections(pho2cu,1.0);
  randomize_connections(cu2pho,1.0);
  randomize_connections(pho2comphidden,1.0);
  randomize_connections(comphidden2sem,1.0);
  randomize_connections(orth2oshidden,1.0);
  randomize_connections(orth2ophidden,1.0);
  randomize_connections(oshidden2sem,1.0);
  randomize_connections(ophidden2pho,1.0);
  randomize_connections(bias2sem,1.0);
  randomize_connections(bias2semCU,1.0);
  randomize_connections(bias2prodhidden,1.0);
  randomize_connections(bias2pho,1.0);
  randomize_connections(bias2phoCU,1.0);
  randomize_connections(bias2comphidden,1.0);
  randomize_connections(bias2orth,1.0);
  randomize_connections(bias2oshidden,1.0);
  randomize_connections(bias2ophidden,1.0);
  randomize_connections(orth2sem,1.0);
  randomize_connections(orth2pho,1.0);

  return;
}

// Main method for RT Test Script
int main(int argc,char *argv[])
{
  char cmd[255];
  Example *ex;
  float pherr=0, serr=0, cerr=0, prerr=0, oerr=0, sw, diffMax, setThresh=0.1;
  int seed=1,iters=1, phtrials=1, strials=1, ctrials=1, prtrials=1, otrials=1,startTs=1,noSteps=12;
  int i,j,k,r,phonology_units,orthography_units,semantic_units;
  char  loadFile[255],fn[255],testTask[255];
  char *weightFile=NULL,*patternFile=NULL,*layer2print=NULL;
  float prob[10000][14];
  char name[40];
  FILE *f,*lf;

  setbuf(stdout,NULL);

  loadFile[0]=0;

  // Extract command line arguements
  for(i=1;i<argc;i++)
    {
      if (strcmp(argv[i],"-seed")==0)
	{
	  seed=atoi(argv[i+1]);
	  i++;
	}else if (strcmp(argv[i],"-weights")==0){
		weightFile=argv[i+1];
		i++;
	}else if (strcmp(argv[i],"-patterns")==0){
		patternFile=argv[i+1];
		i++;
	}else if (strcmp(argv[i],"-task")==0){
		strcpy(testTask,argv[i+1]);
		i++;
	}else if (strcmp(argv[i],"-layer")==0){
		layer2print=argv[i+1];
		i++;
	}else if (strncmp(argv[i],"-setThresh",5)==0){
	  setThresh=atof(argv[i+1]);
	  i++;
	}else if(strcmp(argv[i],"-startTs")==0){
	  startTs=atoi(argv[i+1]);
	  i++;
	}else if(strcmp(argv[i],"-noSteps")==0){
	  noSteps=atoi(argv[i+1]);
	  i++;
	}else{
	  fprintf(stderr,"unknown argument: %s\n",argv[i]);
	  exit(1);
	}
    }


  default_epsilon=0.05;

  mikenet_set_seed(seed);
  srand(seed);
  
  // Build HS04 model
  //printf("building model ...\n");
  phonology_units = 200;
  semantic_units = 2446;
  build_reading_model(noSteps);
  load_weights(read,weightFile);

  // Load test examples/trials
  //printf("loading test trials ...\n");
  test_trials=load_examples(patternFile,noSteps);

  // Run test trials
  //printf("running test trials ...\n");
  printf("Trial, Layer, MaxUnitChange, RT, Unit_Activation\n");
  for(i=0;i<test_trials->numExamples;i++){
		ex=&test_trials->examples[i];
		get_word(ex->name,name);
		printf("%s\t",name);
		printf("%s\t",layer2print);
		if((strcmp(testTask,"p2p")==0)){
     	      		crbp_forward(illit,ex);
		}
		else if ((strcmp(testTask,"s2s")==0)){
			crbp_forward(illit,ex);
		}
		else if ((strcmp(testTask,"p2s")==0)){
			crbp_forward(illit,ex);
		}
		else if ((strcmp(testTask,"s2p")==0)){
			crbp_forward(illit,ex);
		}
		else if ((strcmp(testTask,"o2ps")==0)){
			crbp_forward(read,ex);
		}
		else{
		  printf("Error: Layer name not recognised!");
		}
		if((strcmp(layer2print,"phonology")==0)){
			for(j=startTs;j<noSteps;j++){
				diffMax = get_Diff(phonology->outputs[j-1],phonology->outputs[j], phonology_units);
				if(diffMax<setThresh){break;}
			}
			printf("%f\t",diffMax);
			printf("%d\t",j-startTs);
			if(j>noSteps-1){j=noSteps-1;}
			print_Layer(phonology->outputs[j], phonology_units);
		}
		else if ((strcmp(layer2print,"semantic")==0)){
			for(j=startTs;j<noSteps;j++){
				diffMax = get_Diff(semantic->outputs[j-1],semantic->outputs[j], semantic_units);
				if(diffMax<setThresh){break;}
			}
			printf("%f\t",diffMax);
			printf("%d\t",j-startTs);
			if(j>noSteps-1){j=noSteps-1;}
			print_Layer(semantic->outputs[j], semantic_units);
		}
		else{
		  printf("Error: Layer name not recognised!");
		}
		printf("\n");
  }


  return 0;

}
