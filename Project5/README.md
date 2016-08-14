#File System Checker

To run the program:

1. `gcc -O -Wall -o fscheck fscheck.c`
2. `./fscheck file_image`

##Workflow:
1. The checker will first find the start of each segments, and initialize a new bitmap that is used for checking bits that is unused but marked used.
2. Check is root directory.
3. Use a Deep First Search algorithm to iterate through all the files in the directory. We could check the inconsistency between directory and inodes, including inode type, format and recording the reference count. Also, the address checking and data block usage recording will be handled during DFS by looking through all the addresses in the `addrs[]`.
4. After DFS, the checker will go through the inode map to check errors related to references. The last thing it will check is comparing two bitmaps to see if the block is marked in use but not be used at all. Note that for indirect blocks, it will first use a block to record all the addresses, then use all the recorded blocks. Don't forget to mark the address block in use!
