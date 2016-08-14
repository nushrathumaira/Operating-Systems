#Fast sort
This program could sort the lines in the files according to the specified position of word. If a line doesn't have enough words, it will simply use the last word.

To run the program:

1. Type `gcc -O -Wall -o fastsort fastsort.c`.
2. Type `./fastsort [-#] filename`, where `[-#]` is optional, and `#` indicates the picked position in a line. If not presented, the first word of each line will be picked.

##Workflow
1. Check if the arguments are legal. 
2. Go through the file one time to count the line number, and read the file again to copy the line content and one of the word that is identified by the parameter (or the first word).
3. Use the built-in `qsort()` function to sort the array. 
4. Print the output according to the sorting result.
