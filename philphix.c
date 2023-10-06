/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philphix.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This hash table stores the dictionary.
 */
HashTable *dictionary;

/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in 
 * the grading process.
 */
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Specify a dictionary\n");
    return 1;
  }
  /*
   * Allocate a hash table to store the dictionary.
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(0x61C, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();

  /*
   * The MAIN function in C should always return 0 as a way of telling
   * whatever program invoked this that everything went OK.
   */
  return 0;
}

/*
 * This should hash a string to a bucket index.  void *s can be safely cast
 * to a char * (null terminated string)
 */
unsigned int stringHash(void *s){
  char *string = (void *) s;
  /* Printing string is to prevent a compiler warning until you
     actually implement this function */
  /* fprintf(stderr, "need to implement stringHash%s\n", string); */

  unsigned int hashCode = 0;
  unsigned int coef = 1;
  while (*string) {
    hashCode += coef * (*string);
    coef *= 31;
    string++;
  }
  // printf("%s %d\n", (char*)s, hashCode);
  return hashCode;
}

/*
 * This should return a nonzero value if the two strings are identical 
 * (case sensitive comparison) and 0 otherwise.
 */
int stringEquals(void *s1, void *s2) {
  // -- TODO --
  char *string1 = (char *)s1;
  char *string2 = (char *)s2;
  // -- TODO --
  if(strcmp(string1, string2) == 0) {
    return 1;
  } else {
    return 0;
  }
}

/*
 * This function should read in every word and replacement from the dictionary
 * and store it in the hash table.  You should first open the file specified,
 * then read the words one at a time and insert them into the dictionary.
 * Once the file is read in completely, return.  You will need to allocate
 * (using malloc()) space for each word.  As described in the spec, you
 * can initially assume that no word is longer than 60 characters.  However,
 * for the final bit of your grade, you cannot assumed that words have a bounded
 * length.  You CANNOT assume that the specified file exists.  If the file does
 * NOT exist, you should print some message to standard error and call exit(61)
 * to cleanly exit the program.
 */
void readDictionary(char *name){
  FILE *inputFile = fopen(name, "r");
  if (!inputFile) {
    fprintf(stderr, "Error: open dictionary file failed: %s\n", name);
    exit(1);
  }
  char key[1024], value[1024];
  while (fscanf(inputFile, " %1023s", key) == 1) {
      if (fscanf(inputFile, " %1023s", value) == 1) {
        fprintf(stderr, "readDictionary: insert (%s, %s)\n", key, value);
        insertData(dictionary, key, value);
      }
  }
  fclose(inputFile);
}

/*
 * This should process standard input (stdin) and perform replacements as 
 * described by the replacement set then print either the original text or 
 * the replacement to standard output (stdout) as specified in the spec (e.g., 
 * if a replacement set of `taest test\n` was used and the string "this is 
 * a taest of  this-proGram" was given to stdin, the output to stdout should be 
 * "this is a test of  this-proGram").  All words should be checked
 * against the replacement set as they are input, then with all but the first
 * letter converted to lowercase, and finally with all letters converted
 * to lowercase.  Only if all 3 cases are not in the replacement set should 
 * it report the original word.
 *
 * Since we care about preserving whitespace and pass through all non alphabet
 * characters untouched, scanf() is probably insufficent (since it only considers
 * whitespace as breaking strings), meaning you will probably have
 * to get characters from stdin one at a time.
 *
 * Do note that even under the initial assumption that no word is longer than 60
 * characters, you may still encounter strings of non-alphabetic characters (e.g.,
 * numbers and punctuation) which are longer than 60 characters. Again, for the 
 * final bit of your grade, you cannot assume words have a bounded length.
 */
void processInput()
{
    // -- TODO --
    char *str1 = (char *)malloc(70);
    char *str2 = (char *)malloc(70);
    char *str3 = (char *)malloc(70);
    int c = 0;
    int i = 0;
    int total = 70;

    while ((c = fgetc(stdin)) != EOF)
    {

        if (isalpha(c) != 0)
        {
            if (i == total)
            {
                str1 = (char *)realloc(str1, total * 2);
                str2 = (char *)realloc(str2, total * 2);
                str3 = (char *)realloc(str3, total * 2);
                total = total * 2;
            }
            str1[i] = (char)c;
            if (i == 0)
            {
                str2[i] = (char)c;
            }
            else
            {
                str2[i] = (char)tolower(c);
            }
            str3[i] = (char)tolower(c);
            i++;
        }
        else
        {
            if (isalpha(str1[0]))
            {
                str1[i] = '\0';
                str2[i] = '\0';
                str3[i] = '\0';
                if (findData(dictionary, str1) == NULL && findData(dictionary, str2) == NULL && findData(dictionary, str3) == NULL)
                {
                    fprintf(stdout, "%s [sic]%c", str1, c);
                }
                else
                {
                    fprintf(stdout, "%s%c", str1, c);
                }
            }
            else
            {
                fprintf(stdout, "%c", c);
            }
            i = 0;
            memset(str1, 0, strlen(str1));
            memset(str2, 0, strlen(str2));
            memset(str3, 0, strlen(str3));
        }
    }
    if (isalpha(str1[0]))
    {
        str1[i] = '\0';
        str2[i] = '\0';
        str3[i] = '\0';
        if (findData(dictionary, str1) == NULL && findData(dictionary, str2) == NULL && findData(dictionary, str3) == NULL)
        {
            fprintf(stdout, "%s [sic]", str1);
        }
        else
        {
            if (strlen(str1) != 0)
            {
                fprintf(stdout, "%s", str1);
            }
        }
    }
    free(str1);
    free(str2);
    free(str3);
}
