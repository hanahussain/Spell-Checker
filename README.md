# Spell Checker
Spell checker using hashing in C++.

### Purpose
The program prompts the user for a word. If the word exists in the dictionary, the program responds with "True" and suggests words from the dictionary starting with the same first two letters. At the end of each suggestion the execution time (in micro-sec) is displayed.

If the input word does not exist in the dictionary due to user misspelling but is similar to an existing word, the program returns a recommended word from the dictionary. 

If the input word does not exist in the dictionary, the program responds with "False".

The dictionary is placed in a hashmap using a custom hash function to minimize search time. 

### Running Instructions
Download the .zip file and extract all the files. In a Linux environment (Windows PowerShell, Git Bash, etc), enter the command `make run` to start the program. 
Between runs, you may enter the command `make clean` to clean out output files.
