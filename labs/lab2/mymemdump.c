
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void mymemdump(FILE * fd, char * p , int len) {
    // Add your code here.
    // You may see p as an array.
    // p[0] will return the element 0 
    // p[1] will return the element 1 and so on
    
    //int count = 0;
    //while ((count * 8) < len) {
    
    fprintf(fd, "0x%016lX: , LENGTH= %d", (unsigned long) p, len);
    for (int i = 0; i < len; i++) {
      int c = p[i]&0xFF;
        
      // Print first byte as hexadecimal
      fprintf(fd, "%02X ", c);
    }
    for (int i = 0; i < len; i++) {
      int c = p[i]&0xFF;
      // Print first byte as character
      if (c>=127)
        fprintf(fd, "%c", '.');
      else
      fprintf(fd, "%c", (c>=32)?c:'.');
    }
    //count++;
    fprintf(fd,"\n");
    //}
}
