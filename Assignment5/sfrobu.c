/*
NAME: Baolinh Nguyen
UID: 104732121
TA: Farnoosh Javadi
CS35L Lab 6
Assignment 5 HW
File: sfrobu.c
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<errno.h>
#include<limits.h>

int caseSensitive = 1;

int comp = 0;

int compare(const char* a, const char* b)
{
  comp++;
  //return -1 if a < b
  //return 0 if a==b
  //return 1 if a > b

  //look through entire word
  for (;;a++, b++)
    {
      if (*a == ' ' && *b  == ' ')
	return 0;
      if (!caseSensitive)
	{
	  if (*a < UCHAR_MAX && *b < UCHAR_MAX)
	    {
	      if ((toupper(*a))^42 < ((toupper(*b))^42))
		return -1;
	      else if (*a == ' ')
		return -1;
	      if ((toupper(*a))^42 > ((toupper(*b))^42))
		return 1;
	      else if (*b == ' ')
		return 1;
	    }
	  else if (*a < UCHAR_MAX)
	    {
	      if ((toupper(*a))^42 < (*b)^42)
		return -1;
	      else if (*a == ' ')
		return -1;
	      if ((toupper(*a))^42 > (*b)^42)
		return 1;
	      else if (*b == ' ')
		return 1;
	    }
	  else if (*b < UCHAR_MAX)
	    {
	      if ((*a)^42 < ((toupper(*b))^42))
		return -1;
	      else if (*a == ' ')
		return -1;
	      if ((*a)^42 > ((toupper(*b))^42))
		return 1;
	      else if (*b == ' ')
		return 1;
	    }
	}
      else
	{
	  if (((*a)^42) < ((*b)^42))
	    return -1;
	  else if (*a ==  ' ')
	    return -1;
	  if (((*a)^42) > ((*b)^42))
	    return 1;
	  else if (*b == ' ')
	    return 1;
	}
    }
}

//wrapper function
int compareWrap(const void* a, const void* b)
{
  return compare(*(char**) a, *(char**) b);
}

//check for output error
void writeError(void)
{
  if (ferror(stdout))
    {
      fprintf(stderr, "error while writing to stdout");
      exit(1);
    }
}

//check fo input error
void readError(int s)
{
  if (s < 0)
    {
      fprintf(stderr, "error while reading file");
      exit(1);
    }
}

//check for memory allocation error
void allocError(void)
{
  fprintf(stderr, "error while allocating memory");
  exit(1);
}

int main(int argc, char** argv)
{
  int initialWords = 0;
  struct stat fs;
  if (fstat(0, &fs) < 0)
    {
      fprintf(stderr, "error with fstat");
      exit(1);
    }
  if (argc > 2)
    {
      fprintf(stderr, "wrong number of arguments!");
      exit(1);
    }
  char* word;
  char** wordArray;
  char* fileBuffer;

  int words = 0;
 
  int state;

  //check if regular file has been inputted
  if(S_ISREG(fs.st_mode))
    {
      int fileSize = (int) fs.st_size;
      //create a buffer of that size
      fileBuffer = (char*) malloc(fileSize * sizeof(char));
      int i = 0;
      state = read(STDIN_FILENO, fileBuffer, fileSize);
      readError(state);
      //count number of words in the file
      while (i < fileSize)
	{
	  if (i == fs.st_size-1)
	    fileBuffer[i] = ' ';
	  //account for extra spaces
	  if (fileBuffer[i] == ' ')
	    {
	      while (fileBuffer[i] == ' ')
		i++;
	      words++;
	      continue;
	    }
	  i++;
   	}

      //create words in words array
      //allocate enough for the amount of words we've counted
      wordArray = (char**) malloc((words+2) * sizeof(char*));
      //go through file buffer
      i = 0;
      int EOW = 1;
      int wordPtr = 0;

      while(i<fileSize)
	{
	  if (EOW && fileBuffer[i] !=  ' ')
	    {
	      wordArray[wordPtr] = &fileBuffer[i];
	      wordPtr++;
	      EOW = 0;
	    }
	  if (fileBuffer[i] == ' ')
	    {
	      EOW = 1;
	    }
	  i++;
	}
      initialWords = words;
    }
  else
    {
      wordArray = (char**) malloc(sizeof(char*));
    }

  int letters = 0;
  
  char curr[1];
  char next[1];
  
  word = (char*) malloc(sizeof(char));
  
  state = read(STDIN_FILENO, curr, 1);
  readError(state);
  int nextState = read(STDIN_FILENO, next, 1);
  readError(nextState);

  while (state > 0)
    {
      word[letters] = curr[0];
      char* temp1 = realloc(word, (letters+2) * sizeof(char));
      if (temp1 == NULL)
	{
	  free(wordArray);
	  allocError();
	}
      word = temp1;
      letters++;     
      //end of a word
      if (curr[0] == ' ')
	{
	  //add the word to the wordArray
	  wordArray[words] = word;
	  //constantly allocating more memory
	  char** temp2 = (char**) realloc(wordArray, (words+2) * sizeof(char*));
	  if (temp2 == NULL)
	    {
	      free(wordArray);
	      allocError();
	    }
	  wordArray = temp2;
	  words++;
	  //setting word to NULL will "zero-out" the memory so that a new word can be created
	  word = NULL;
	  word = (char*) malloc(sizeof(char));
	  letters = 0;
	}
      if (nextState == 0)
	{
	  if (curr[0] == ' ')
	    break;
	  if (curr[0] != ' ')
	    {
	      //add extra space if there is not already one
	      curr[0] = ' ';
	      letters++;
	      continue;       
	    }
	}
      if (next[0] == ' ' && curr[0] == ' ')
	{
	  //accounts for spaces in the middle of input
	  while (curr[0] == ' ')
	    {
	      state = read(STDIN_FILENO, curr, 1);
	      readError(state);
	    }
	  nextState = read(STDIN_FILENO, next, 1);
	  readError(nextState);
	  letters++;
	  continue;
	}
      //increment letters
      curr[0] = next[0];
      nextState = read(STDIN_FILENO, next, 1);
      readError(nextState);
    }

  //check if the -f flag is set before sorting
  if (argc == 2 && argv[1][1] == 'f')
    {
      caseSensitive = 0;
    }

  qsort(wordArray, words, sizeof(char*), compareWrap);
  
  int i = 0;
  int j;
  while (i <words)
    {
      j = 0;
      while (wordArray[i][j] != ' ')
	{
	  write(1, wordArray[i]+j, 1);
	  j++;
	}
      write(1, " ", 1);
      i++;
      }
  //free allocated memory
  int a = 0;

  if (initialWords != 0)
    {
      free(fileArray);
    }
  for (i = initialWords; i < words; i++)
    {
      free(wordArray[i]);
    }
  
  free(wordArray);
  
  //  fprintf(stderr, "Comparisons: %u", comp);

  exit(0);


}
