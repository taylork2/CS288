/* 
   CS288 HW10
*/
#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define NELMS 1000000
#define MASTER 0
#define MAXPROCS 16

int dot_product(int s,int e, int x[], int y[]);
void init_lst(int *l,int n);
void print_lst(int s,int e, int *l);

int main(int argc, char **argv) {
  int i,n,vector_x[NELMS],vector_y[NELMS];
  int lst_prod[100];
  int prod,tmp_prod,sidx,eidx,size;
  int pid,nprocs, rank;
  double stime,etime;
  MPI_Status status;
  MPI_Comm world;

  n = atoi(argv[1]);
  if (n > NELMS) { printf("n=%d > N=%d\n",n,NELMS); exit(1); }

  MPI_Init(&argc, &argv);
  world = MPI_COMM_WORLD;
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);

  stime = MPI_Wtime();
  //...
  int portion = n/nprocs;
  if (pid==MASTER){
    init_lst(vector_x, n);
    init_lst(vector_y, n);
    print_lst(0, n, vector_x);
    print_lst(0, n, vector_y);
  }
  sidx=pid*portion; 
  eidx=sidx+portion;
  MPI_Scatter(vector_x,portion,MPI_INT,vector_x+sidx,portion,MPI_INT,MASTER,world);
  MPI_Scatter(vector_y,portion,MPI_INT,vector_y+sidx,portion,MPI_INT,MASTER,world);
  prod = dot_product(sidx, eidx, vector_x, vector_y);
  MPI_Gather(&prod,1,MPI_INT,&lst_prod,1,MPI_INT,MASTER,world);
  if (pid==MASTER){
    for (i=1; i<nprocs; i++){
      prod = prod + lst_prod[i];
    }
  }
  etime = MPI_Wtime();

  if (pid == MASTER) {
    printf("pid=%d: final prod=%d\n",pid,prod);
    printf("pid=%d: elapsed=%f\n",pid,etime-stime);
  }
  MPI_Finalize();
}

int dot_product(int s,int e, int x[], int y[]){
  int i,prod=0;
  for (i=s; i<e; i++){
    prod = prod + x[i] * y[i];
  }
  return prod;
}

void init_lst(int *l,int n){
  int i;
  for (i=0; i<n; i++) *l++ = i;
}
void print_lst(int s,int e, int *l){
  int i;
  for (i=s; i<e; i++) {
    printf("%x ",l[i]);
  }
  printf("\n");
}

// end of file