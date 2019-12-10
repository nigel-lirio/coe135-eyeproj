
// C program to implement one side of FIFO 
// This side reads first, then reads 
#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
  
int main() 
{ 
    int fd1; 
  
    // FIFO file path 
    char * myfifo = "/mouse_fifo";
  
    // Creating the named file(FIFO) 
    // mkfifo(<pathname>,<permission>) 
    mkfifo(myfifo, 0666); 
  
    //char str1[80], str2[80]; 
    int x_fifo[2];
    while (1) 
    { 
        // First open in read only and read 
        fd1 = open(myfifo,O_RDONLY); 
        read(fd1, &x_fifo, sizeof(x_fifo)); 
  
        // Print the read string and close 
        printf("User1: %d, %d\n", x_fifo[0], x_fifo[1]); 
        close(fd1);


        //x = 54;
        //y = 45;
        // Now open in write mode and write 
        // string taken from user. 
        //fd1 = open(myfifo,O_WRONLY); 
        //fgets(str2, 80, stdin); 
        //write(fd1, x, sizeof(x)); 
        //close(fd1);

        //write(fd1, y, sizeof(y)); 
        //close(fd1);  


    } 
    return 0; 
} 
