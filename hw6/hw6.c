/* HW6 read cafefully and follow all the instructions/comments
   this program compiles and runs as is although it's not doing much.
   DO NOT CHANGE THE SEED. Changing seed gives a different sequence of
   numbers but we want the same numbers for comparison purposes.

   FLOATING point radix sort requires some substantial changes to this code
   float n, lst[N],tmp[N];	
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>
#include <string.h>

#define MAXBITS 64
#define OK 1
#define NOK 0
#define NELM 100		/* default 100 elements */
#define N 1048576		/* 2^30 or 1 meg elements  */
//#define N 2097152
//#define N 4194304
//#define N 8388608
//#define N 16777216
//#define N 33554432

void selection_sort(int list[],int n);

void merge_sort(int list[], int temp[], int n);
void msort_recursive(int list[], int temp[], int left, int right);
void merge(int list[], int temp[], int left, int mid, int right);

// void radix_sort(int ii);
void int_radix_sort(int *list,int temp[],int n);
void float_radix_sort(int *list,int temp[],int n);
void swap();
void self_check(int *list,int n);

int rand(void);
void srand();
int rand_r();
void init_lst(int *l,int n);
void print_lst(int *l,int n);

int n, lst[N],tmp[N];

int main(int argc,char **argv) {

  long int del_sec,del_msec;
  struct timeval tv_s,tv_e, diff;

  if (argc>1) n = atoi(argv[1]);
  else n = NELM;
  printf("n=%d\n",n);
  init_lst(lst,n);
  // print_lst(lst,n);
  int i;
  int test = 50;
  int avg=0;
  for (i=0; i<test; i++){
    gettimeofday(&tv_s, NULL); 
    // selection_sort(lst,n);
     merge_sort(lst,tmp,n);
     // int_radix_sort(lst,tmp,n);
    //  float_radix_sort(lst,tmp,n);
    gettimeofday(&tv_e, NULL); 

    /****
      PRINT elapsed time in sec and milli secs
    ****/
    timersub(&tv_e, &tv_s, &diff);
    avg+=diff.tv_usec;
    printf("%ld\t", diff.tv_usec);
  }
  printf("\nAverage: %d\n", avg/test);

  // print_lst(lst,n);
  // self_check(lst, n);
  return 0;
}

void selection_sort(int list[],int n){
  // fill here
  int i, j;
  for (i=0; i<n-1; i++){
    int min=i;  
    for (j=i+1; j<n; j++){
      if (list[j]<list[min]){
        min=j;
      }
    }
    if (min != j){
      int temp = list[i];
      list[i] = list[min];
      list[min] = temp;
    }
  }
}

void merge_sort(int list[], int temp[], int n){
  msort_recursive(list, temp, 0, n-1);
}

//use recursion
void msort_recursive(int list[], int temp[], int left, int right){
  // fill here
  int mid;

  if (right > left){
    mid = (right + left)/2;

    msort_recursive(list, temp, left, mid);
    msort_recursive(list, temp, mid+1, right);
    merge(list, temp, left, mid+1, right);
  }
}

void merge(int list[], int temp[], int left, int mid, int right){
  int i, end, num, temp_idx;
  end = mid-1;
  temp_idx=left;
  num = right-left+1;

  while ((left<=end) && (mid<=right)){
    if (list[left] <= list[mid]){
      temp[temp_idx]=lst[left];
      temp_idx=temp_idx + 1;
      left=left+1;
    }
    else {
      temp[temp_idx] = list[mid];
      temp_idx=temp_idx+1;
      mid=mid+1;
    }
  }

  while (left<=end){
    temp[temp_idx]=list[left];
    left=left+1;
    temp_idx=temp_idx+1;
  } 

  while (mid<=right){
    temp[temp_idx]=list[mid];
    mid=mid+1;
    temp_idx=temp_idx+1;
  }
  for (i=0; i<num; i++){
    list[right]=temp[right];
    right=right-1;
  }
}

//fix the bucket size to 256. run 4 passes where each pass processes 8 bits
//use lst and tmp only. do not use any more memory of size n.
void int_radix_sort(int list[], int temp[], int n) {
  int group=8;			/* 8 bits per pass (or round) */
  int bucket = 1 << group;	/* number of buckets = 256 */
  // fill here
  int mask = bucket-1;
  int i, iii;
  for (iii=0; iii<MAXBITS; iii+=group){

    int cnt[bucket], map[bucket];
    memset(cnt, 0, sizeof(int)*bucket);

    //count 
    for (i=0; i<n; i++){
      cnt[(list[i]>>iii)&mask]++;
    }

    //map 
    map[0]=0;
    for (i=1; i<bucket; i++){
      map[i]=map[i-1]+cnt[i-1];
    }

    //move 
    for (i=0; i<n; i++){
      temp[map[(list[i]>>iii)&mask]++]=list[i];
    }

    memcpy(list, temp, n*sizeof(int));
  }

}

void float_radix_sort(int list[], int temp[], int n){
  int_radix_sort(list, temp, n);
  int i, j, k, negative;
  //find the negative number 
  for (i=0; i<n; i++){
    if (lst[i]<0){
      negative=i;
      break;
    }
  }
  k=0;
  for (j=negative; j<n; j++){
    tmp[k++] = lst[j];
  }

  for (k=0; k<negative;k++){
    tmp[negative++] = lst[k];
  }
}

void print_lst(int *l,int n){
  int i;
  for (i=0; i<n; i++) {
    printf("%d ",l[i]);
  }
  printf("\n");
}

void init_lst(int *l,int n){
  int i;
  //  int seed = time(0) % 100;	/* seconds since 1/1/1970 */
  //  printf ("seed=%d\n", seed);
  srand(1234);			/* SEED */
  for (i=0; i<n; i++) {
    l[i] = rand();
  }
}

void self_check(int *list,int n) {
  int i,j,flag=OK,*listp;

  listp = list;
  for (i=0;i<n-1;i++)
     if (listp[i] > listp[i+1]) { flag = NOK; break; }

  if (flag == OK) printf("sorted\n");
  else printf("NOT sorted at %d\n",i);
}