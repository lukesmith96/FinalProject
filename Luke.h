#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <ctype.h>

#define DCALLOC 100

typedef struct Luke {
   int dFlag;
   int fFlag;
   int tFlag;
   char *tValue;
   char *file;
   char *value;
} Luke;

inline void InitLuke(Luke *luke);

void PrintVals(Luke *luke);

char **AddString(char **dest, char *source, int size, int *limit);

void ParseInput(int argc, char *argv[], Luke *luke);

void InputValidation(Luke *luke);

void EditSearchValue(Luke *luke);

char *CheckAlphaNumeric(char *source);

char **ReadFile(int *size, Luke  *luke);

int CompareHelper(Luke *luke, char *comp);

void ExecuteActions(Luke *luke, char **text, int lower, int upper, int max);
