#include <stdio.h>  
#include <stdlib.h>  
  
int  
main(void)  
{  
   char *line = NULL;  
   size_t len = 0;  
   ssize_t read;  
  
   while ((read = getline(&line, &len, stdin)) != -1) {  
       printf("Retrieved line of length %zu, %u :\n", read, len);  
       printf("%s", line);  
   }  
  
   free(line);  
   exit(EXIT_SUCCESS);  
} 