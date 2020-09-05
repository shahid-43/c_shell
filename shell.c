// **** ls function working in normal terminal but not in replit****


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h> 
int shellinbuilt(char **args);
int linexecute(char **args);
void startshell();
char **parse(char *line);
int position = 0;
int shellgrep(int position,char **args);
int shellpwd(char **args);
int shellmkdir(int position,char **args);
int shellcp(int position,char **args);
int shellcd(char **args);
int shellcat(int position,char **args);
//int shellls(int argc,char **argv);


int main(int argc , char **argv){ //intiating the shell
	while(1){
	startshell(); //shell loop 
	}
}


void startshell(void){
	char line[200];//input line 
	int m = 0;//for loop variable
  
	char **args;//after parsing the line the words are stored here
	
	int status;//to keep the shell running after the running a command
    do {
    printf("\n~");
		fgets(line,200,stdin);//takes the user input from terminal
		if (line[strlen(line)-1]== '\n'){//closeing the line i.e NULL
			line[strlen(line)-1] = '\0';
		}
			
	args = parse(line);
   
	status = linexecute(args);
	
	
  for (m= 0;m<position;m++){
		args[m]='\0';
	}
  //after running any command the args is updated to null 
  
	
    
    }while(status);
}



//parsing the line return args
#define BUFF_SIZE 64
//buffer size for allocation of memory for args

char **parse(char *line){
	int bufsize = 64;
  
  position = 0 ;
	char *token;
  //token variable is used to store the separated words from the line input 
	char **words = malloc(bufsize*sizeof(char));
	//error
	if(!words){
		fprintf(stderr, "shell:allocation error\n" );
		exit(EXIT_FAILURE);	
    }



	token = strtok(line," ");
  //separates when space occurs 
  //&detection?

	while (token != NULL){
		
		words[position] = token;

		position = position + 1;
		//we are using the position variable to store the length of the args
		if (position >= bufsize){
			bufsize+=bufsize;
			words = realloc(words,bufsize * sizeof(char*));

			if(!words){
		fprintf(stderr, "shell:not allocated\n" );
		exit(EXIT_FAILURE);	
	}
		}
         token = strtok(NULL," ");

	}
	//printf("%d",position);

	words[position] = '\0';
  //final word is NULL 
     return words;


	}

 int linexecute(char **args){
   
//      from here onwards normal execution of functions

	  if (strcmp(args[0],"exit")==0){
     //comparing first word
		 exit(0);
	 }
	 else if (strcmp(args[0],"cat")==0){
	shellcat(position,args);
   }
   else if(strcmp(args[0],"cd")== 0){
      shellcd(args);
		 
   }
  
   else if(strcmp(args[0],"cp")== 0){
     shellcp(position,args);

   }

   else if(strcmp(args[0],"grep")== 0){
     shellgrep(position,args);
   }

	//   else if (strcmp(args[0],"ls")==0){
	// 		shellls(position,args);

	// 	}

	 else if (strcmp(args[0],"mkdir")==0){
		 shellmkdir(position,args);

	 }


	 else if (strcmp(args[0],"pwd")==0){
		 if (position >1){

			 printf("error:unsupported format");
       //pwd wont take any extra arguments
		 return 1;

	   }
		 else{
		 shellpwd(args);
		}
	  
	 }
   else{
     shellinbuilt(args);
   }
 	
   }
 		
 
 	
int shellmkdir(int position,char **args){
  //creating a directory using mkdir function
	int created;

	if (position==1){
    //mkdir requires atleast one more argument
		printf("invalid input");
		return 1;
	}
  else{
    for (int i = 1; i<position;i++){
    //position>1 => create multiple directories
		
		created = mkdir(args[i],0777);
    //mkdir returns 0 if a directory is created 
		
		if(!created){
			printf("directory is created");
			system("dir");
			//lists the contents of the directory...
      //for checking whether directory created or not 
		}
		else{
			printf("directory cannot be created ");
			
		}
    created = 0;
    }
    return 1;
    //status
    
	}
}





int shellpwd( char **args) {
  //prints the path of the currently working directory
         char *ourdirectory = (char *) malloc (500);

         if (getcwd(ourdirectory, 500) != NULL){
           //getcwd gets the path of currentdir

                 fprintf(stdout, "%s\n", ourdirectory);
		 //printing in terminal the directory path
         }
        return 1;
        //status
         }

int shellgrep(int position,char **args){
  //function to print the line from a file, if the line contains a given word 
  //or sentence
  if (position == 2){
  //file is not mentioned 
  //in this case it takes an string input from the user and checks whether 
  //line in the given input contains the pattern or not
char line[200];
//allocating some memory
char word;
fgets(line,200,stdin);
		if (line[strlen(line)-1]== '\n'){
			line[strlen(line)-1] = '\0';
		}

    if(strstr(line,args[1])!=NULL){
      //checks the line for args[1]
     printf("%s\n",line);

  }
  }
  else{
    for (int i = 2;i<position;i++){
    //for checking in multiple files
        FILE *fp;
        char *word = NULL;
        
        ssize_t len = 0;
      //getline allocates the size 
        fp = fopen(args[i],"r");
        //opening file which is to be checked
        if (fp == NULL){
            printf("file doesnt exist");
            return 1;
        }
        while(getline(&word,&len,fp)!= EOF){  
          //checking line by line from the file args[i]  
        
        if(strstr(word,args[1])!=NULL){
                    printf("%s\n",word);
        }
        
        }
        free(word);
      fclose(fp);  
    }
  }
}



int shellcp(int position,char **args){
  //cp file1 file2 
  if (position<3){
    printf("not sufficient files");
    return 1;
  }
  
  if (position == 3){
    FILE *fr;
    char *line = '\0';
    FILE *fw;
    ssize_t len = 0;
    fr = fopen(args[1],"r");
    fw = fopen(args[2],"a");
    if(fr==NULL){
      printf("%s",args[1]);
      printf("file1 doesnot exist");
    return 1;
    }
  if(fw ==NULL){
    printf("file2 doesnot exist");
    return 1;
  }
  while (getline(&line,&len,fr)!=EOF){
    //gets line from the file from which we have to copy 
    //printf("%s",line);
    
    fprintf(fw, "%s", line);
    //writes the lines in to a file
  
  }
  
  fclose(fr);
  fclose(fw);
  }
  //multiple files ex:
  //cp file1 file2 file3 .....filen copies file1 file2.... in to the filen
   if (position > 3){
     //printf("new loop");
     char *word = '\0';
     //NULL
     ssize_t len;
     //alloted by getline
     for (int i = 1;i<position-1;i++){
     FILE *fr = NULL;
     FILE *fw = NULL;
     fr = fopen(args[i],"r");
     //printf("%s",args[i]);
     fw = fopen(args[position-1],"a");
     if(fr==NULL){
    return 1;
    }
  if(fw ==NULL){
    return 1;
  }
  while ((getline(&word,&len,fr))!=EOF){//similar to the two file case
   
    fprintf(fw,"%s",word);
  }
  
  fclose(fr);
  fclose(fw);

     }
     return 1;
   }

}

int shellcat(int position,char **args)
{ //function to print the contents in a file
  int i;
	if (position > 1 ){
    //cat takes arguments as input

 for (i = 1;i<position;i++){
 //printing contents from multiple files 
   char ch;
   FILE *fp;
printf("%s",args[i]);
//states the name of file before its contents

   fp = fopen(args[i], "r"); // read mode
  
   if (fp == NULL)//failed to open
   {
      printf("cant open file %s.\n",args[i]);
      return 1;
      
			
   }

   else{
   while((ch = fgetc(fp)) != EOF){
     //fgets every charecter in the opened file and prints them
      printf("%c", ch);
   
   
	 }
   fp = NULL;
 } 
 
}
return 1;
  }
else{
  printf("file cannot be opened \n");
	return 1;
}
 return 1;
}

 
int shellcd(char**args){
  //changes directory

  if (args[1] == NULL) { //no directory given as input
    printf("no directory");
    return 1;
  } else {
    if (chdir(args[1]) != 0) {//change directory not successfull
      printf("cannot make the directory");
      return 1;
    }//if not directory ch dir successfull
  }
  return 1;


}

int shellinbuilt(char **args){
        //if args [0] does not match with any of our functions 
        //checking if its any inbuilt function
  pid_t p; 
  int rc = fork();
  
   
     if(rc<0){
      printf("child not created");
      return 1;
    }
    else if( rc == 0){//successfully created child
     if(execvp(args[0],args)== -1){
      printf("error");
      return 1;
    }
    }
    else {
      p = wait(NULL);
      return 1;

    }
  }


