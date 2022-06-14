// This script will build and train a HS04 network as described in README_HS04_Rep_0521.txt & README_training.txt

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

Net *read, *illit;
Group *phonology, *orthography, *semantic, *bias, *semCU, *phoCU, *oshidden, *ophidden, *comphidden, *prodhidden;
Connections *sem2cu, *cu2sem, *sem2prodhidden, *prodhidden2pho, *pho2cu, *cu2pho, *pho2comphidden, *comphidden2sem, *orth2oshidden, *oshidden2sem, *orth2ophidden, *ophidden2pho, *bias2sem, *bias2semCU, *bias2prodhidden, *bias2pho, *bias2phoCU, *bias2comphidden, *bias2orth, *bias2oshidden, *bias2ophidden, *orth2sem, *orth2pho;
ExampleSet *read_examples, *comp_examples, *prod_examples, *sem_examples, *pho_examples;

float random_number_range=RANDOM_NUMBER_RANGE;

// Method for building a new model
void build_reading_model(int samples)
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

  /* Build a network (and sub-networks), with samples number of time samples */
  read=create_net(samples);
  illit=create_net(samples);
  
  read->integrationConstant=(float)SECONDS/(float)samples;
  illit->integrationConstant=(float)SECONDS/(float)samples;
  
  // Define layers within network
  phonology=init_group("phonology",200,samples);
  orthography=init_group("orthography",364,samples);
  semantic=init_group("semantic",2446,samples);
  semCU=init_group("semCU",100,samples);
  phoCU=init_group("phoCU",50,samples);
  oshidden=init_group("OSHidden",500,samples);
  ophidden=init_group("OPHidden",200,samples);
  comphidden=init_group("CompHidden",500,samples);
  prodhidden=init_group("ProdHidden",500,samples);
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

// This main method will build and train a HS04 network
int main(int argc,char *argv[])
{
  char cmd[255];
  Example *ex;
  float epsilon = 0.2, pherr=0, serr=0, cerr=0, prerr=0, oerr=0, sw;
  int seed=1,iters=1, phtrials=1, strials=1, ctrials=1, prtrials=1, otrials=1;
  int i,j,k,r;
  char  loadFile[255],fn[255];
  float prob[10000][14];
  char name[40];
  FILE *f,*lf;

  setbuf(stdout,NULL);

  loadFile[0]=0;

  // Initial parameters for training: iters = total number of training trials; epsilon = initial learning rate
  iters = 4000001;
  epsilon = 0.2;

  /* Command line arguements */
  for(i=1;i<argc;i++){
      if (strcmp(argv[i],"-seed")==0){
	       seed=atoi(argv[i+1]);
	       i++;
	    }
      else{
	       fprintf(stderr,"unknown argument: %s\n",argv[i]);
	       exit(1);
      }
  }

  default_epsilon=epsilon;

  mikenet_set_seed(seed);
  srand(seed);

  // Builds model
  printf("building model ...\n");
  build_reading_model(SAMPLES);

  /* Loads example set */
  printf("loading examples ...\n");
  printf("Pho Error;\tSem Error;\tComp Error;\tProd Error;\tReading Error;\n");
  read_examples=load_examples("6k_Read_Train.pat",SAMPLES);
  comp_examples=load_examples("6k_Pho2Sem_Train.pat",SAMPLES);
  prod_examples=load_examples("6k_Sem2Pho_Train.pat",SAMPLES);
  sem_examples=load_examples("6k_Sem_Train.pat",SAMPLES);
  pho_examples=load_examples("6k_Pho_Train.pat",SAMPLES);

  // Code for specific Run (if needed):

  // Trains network: Single trial per iter loop
  for(i=0;i<=iters;i++){

	r = rand();
	sw = r/(float) RAND_MAX;

  // Pre-literacy training
	if(i<3000001){
        // Change learning rate after 500000 trials
        if(i==500000){
            epsilon = 0.1;
        }
    //Phonology -> Phonology training task
		if(sw<=0.1){
			phtrials ++;
			ex=get_random_example(pho_examples);
			crbp_forward(illit,ex);
			crbp_compute_gradients(illit,ex);
			pherr += compute_error(illit,ex);
			crbp_apply_deltas(illit);
		}
    //Semantic -> Semantic training task
		else if (sw<=0.2){
			strials ++;
			ex=get_random_example(sem_examples);
			crbp_forward(illit,ex);
			crbp_compute_gradients(illit,ex);
			serr += compute_error(illit,ex);
			crbp_apply_deltas(illit);
		}
    //Phonology -> Semantic training task
		else if (sw<=0.6){
			ctrials ++;
			ex=get_random_example(comp_examples);
			crbp_forward(illit,ex);
			crbp_compute_gradients(illit,ex);
			cerr += compute_error(illit,ex);
			crbp_apply_deltas(illit);
		}
    //Semantic -> Phonology training task
		else{
			prtrials ++;
			ex=get_random_example(prod_examples);
			crbp_forward(illit,ex);
			crbp_compute_gradients(illit,ex);
			prerr += compute_error(illit,ex);
			crbp_apply_deltas(illit);
		}
	}
    // Literacy training
    else {
		if(sw<=0.05){
			phtrials ++;
			ex=get_random_example(pho_examples);
			crbp_forward(illit,ex);
			crbp_compute_gradients(illit,ex);
			pherr += compute_error(illit,ex);
			crbp_apply_deltas(illit);
		}
		else if (sw<=0.1){
			strials ++;
			ex=get_random_example(sem_examples);
			crbp_forward(illit,ex);
			crbp_compute_gradients(illit,ex);
			serr += compute_error(illit,ex);
			crbp_apply_deltas(illit);
		}
		else if (sw<=0.35){
			ctrials ++;
			ex=get_random_example(comp_examples);
			crbp_forward(illit,ex);
			crbp_compute_gradients(illit,ex);
			cerr += compute_error(illit,ex);
			crbp_apply_deltas(illit);
		}
		else if (sw<=0.6){
			prtrials ++;
			ex=get_random_example(prod_examples);
			crbp_forward(illit,ex);
			crbp_compute_gradients(illit,ex);
			prerr += compute_error(illit,ex);
			crbp_apply_deltas(illit);
		}
    // Orthography -> Semantics & Phonology Training Task
		else{
			otrials ++;
			ex=get_random_example(read_examples);
			crbp_forward(read,ex);
			crbp_compute_gradients(read,ex);
			oerr += compute_error(read,ex);
			crbp_apply_deltas(read);
		}
	}
    // Save weights after 100000 trials
    if (i % 100000 == 0 ) {
		sprintf(fn,"saveweights_read_%d",i);
		save_weights(read,fn);
    }

    // Output performance on each task after 5000 trials
    if (i % 5000 == 0){
		printf("%d %f; %f; %f; %f; %f; %f;\n",i,pherr/phtrials,serr/strials,cerr/ctrials,prerr/prtrials,oerr/otrials,epsilon);
		pherr=0.0;
		serr=0.0;
		cerr=0.0;
		prerr=0.0;
		oerr=0.0;
		phtrials =1;
		strials =1;
		ctrials =1;
		prtrials =1;
		otrials =1;
	}
  }

  // Once training is complete output final weight matrix
	printf("Pho Error;\tSem Error;\tComp Error;\tProd Error;\tReading Error;\tEpsilon;\n");
	sprintf(fn,"saveweights_read_%d",i);
	save_weights(read,fn);

	return 0;

}
