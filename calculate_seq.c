#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char ** argv){
	double t;	
	double bestTime;
	FILE* f;
	FILE* fout = fopen("result.txt","w");
	
	int i,j;
	char cmd[255];
	int num_progs = 5;
	
	int num_test = atoi(argv[1]);
	int size;
	
	for (j=1; j<22; j++){
		
		size = j*50;
		
		f = fopen("defineN.h","w");
		fprintf(f, "#define N %d\n",size);
		fclose(f);
	
		/* Compile */
		sprintf(cmd, "gcc -o seq %s",argv[2]);
		i = system(cmd);
		
		if (i!=0){
			printf("Compile %d Fail\n", i);
			return -1;
		}
		printf("Compile OK with N = %d\nRunning\n",size);
		
		// Sequency
		bestTime = 0;
		fprintf(fout,"Sequenly run %d times (N = %d): ", num_test, size);
		system("rm temp.txt");
		
		for (i=0; i<num_test; i++){
			system("./seq >> temp.txt");
		}
		
		
		f = fopen("temp.txt","r");
		bestTime = 0;
		for (i=0; i<num_test; i++){
			fscanf(f,"%lf",&t);
			
			if (bestTime == 0 || bestTime > t)
				bestTime = t;
			//fprintf(fout,"\t%d:\t%f\n",i,t);
			//printf("best time is %f
		}
		fclose(f);
		//fprintf(fout,"--> With N = %d, best time is: %f\n",size, bestTime);
		//fprintf(fout, "################################################\n\n");
		fprintf(fout, "%f\n\n", bestTime);
	}
	
	fclose(fout);
	return 0;
}
