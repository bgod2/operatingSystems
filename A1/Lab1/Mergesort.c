//***** commented out code is for c++*****

//#include <iostream>
//#include <fstream>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>
//#include <vector>
#include<stdio.h>

#define SHM_MODE 0600

//using namespace std;






/*
  @brief: print an array
  @param: arr, a pointer to a given array
          arr will be printed
  @param: n, an integer that is the length of the array
*/   
void print_arr( int *arr, int n ){
  printf("[ ");                   //cout << "[ ";
  for( int i = 0; i < n; i++ ){
    printf("%d ", arr[i]);          // cout << arr[i] << " ";
  }
  printf("]\n ");                   //cout << "]" << endl;
}

/*
  @brief: sorts an array using the merge sort argorithm
          the algorithm splits arr and sorts each half until they are
          single element arrays. the two arrays are then merged into
          a sorted version of arr
  @param: arr, a pointer to a given array
          arr will be sorted
  @param: n, an integen that is the length of the array
*/
int *merge_sort( int arr[],  int n ){
  
  pid_t c1_ID, c2_ID, c1_fin_ID, c2_fin_ID;
  int status1, status2;
  int shmID;
  //key_t key = base++;


  // create shared mem segment
  if( (shmID = shmget(IPC_PRIVATE , n * sizeof(int) + 8 , SHM_MODE)) < 0){
    perror("shmget error");
  }
 
 
  //base case
  if( n <= 1){
    return arr;
  }
  

  int n_1 = n/2;
  int n_2 = n_1 + n%2; 
  int *left = arr;
  int *right = (arr + n_1) ;
 
  
  
  if(( c1_ID = fork() ) == -1){
    perror("fork error");
    exit(EXIT_FAILURE);
  } else if  (c1_ID == 0) {
                                // child 1 process 
                                // vvvvvvvvvv
    // attach to shared mem
   
    
    int* ptr = (int*) shmat( shmID, 0, 0);
    int *left_s = merge_sort( left, n_1);
 
    
    for(int i = 0; i < n_1; i++){
      ptr[i] = left_s[i];
    }

    exit(EXIT_SUCCESS);         // ^^^^^^^^
                                //end of child 1
  } else {
    if(( c2_ID = fork() ) == -1){
      perror("fork error");
      exit(EXIT_FAILURE);
    } else if (c2_ID ==0) {
                                    // child 2 process
                                    // vvvvvvvvvvvvvv
      //attatch to shared mem
   
      int* ptr = (int*) shmat( shmID, 0, 0);      
      int *right_s = merge_sort( right, n_2);
      
      
      
      for( int i = 0; i < n; i++){
	ptr[i + n_1] = right_s[i];
      }
      
      exit(EXIT_SUCCESS);       // ^^^^^^^^^^^
                                // end of child 2
    } else {
                                // parent process
      c1_fin_ID = waitpid( c1_ID, &status1, WUNTRACED);
      c2_fin_ID = waitpid( c2_ID, &status2, WUNTRACED);

      // set if statements for  fin ids
      
      if( WIFEXITED( status1 ) && WIFEXITED( status2) ){
        // attach to shared mem
        int* ptr = (int*) shmat( shmID, 0, 0);
	//both cildren finished
	// rebuild
	int i_1 = 0;
	int i_2 = n_1;
      	
	for( int i = 0; i < n; i++){
	  if( i_1 == n_1 ){ // left empty
	    arr[i] = ptr[i_2];
	    i_2++;
          	    
	  } else if( i_2 == n) { // right empty
	    arr[i] = ptr[i_1];
	    i_1++;
                     
	  } else if( ptr[i_1] < ptr[i_2] ){
	    arr[i] = ptr[i_1];
	    i_1++;
       	    
	  }else{
	    arr[i] = ptr[i_2];
	    i_2++;
            
	  }
	}
       
	
      }
      

    }
  }
  
  return arr;
}

/*
  @brief: reads a file of integers and puts them in a array. the array is
          sorted using merge sort.
          the number of input elements, the input array, and the sorted
          array are printed.
  @param: argv, the input for main is a file name that contains integers
          the file is opened, and read. the contents are put into an array
          which is put into merge sort.
*/
int main(int argc, char* argv[]){
   
  if( argc != 2){
    
    /*
    cout << "function call needs to have a file name after call" 
	 << endl;
    */
    printf("function call needs to have a file name after call\n");
    return 1;
  }
  
  int a;
  int num = 0;
  /*
  vector<int> temp_arr;
  ifstream infile( argv[1]);  
  if(!infile) { 
    cout << "error opening file" << argv[1] << endl;
    return 1;
  }

  while( infile >> a ){
    temp_arr.push_back(a);
    num++;
  }
  
  int arr[num];
  for(int i = 0; i < num; i++){
    arr[i] = temp_arr[i];
  }
  cout << endl << num << " elements read" << endl << endl;
  cout << "Input Numbers: " << endl;
  */
  FILE *fp;
  fp = fopen( argv[1], "r" );
  
  while( !feof (fp)){ //count elements in the file
    fscanf(fp, "%d" , &a);
    num++;
  }
  rewind(fp);
  int arr[num];
  int i = 0;
  while( !feof (fp)){ //count elements in the file
    fscanf(fp,"%d" , &a);
    arr[i] = a;
    i++;
  }
  
  print_arr(arr, num);
  int *sorted = merge_sort( arr,  num );
  printf("Sorted Numbers: \n");
  //cout << endl << "Sorted Numbers: " << endl;
  print_arr( sorted, num);
  
  return 0;
}
