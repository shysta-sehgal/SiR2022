/* to compile this, your MIKENET_DIR environment variable
   must be set to the appropriate directory.  putting this
   at the bottom of your .cshrc file will do the trick:

   setenv MIKENET_DIR ~mharm/mikenet/default/


   This demo program solves the xor problem.  

*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <mikenet/simulator.h>

#define SAMPLES 5
#define PHO_FEATURES 11
#define PHO_SLOTS 6
#define ORTHO_FEATURES 260
#define RANDOM_NUMBER_RANGE 0.1

Net *reading, *hearing;
Group *phonology, *orthography, *hidden, *cleanup, *bias;
Connections *oh, *hp, *cp, *pc, *pp, *bh, *bp, *bc;
ExampleSet *reading_examples, *hearing_examples;

float random_number_range=RANDOM_NUMBER_RANGE;

void build_reading_model(int samples)
{

  int i,j;
  float range;

  range = random_number_range;
  //default_errorRamp=RAMP_ERROR;
  default_tai=1;

  default_activationType=TANH_ACTIVATION; 
  //default_errorComputation=SUM_SQUARED_ERROR;
  default_errorRadius=0.1;

  //default_scaling=SCALE_PROB;

  /* build a network, with samples number of time samples */
  reading=create_net(samples);
  hearing=create_net(samples);

  phonology=init_group("Phono",PHO_FEATURES * PHO_SLOTS,samples);
  hidden=init_group("Hidden",100,samples);
  cleanup=init_group("Cleanup",25,samples);
  orthography=init_group("Ortho",ORTHO_FEATURES,samples);
  bias=init_bias(1.0,samples);

  /* now add our groups to the network object */
  bind_group_to_net(reading,phonology);
  bind_group_to_net(reading,hidden);
  bind_group_to_net(reading,orthography);
  bind_group_to_net(reading,cleanup);
  bind_group_to_net(reading,bias);
  
  bind_group_to_net(hearing,phonology);
  bind_group_to_net(hearing,cleanup);
  bind_group_to_net(hearing,bias);

  /* now connect our groups */
  oh=connect_groups(orthography,hidden);
  hp=connect_groups(hidden,phonology);
  pc=connect_groups(phonology,cleanup);
  cp=connect_groups(cleanup,phonology);
  pp=connect_groups(phonology,phonology);
  bh=connect_groups(bias,hidden);
  bp=connect_groups(bias,phonology);
  bc=connect_groups(bias,cleanup);
  
  bind_connection_to_net(reading,oh);
  bind_connection_to_net(reading,hp);
  bind_connection_to_net(reading,pc);
  bind_connection_to_net(reading,cp);
  bind_connection_to_net(reading,pp);
  bind_connection_to_net(reading,bh);
  bind_connection_to_net(reading,bp);
  bind_connection_to_net(reading,bc);
  
  bind_connection_to_net(hearing,pp);
  bind_connection_to_net(hearing,pc);
  bind_connection_to_net(hearing,cp);
  bind_connection_to_net(hearing,bp);
  bind_connection_to_net(hearing,bc);
  
  randomize_connections(oh,range);
  randomize_connections(hp,range);
  randomize_connections(pc,range);
  randomize_connections(cp,range);
  randomize_connections(bh,range);
  randomize_connections(bp,range);
  randomize_connections(bc,range);
  randomize_connections(pp,range);
  
  pc->epsilon=0.001;
  cp->epsilon=0.001;
  bc->epsilon=0.001;
  pp->epsilon=0.001;
  bp->epsilon=0.001;
  
  for(i=0;i<phonology->numUnits;i++){
	pp->weights[i][i]=0.75;
	pp->frozen[i][i] = 1;
  }

  return;
}


int main(int argc,char *argv[])
{
  char cmd[255];
  Example *ex;
  float epsilon, err=0, sw;
  int seed=1,iters=1;
  int i,j,k,r;
  char  loadFile[255],fn[255];
  float prob[10000][14];
  char name[40];
  FILE *f,*lf;

  setbuf(stdout,NULL);

  loadFile[0]=0;

  /* what are the command line arguments? */
  for(i=1;i<argc;i++)
    {
      if (strcmp(argv[i],"-seed")==0)
	{
	  seed=atoi(argv[i+1]);
	  i++;
	}
      else if (strncmp(argv[i],"-epsilon",5)==0)
	{
	  epsilon=atof(argv[i+1]);
	  i++;
	}
      else if (strncmp(argv[i],"-iters",5)==0)
	{
	  iters=atoi(argv[i+1]);
	  i++;
	}
      else
	{
	  fprintf(stderr,"unknown argument: %s\n",argv[i]);
	  exit(1);
	}
    }


  default_epsilon=epsilon;


  mikenet_set_seed(seed);
  srand(seed);
  
  printf("building model ...\n");
  build_reading_model(SAMPLES);

  /* load in our example set */
  printf("loading examples ...\n");
  reading_examples=load_examples("3k_99_trans_read.pat",SAMPLES);
  hearing_examples=load_examples("3k_99_trans_hear.pat",SAMPLES);

  for(i=1;i<=iters;i++){
  
	/* Pre-literate */
	
	if(i<1000001){
		ex=get_random_example(hearing_examples);
		bptt_forward(hearing,ex);
		bptt_compute_gradients(hearing,ex);
		err+=sqrt(powf(compute_error(hearing,ex),2));
		bptt_apply_deltas(hearing);
	}
	
	/* Literate */
    else {
		
		r = rand();
		sw = r / (float) RAND_MAX;
	
		if(sw<0.2){
			ex=get_random_example(hearing_examples);
			bptt_forward(hearing,ex);
			bptt_compute_gradients(hearing,ex);
			err+=sqrt(powf(compute_error(hearing,ex),2));
			bptt_apply_deltas(hearing);
		}
		else{
			ex=get_random_example(reading_examples);
			bptt_forward(reading,ex);
			bptt_compute_gradients(reading,ex);
			err+=sqrtf(powf(compute_error(reading,ex),2));
			bptt_apply_deltas(reading);
		}
	}
	
    if (i % 100000 == 0 ) {
		sprintf(fn,"saveweights_%d",i);
		save_weights(reading,fn);
    }
  
    if (i % 1000 == 0){
		printf("%d %f\n",i,err);
		err=0.0;
	}
  }
  
  sprintf(fn,"saveweights_%d",i);
  save_weights(reading,fn);
  return 0;

}
