// This script will test a HS04 network as described in README_HS04_Rep_0521 & README_testing.txt

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <mikenet/simulator.h>

//Samples defines the number of time steps within a trial
#define SAMPLES 12
#define TICKS SAMPLES
#define SECONDS 4
#define RANDOM_NUMBER_RANGE 0.1

Net *read, *pho, *sem, *comp, *prod, *orth_pho, *orth_sem, *orth_pho_sem, *orth_sem_pho;
Group *phonology, *orthography, *semantic, *bias, *semCU, *phoCU, *oshidden, *ophidden, *comphidden, *prodhidden;
Connections *sem2cu, *cu2sem, *sem2prodhidden, *prodhidden2pho, *pho2cu, *cu2pho, *pho2comphidden, *comphidden2sem, *orth2oshidden, *oshidden2sem, *orth2ophidden, *ophidden2pho, *bias2sem, *bias2semCU, *bias2prodhidden, *bias2pho, *bias2phoCU, *bias2comphidden, *bias2orth, *bias2oshidden, *bias2ophidden, *orth2sem, *orth2pho;
ExampleSet *test_trials;

float random_number_range=RANDOM_NUMBER_RANGE;

// Method for printing network layer
void print_Layer(Real *vec, int units){
	int i;
	for(i=0;i<units;i++){
		printf("%f\t",vec[i]);
	}
}

// Method for accessing trial name
int get_word(char *tag, char *name)
{
  char *p;
  p=strstr(tag,"Word:");
  p+= 5;
  p=strtok(p," \t\n\r");
  strcpy(name,p);
}

// Method for building a new model
void build_reading_model(int samples, int phoUnits, int semUnits, int litUnits, int illUnits){

  int i,j;
  float range;

  range = random_number_range;
  default_errorRamp=RAMP_ERROR;
  default_tai=1;

  default_activationType=LOGISTIC_ACTIVATION;
  default_errorComputation=CROSS_ENTROPY_ERROR;
  default_errorRadius=0.1;

  default_scaling=SCALE_PROB;

  /* Build a network (and sub-networks), with samples number of time samples */
  read=create_net(samples);

  read->integrationConstant=(float)SECONDS/(float)samples;
  
  // Define layers within network
  phonology=init_group("phonology",phoUnits,samples);
  orthography=init_group("orthography",364,samples);
  semantic=init_group("semantic",semUnits,samples);
  semCU=init_group("semCU",100,samples);
  phoCU=init_group("phoCU",50,samples);
  oshidden=init_group("OSHidden",litUnits,samples);
  ophidden=init_group("OPHidden",200,samples);
  comphidden=init_group("CompHidden",illUnits,samples);
  prodhidden=init_group("ProdHidden",illUnits,samples);
  bias=init_bias(1.0,samples);

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

// This main method will build and train a HS04 network
int main(int argc,char *argv[])
{
  char cmd[255];
  Example *ex;
  Real act=0;
  float pherr=0, serr=0, cerr=0, prerr=0, oerr=0, sw;
  int seed=1,iters=1, phtrials=1, strials=1, ctrials=1, prtrials=1, otrials=1, tStep=SAMPLES-1;
  int i,j,k,r,phonology_units,orthography_units,semantic_units,oshidden_units,ophidden_units,pshidden_units,sphidden_units;
  char  loadFile[255],fn[255], testTask[255];
  char *weightFile=NULL,*patternFile=NULL,*route=NULL;
  float prob[10000][14];
  char name[40];
  FILE *f,*lf;

  setbuf(stdout,NULL);

  loadFile[0]=0;

  /* Command line arguements */
  for(i=1;i<argc;i++)
    {
      if (strcmp(argv[i],"-seed")==0)
	{
	  seed=atoi(argv[i+1]);
	  i++;
	}
	else if (strcmp(argv[i],"-tStep")==0){
	  tStep=atoi(argv[i+1]);
	  i++;
	}
      	else if (strcmp(argv[i],"-weights")==0){
		weightFile=argv[i+1];
		i++;
	}
	else if (strcmp(argv[i],"-patterns")==0){
		patternFile=argv[i+1];
		i++;
	}
	else
	{
	  fprintf(stderr,"unknown argument: %s\n",argv[i]);
	  exit(1);
	}
    }


  default_epsilon=0.05;

  mikenet_set_seed(seed);
  srand(seed);

  // Build model
  printf("building model ...\n");
  printf("Time Step = %d\n",tStep);
  printf("Trial\t O2S\t O2P\t C2P\t C2S\t S2P\t P2S\t\n");
  phonology_units = 200;
  semantic_units = 2446;
  oshidden_units = 500;
  pshidden_units = 500;
  sphidden_units = 500;
  build_reading_model(SAMPLES,phonology_units,semantic_units,oshidden_units,pshidden_units);

  // Load trained weight matrix
  load_weights(read,weightFile);

  // Load test trials
  test_trials=load_examples(patternFile,SAMPLES);

  // Test network on test trials
  for(i=0;i<test_trials->numExamples;i++){
		
    ex=&test_trials->examples[i];
	  get_word(ex->name,name);
	  printf("%s\t",name);

    // Propogate activation through network
	  crbp_forward(read,ex);

    // Output mean activation entering via
    // o2s
    act = 0;
    for(j=0;j<2446;j++){
        act += contribution_from_group(oshidden2sem,j,tStep);
        act += contribution_from_group(orth2sem,j,tStep);
    }
    act = act/(2446*2);
    printf("%f\t",act);

    // o2p
    act = 0;
    for(j=0;j<200;j++){
        act += contribution_from_group(ophidden2pho,j,tStep);
        act += contribution_from_group(orth2pho,j,tStep);
    }
    act = act/(200*2);
    printf("%f\t",act);

    // c2p
    act = 0;
    for(j=0;j<200;j++){
        act += contribution_from_group(cu2pho,j,tStep);
    }
    act = act/(200);
    printf("%f\t",act);
            
    // c2s
    act = 0;
    for(j=0;j<2446;j++){
        act += contribution_from_group(cu2sem,j,tStep);
    }
    act = act/(2446);
    printf("%f\t",act);
            
    // s2p
    act = 0;
    for(j=0;j<200;j++){
        act += contribution_from_group(prodhidden2pho,j,tStep);
    }
    act = act/(200);
    printf("%f\t",act);
            
    // p2s
    act = 0;
    for(j=0;j<2446;j++){
        act += contribution_from_group(comphidden2sem,j,tStep);
    }
    act = act/(2446);
    printf("%f\t\n",act);
            
  }


  return 0;

}
