# FinalProject

Developed By:
   Luke Smith

Files:
   Luke.c
   Luke.h
   Makefile

Compiling Instructions:
   This program contains a makefile, simply call the command make in the directory 
   containing all the files listed. 

Basis:
   This program is mimic of the command unix command called "look". Name Luke after 
   the creater it follows the same spec as look including everything but the -a flag
   functionality. 

   You can find the man page for look by calling this command:
      man look

Running:
   To run, first compile the program using the above instructions. Next you will find
   a new file in the directory called "Luke" this is the executable. 

   The format to run the executable:
      ./Luke [-dfa] [-t termchar] string [file]

Description:
   The Luke utility display lines in a file that begin with the requested string. It
   preforms this search using a binary search algorithm that requires the file to be 
   previously sorted.

   If there is no specified file to look through, the defaut file is used. You can
   find it here: /usr/share/dict/words and only alphanumeric characters are used and
   the cases are ignored.

   Flags:
   
   -d    Dictionary character set and order, i.e. only alphanumeric characters are 
         compared. (On by default if no file specified).

   -f    Ignore the case of alphabetic characters. (On by default if no file 
         specified).

   -t    Specify a string termination character, i.e. only the characters in string 
         up to and including the first occurrence of termchar are compared.

Example Unix Commands:

   ./Luke hello -d -t l testfile
      seaches the file "testfile" for the word hel : hello shortened to the first l
      specified by the -t flag and the l following. Also this search ignores non
      aplha numeric characters.
   
   ./Luke test
      searches the default dictionary for the word test, since there is no specified
      file has the -d and -f flags on, alphanumeric character only and ignoring case
      respectivly.

   ./Luke test -f testfile
      searches the testfile for words matching test ignoring case.
