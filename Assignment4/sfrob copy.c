/*
ASSIGNMENT 4
NAME: Baolinh Nguyen
TA: Farnoosh Javadi
UID: 104732121
CS35L Lab 6
FILE: sfrop.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int compare(const char* a, const char* b)
{
  //return -1 if a < b
  //return 0 if a == b
  //return 1 if a > b
  
  //look at the entire word
  for (;;a++, b++)
    {
      if (*a == ' ' && *b == ' ')
	  return 0;
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
      fprintf(stderr, "Error while writing to stdout");
      exit(1);
    }
}

//check for input error
void readError(void)
{
  if (ferror(stdin))
    {
      fprintf(stderr, "Error while reading file");
      exit(1);
    }
}

//check for memory allocation error
void allocError(void)
{
  fprintf(stderr, "Error while allocating memory");
  exit(1);
}
int main(void)
{
  //word is an array of pointers to characters
  char* word = (char*) malloc(sizeof(char));
  //wordArray is an array of words (pointers to words)
  char** wordArray =  (char**) malloc(sizeof(char*));
  char curr = getchar();
  readError();
  char next = getchar();
  readError();
  int words = 0;
  int letters = 0;

  while (curr != EOF && !ferror(stdin))
    {
      //creates the word
      word[letters] = curr;
      //constantly reallocating more memory
      char* temp1 =  realloc(word, (letters+2) * sizeof(char));
      if (temp1 == NULL)
	{
	  free(word);
	  allocError();
	}
      word = temp1;
      letters++;

      //end of a word
      if (curr == ' ')
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

      if (next == EOF)
	{
	  if (curr == ' ')
	    break;
	  if (curr != ' ')
	    {
	      //add extra space if there is not already one
	      curr = ' ';
	      letters++;
	      continue;
	    }
      	}
      if (next == ' ' && curr == ' ')
	{
	  //accounts for spaces in the middle of input
	  while (curr == ' ')
	    {
	      curr = getchar();
	      readError();
	    } 
	  next = getchar();
	  readError();
	  letters++;
	  continue;
	  }
      //increment letters
      curr = next;
      next = getchar();
      readError();
    }

  qsort(wordArray, words, sizeof(char*), compareWrap);

  //output results of sort
  int i = 0;
  int j = 0;
  while (i < words)
    {
      j = 0;
      while (wordArray[i][j] != ' ')
	{
	  putchar(wordArray[i][j]);
	  writeError();
	  j++;
	}
      putchar(' ');
      writeError();
      i++;
    }

  //free all allocated memory
  int a = 0;
  for (a = 0; a < words; a++)
    free(wordArray[a]);
  free(wordArray);

  exit(0);
 
}
