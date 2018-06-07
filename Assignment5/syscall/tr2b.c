/*
Name: Baolinh Nguyen
TA: Farnoosh Javadi
CS 35L Lab 6
Assignment 5
File: tr2b.c
use getchar() and putchar() 
*/

#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char** argv)
{
  //checking if correct number of arguments are passed
  if (argc != 3)
    {
      fprintf(stderr, "incorrect number of arguments");
      exit(1);
    }
  
  char* from = argv[1];
  char* to = argv[2];
  int toLen = strlen(to);
  int fromLen = strlen(from);

  //checking if length of to and from are the same
  if (toLen != fromLen)
    {
      fprintf(stderr, "from and to are not the same length");
      exit(1);
    }

  //checking for duplicates in from
  int i = 0;
  for (i = 0; i < fromLen; i++)
    {
      int j = 0;
      for (j = 1; j < fromLen; j++)
	{
	  if (i != j && from[i] == from[j])
	    {
	      fprintf(stderr, "from has duplicate bytes");
	      exit(1);
	    }
	}
    }

  char curr = getchar();
  while (!feof(stdin))
    {
      //flag indicates whether the character was found in the 'from' string
      int flag = 0;
      //checks if it is in the from string
      for (i = 0; i < fromLen; i++)
	{
	  if (curr == from[i])
	    {
	      putchar(to[i]);
	      flag = 1;
	      break;
	    }
	}
      if (!flag)
	putchar(curr);
      curr = getchar();
    }
}
