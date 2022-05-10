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

typedef struct
{
  char ch;
  Real vector[PHO_FEATURES];
} Phoneme;

Phoneme phonemes[50];
char words[30000][30];
int symbol_hash[255];
int phocount=0;

float nonword_error(Real *vec,char *t1){
  float e1=0,e,d1;
  char c;
  int v,i,j;

  for(i=0;i<PHO_SLOTS;i++){

      v=symbol_hash[(int)t1[i]];

      if (v == -1){
    	  fprintf(stderr,"error on hash lookup, char %c\n",
		    t1[i]);
	      exit(0);
	    }
      
      d1=0;

      for(j=0;j<PHO_FEATURES;j++){

    	  e=vec[i*PHO_FEATURES+j] - phonemes[v].vector[j];
    	  d1 += e * e;
    	}
    
      e1 += d1;
  }

  return e1;

}



load_phonemes(){
  FILE * f;
  char line[255],*p;
  int i,x;
  f=fopen("mapping","r");
  if(f==NULL){
    fprintf(stderr,"no mapping file\n");
    exit(1);
  }
  x=0;
  fgets(line,255,f);
  while(!feof(f)){
    p=strtok(line," \t\n");
    if (p[0]=='-') p[0]='-';
    phonemes[phocount].ch=p[0];
    // printf("%d %c\n",phocount,p[0]);
    symbol_hash[(int)(p[0])]=x++;
    for(i=0;i<PHO_FEATURES;i++){
      p=strtok(NULL," \t\n");
      if (strcmp(p,"NaN")==0)
	phonemes[phocount].vector[i]= -10;
      else 
	phonemes[phocount].vector[i]= atof(p);
    }
    phocount++;
    fgets(line,255,f);
  }
  fclose(f);
}


float euclid_distance(Real *x1,Real *x2){
  float d=0,r;
  int i;
  for(i=0;i<PHO_FEATURES;i++){
    r = x1[i] - x2[i];
    d += r * r;
  }
  return d;
}
      
Real euclid(Real *v,char *out){
 
  int i,j;
  int nearest_item;
  float error=0;
  float nearest_distance,d;
  
  for(i=0;i<PHO_SLOTS;i++){
    
    nearest_item=-1;
    
    for(j=0;j<phocount;j++){

      d=euclid_distance(&v[i*PHO_FEATURES],phonemes[j].vector);

      // printf("%d %d %s\n",i,j,&v[i*PHO_FEATURES]);

      if ((nearest_item == -1) || (d < nearest_distance)){

	        nearest_item=j;
	        nearest_distance=d;
      }
    }
    error += d;
    out[i]=phonemes[nearest_item].ch;
  }

  out[PHO_SLOTS]=0;
  return nearest_distance;

}

// adds a return statement to end of phono representation char array taken from example (pat file)
int get_name(char *tag, char *name)
{
  char *p;
  p=strstr(tag,"Pho:");
  p+= 4;
  p=strtok(p," \t\n");
  strcpy(name,p);
}

// adds a return statement to end of word char array taken from example (pat file)
int get_nameword(char *tag, char *name)
{
  char *p;
  p=strstr(tag,"Word:");
  p+= 5;
  p=strtok(p," \t\n");
  strcpy(name,p);
}

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
  
  randomize_connections(oh,1.0);
  randomize_connections(hp,1.0);
  randomize_connections(pc,1.0);
  randomize_connections(cp,1.0);
  randomize_connections(bh,1.0);
  randomize_connections(bp,1.0);
  randomize_connections(bc,1.0);
  randomize_connections(pp,1.0);
  
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
  float epsilon, err=0;
  int seed=1,iters=1;
  int i,j;
  char  loadFile[255],fn[255];
  float prob[10000][14];
  FILE *f,*lf;
  char *weightFile=NULL;
  char euclid_output[30], name[40], nameword[40];
  Real error=0;

  setbuf(stdout,NULL);

  loadFile[0]=0;

  /* what are the command line arguments? */
  for(i=1;i<argc;i++)
    {
      if (strcmp(argv[i],"-weights")==0){
	weightFile=argv[i+1];
	i++;
      }
      else if (strncmp(argv[i],"-epsilon",5)==0)
	{
	  epsilon=atof(argv[i+1]);
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

  load_phonemes();

  build_reading_model(SAMPLES);

  /* load in our example set */
  reading_examples=load_examples("3k_99_trans_read.pat",SAMPLES);
  hearing_examples=load_examples("3k_99_trans_hear.pat",SAMPLES);
  
  for(i=0;i<reading_examples->numExamples;i++){
    ex=&reading_examples->examples[i];
    ex->prob=1;
    }
	for(i=0;i<hearing_examples->numExamples;i++){
    ex=&hearing_examples->examples[i];
    ex->prob=1;
    }
  
  load_weights(reading,weightFile);

  for(i=0;i<reading_examples->numExamples;i++){
    ex=&reading_examples->examples[i];
    bptt_forward(reading,ex);
    euclid(phonology->outputs[SAMPLES-1],euclid_output);
    get_name(ex->name,name);
    get_nameword(ex->name,nameword);
    error=nonword_error(phonology->outputs[SAMPLES-1],name); 

    printf("%s\t%s\t%f\t%s\n",nameword,name,error,euclid_output);    
  }
  
  printf("\n\n\nHEARING TEST\n\n");   
  
  for(i=0;i<hearing_examples->numExamples;i++){
    ex=&hearing_examples->examples[i];
    bptt_forward(hearing,ex);
    euclid(phonology->outputs[SAMPLES-1],euclid_output);
    get_name(ex->name,name);
    get_nameword(ex->name,nameword);
    error=nonword_error(phonology->outputs[SAMPLES-1],name); 

    printf("%s\t%s\t%f\t%s\n",nameword,name,error,euclid_output);    
  }
  
  return 0;
}
