#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char ** argv){
	double t;
	double maxSpeedUp = 0;
	double speedup;
	double bestTime;
	double sequenTime = 0;
	FILE* f;
	FILE* fout = fopen("result.txt","w");
	
	int i;
	char cmd[255];
	int num_progs = 5;
	
	int num_test = atoi(argv[1]);
	int max_num_progs = atoi(argv[3]);
	int min_num_progs = atoi(argv[2]);
	
	/* Compile */
	sprintf(cmd, "gcc -o seq %s && mpicc -o par %s",argv[4], argv[5]);
	i = system(cmd);
	
	if (i!=0){
		printf("Compile %d Fail\n", i);
		return -1;
	}
	printf("Compile OK\nRunning\n");
	
	// Sequency
	bestTime = 0;
	printf("[-] Sequency\n");
	fprintf(fout,"[-] Sequenly run %d times:\n", num_test);
	system("rm temp.txt");
	
	for (i=0; i<num_test; i++){
		system("./seq >> temp.txt");
	}
	
	
	f = fopen("temp.txt","r");
	
	for (i=0; i<num_test; i++){
		fscanf(f,"%lf",&t);
		
		if (bestTime == 0 || bestTime > t)
			bestTime = t;
		fprintf(fout,"\t%d:\t%f\n",i,t);
	}
	fclose(f);
	
	sequenTime  = bestTime;
	
	fprintf(fout,"Sequenly run %d times, best time = %f\n",num_test, sequenTime); 
	fprintf(fout, "################################################\n\n");
	
	
	//Parallel
	for (num_progs = min_num_progs; num_progs <= max_num_progs; num_progs++){
		bestTime  = 0;
		printf("[-] Parallel with %d process\n",num_progs);
		fprintf(fout,"Parallel run with %d process %d times:\n", num_progs, num_test);
		system("rm  temp.txt");
		for (i=0; i<num_test; i++){
			sprintf(cmd,"mpirun -np %d ./par >> temp.txt",num_progs);
			system(cmd);
		}
		
		f = fopen("temp.txt","r");
		
		for (i=0; i<num_test; i++){
			
			fscanf(f,"%lf",&t);
			
			if (bestTime == 0 || bestTime > t)
				bestTime = t;
			fprintf(fout,"\t%d:\t%f\n",i,t);
		}
		fclose(f);
		
		fprintf(fout,"Parallel run (%d process) %d times, avr = %f\n",num_progs, num_test, bestTime);
		speedup = sequenTime/bestTime;
		maxSpeedUp = (speedup > maxSpeedUp)? speedup:maxSpeedUp;
		
		fprintf(fout,"----------> SPEED UP = %f\n",speedup);
		fprintf(fout, "################################################\n\n");
		
	}
	
	printf("Finish, best speedup is %f\n", maxSpeedUp);
	fprintf(fout, "Finish, best speedup is %f\n", maxSpeedUp);
	fclose(fout);
	return 0;
}
