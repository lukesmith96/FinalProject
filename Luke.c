#include "Luke.h"

inline void InitLuke(Luke *luke) {
   luke->dFlag = 0;
   luke->fFlag = 0;
   luke->tFlag = 0;

   luke->tValue = calloc(1, 1);
   luke->file = calloc(1, 1);
   luke->value = calloc(1, 1);
}

void PrintVals(Luke *luke) {
   printf("Value: %s\n", luke->value);
   printf("tValue: %s\n", luke->tValue);
   printf("File: %s\n", luke->file);
   printf("Flags: d%d f%d t%d\n", luke->dFlag, luke->fFlag, luke->tFlag);
}

/*
 * AddString:
 * adds a string to a destination list dynamically
 *                                                 
 * Param <dest>: destination of the new string to add
 * Param <source>: source of the string that is to be added
 * Param <size>: current size of the dest list;            
 *                                                         
 * Return: dest, the new list of strings with the added string
 */                                                           
char **AddString(char **dest, char *source, int size, int *limit) {       
   char **dtemp;                                              
   int length = strlen(source);
   
   if (size == *limit) {
      // duplicate destination to increase amount of string able to be held
      dtemp = dest;                                                        
      *limit = size * size;
      dest = calloc(*limit + 1, sizeof(char *));                                                
      memcpy(dest, dtemp, size * sizeof(char *));                       
      free(dtemp);                                                                  
      // add one string to the destination. int length = strlen(source);
   }
   dest[size] = calloc(length + 1, sizeof(char));                       
   strcpy(dest[size], source);                                       
   return dest;                                                         
}

void ParseInput(int argc, char *argv[], Luke *luke) {
   int argIter;
   
   if (fopen(argv[argc - 1], "r")) {
      luke->file = calloc(strlen(argv[argc - 1]) + 1, sizeof(char));
      // it has to be a file name
      strcpy(luke->file, argv[argc - 1]);
   }
   
   for (argIter = 1; argIter < argc; argIter++) {
      // If it is a tag
      if (*argv[argIter] == '-') {
         if (*(1 + argv[argIter]) == 'd') {
            luke->dFlag = 1;
            if (*(2 + argv[argIter]) == 'f')
               luke->fFlag = 1;
         }
         if (*(1 + argv[argIter]) == 'f') {
            luke->fFlag = 1;
            if (*(2 +argv[argIter]) == 'd')
               luke->dFlag = 1;
         }
         if (*(1 + argv[argIter]) == 't') {
            luke->tFlag = 1;
            argIter++;
            if ((argc - (*(luke->file) != 0)) >  argIter) {
               luke->tValue = calloc(strlen(argv[argIter]) + 1, 
                sizeof(char));
               strcpy(luke->tValue, argv[argIter]);
            }
         }
      }
      else if (*(luke->value) == 0) {
         luke->value = calloc(strlen(argv[argIter]) + 1, sizeof(char));
         strcpy(luke->value, argv[argIter]);
      }
      else if (*(luke->file) == 0) {
         printf("usage: look [-df] [-t char] string [file]\n");
         exit(EXIT_FAILURE);
      }
   }
}

void InputValidation(Luke *luke) {
   int exitVal = 0;
   char *stddict = "/usr/share/dict/words";

   if (luke->tFlag && (int)*(luke->tValue) == 0) {
      printf("look: option requires an argument -- \'t\'\n");
      exitVal++;
   }
   if (*(luke->value) == 0)
      exitVal++;
   if (*(luke->file) == 0) {
      luke->dFlag = 1;
      luke->fFlag = 1;
      luke->file = calloc(strlen(stddict) + 1, sizeof(char));
      strcpy(luke->file, stddict);
   }
   if (exitVal) {
      printf("usage: look [-df] [-t char] string [file]\n");
      exit(EXIT_FAILURE);
   }
}

void EditSearchValue(Luke *luke) {
   char *first, *upCase, edit = *(luke->tValue);
   
   upCase = first = strchr(luke->value, edit);
   
   if (luke->fFlag) {
      edit = (toupper(edit) < edit) ? toupper(edit) : tolower(edit);
      upCase = strchr(luke->value, edit);
   }
   if (first < upCase && first != NULL)
      *(first + 1) = '\0';
   else if (upCase != NULL)
      *(upCase + 1) = '\0';
}

char *CheckAlphaNumeric(char *source) {
   char *copy, *str;
   
   copy = str = calloc(strlen(source) + 1, 1);
   
   // go through str and check if there is a space in copy 
   // then overwrite the original pointer
   while (*source) {
      if ((*source >= 'A' && *source <= 'Z') || (*source >= 'a' 
       && *source <= 'z') || (*source >= '0' && *source <= '9')) {
         *str++ = *source;
      }
      source++;
   }
   //end new string
   *str = '\0';
   return copy;
}

char **ReadFile(int *size, Luke  *luke) {
   char **text, *curLine, *map, *file;
   struct stat st;
   int len = 0, *limit, fd;
   
   fd = open(luke->file, O_RDONLY);
   stat(luke->file, &st);
   map = mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
   
   file = calloc(sizeof(char), st.st_size + 1);
   memcpy(file, map, st.st_size);

   text = calloc(sizeof(int), DCALLOC);
   limit = calloc(sizeof(int), 1);
   *limit = DCALLOC;

   curLine = strtok(file, "\n");
   while ((curLine) != NULL) {
      text = AddString(text, curLine, len++, limit);
      curLine = strtok(NULL, "\n");
   }

   munmap(map, st.st_size);
   close(fd);
   *size = len;
   return text;
}

int CompareHelper(Luke *luke, char *comp) {
   if (luke->fFlag)
      return strncasecmp(luke->value, comp, strlen(luke->value));
   else
      return strncmp(luke->value, comp, strlen(luke->value));
}

void ExecuteActions(Luke *luke, char **text, int lower, int upper, int max) {
   int middle, comp = 0;
   char *toComp;
   
   middle = lower + ((upper - lower) / 2);
   toComp = calloc(strlen(text[middle]) + 1, 1);
   strcpy(toComp, text[middle]);
   if (luke->dFlag) {
      toComp = CheckAlphaNumeric(toComp);
   }
   comp = CompareHelper(luke, toComp);
   
   if (comp == 0) { // they are equal
      int start = middle;
      
      while (CompareHelper(luke, luke->dFlag ?
       CheckAlphaNumeric(text[start]) : text[start]) == 0)
         start--;
      start++;
      while ((start < max) && (CompareHelper(luke, luke->dFlag ? 
       CheckAlphaNumeric(text[start]) : text[start]) == 0)) {
         printf("%s\n", text[start]);
         start++;
      }
   } 
   else if (comp > 0 && lower != middle) { // if value is greater then text
      ExecuteActions(luke, text, middle, upper, max);
   }
   else if (lower != middle) { // if value is less then text
      ExecuteActions(luke, text, lower, middle, max);
   }
}

int main(int argc, char *argv[]) {
   Luke *luke; 
   char **fileText;
   int *size = calloc(sizeof(int), 1);
   
   fileText = calloc(sizeof(int), 1);
   luke = calloc(sizeof(Luke), 1);
   *size = 0;
   
   InitLuke(luke);
   ParseInput(argc, argv, luke);
   InputValidation(luke);
   fileText = ReadFile(size, luke);
   if (luke->tFlag)
      EditSearchValue(luke);
   if (luke->dFlag)
      luke->value = CheckAlphaNumeric(luke->value);
   ExecuteActions(luke, fileText, 0, *size, *size);
   return 0;
}
