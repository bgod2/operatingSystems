#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>


/** @brief Prints the premissions of a item in the directory
 *  @param fStat the stat structure of the item 
 *  @return void.
 */
void printPerm( struct stat fStat){
  //from the lab slides
  printf( (fStat.st_mode & S_IRUSR) ? "r" : "-");
  printf( (fStat.st_mode & S_IWUSR) ? "w" : "-");
  printf( (fStat.st_mode & S_IXUSR) ? "x" : "-");
  printf( (fStat.st_mode & S_IRGRP) ? "r" : "-");
  printf( (fStat.st_mode & S_IWGRP) ? "w" : "-");
  printf( (fStat.st_mode & S_IXGRP) ? "x" : "-");
  printf( (fStat.st_mode & S_IROTH) ? "r" : "-");
  printf( (fStat.st_mode & S_IWOTH) ? "w" : "-");
  printf( (fStat.st_mode & S_IXOTH) ? "x" : "-");
  printf(" ");
}

/** @brief print the number of links for an item in the directory
 *  @param fStat the stat structure of the item 
 *  @return void
 */

void printNumL( struct stat fStat ){
  //change value to number  
  printf("%2d ", (int)fStat.st_nlink);
}

/** @brief print the  owner of an item in the directory
 *  @param fStat the stat structure of the item 
 *  @return void
 */
void printOwnr( struct stat fStat ){
  uid_t Uid = fStat.st_uid;
  struct passwd *pw;

   if ((pw = getpwuid(Uid)) == NULL) {
     printf("FAIL: uid %d does not exist\n", Uid);
     return;
  }
  printf("%s ", pw->pw_name);
  
}

/** @brief print the group of an item in the directory
 *  @param fStat the stat structure of the item 
 *  @return void
 */
void printGrp( struct stat fStat ){
  gid_t Gid = fStat.st_gid;
  struct group *grp;

   if ((grp = getgrgid(Gid)) == NULL) {
     printf("FAIL: gid %d does not exist\n", Gid);
     return;
   }
   printf("%s ", grp->gr_name);
  
}
 
/** @brief print the size of an item in the directory
 *  @param fStat the stat structure of the item 
 *  @return void */
void printSize( struct stat fStat ){
  printf("%4d ",(int) fStat.st_size); 
}

/** @brief print the last time modified of an item in the directory
 *  @param time the last modified time
 *  @return void
 */
void printDate( time_t time ){
  char buf[20];
  strftime( buf, 20, "%d %b %H:%M ", localtime( &time));
  printf("%s", buf); 
}

/** @brief print the name of an item in the directory
 *  @param pDirent the item in the directory
 *  @return void
 */
void printName( struct dirent *pDirent ){
  printf("%s", pDirent->d_name);
}

/** @brief print the item that the item in the directory is linked to
 *  @param pdirent the item in the directory
 *  @param fStat the stat structure of the item 
 *  @return void
 */
void printLink( struct dirent *pDirent, struct stat fStat ){
  ssize_t n;
  int len = fStat.st_size + 1;
  char *link = malloc( len );
  
  if( link == NULL){
    printf("insufficient mumory");
    exit(1);// handle error
  }
  
  n = readlink( pDirent->d_name, link, len);
  if( n < 0 ){
    return;
  } else {
    link[len] = '\0';
    printf(" -> %s", link);
  }
  
}

/** @brief go through the input directory and print out the: file 
 *         type, permissions, number of links, owner, group, size
 *         in bytes, last modified date, name of the item, and
 *         where it is pointing if its a link
 *  @param dirName the name of the directory 
 *  @return void
 */

int readDir( char *dirName ){
  struct dirent *pDirent;
  struct stat fStat;
  DIR *pDir;

  pDir = opendir( dirName );
  if( pDir == NULL){
    printf("ERROR cannot open directory");
  }
  int type;
  
  while( ( pDirent = readdir(pDir) ) != NULL){
    //if(
    lstat(pDirent->d_name, &fStat);// == ){
    //  perror("stat error");
    //  continue;
   //}

      
    //type
    //this part doesn't work properly, some files are considered as
    //directories and none of the files are considered links.
    //My code is very similar to the lab code but mine doesn't work.
    if(S_ISLNK( fStat.st_mode)){ 
      printf("l");
      type = 0;
    }else if(S_ISREG( fStat.st_mode)){
      printf("-");
      type = 1;
    }else if(S_ISDIR( fStat.st_mode)){
      printf("d");
      type = 2;
    }    
    
    printPerm( fStat );
    printNumL( fStat  );
    printOwnr( fStat );
    printGrp( fStat  );
    printSize( fStat );
    printDate( fStat.st_mtime );
    printName( pDirent );
    if(type == 0){
      //link
      printLink( pDirent, fStat  );
    }else if(type == 2){
      printf("/");
    }
    printf("\n");
  }

  // go through directories
  
  rewinddir(pDir);
  while( ( pDirent = readdir(pDir) ) != NULL){
    lstat(pDirent->d_name, &fStat);
    if( S_ISDIR( fStat.st_mode)){
      if(strcmp( pDirent->d_name, ".") == 0 ){ continue;}
      if(strcmp( pDirent->d_name, "..") == 0 ){ continue;}
      printf("\n\n%s\n",pDirent->d_name);
      readDir(pDirent->d_name);
    }  
  }
  
  printf("\n\n");
  return 0;
}


/** @brief check inputs and run readDir
 *  @return 0 on success and 1 on failure
 */
int main( int argc, char** argv){
   
  //argument specifications
  if( argc != 2){
    printf("Incorrect commandline arguments: ./lsRalF <directory>");
    return 1;
  }
  
  printf("%s\n",argv[1]);
  readDir( argv[1] );
  
  return 0;
}

