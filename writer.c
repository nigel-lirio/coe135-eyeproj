
// C program to implement one side of FIFO 
// This side writes first, then reads 
#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
  
int main() 
{ 
    int fd; 
  
    // FIFO file path 
    char * myfifo = "/mouse_fifo"; 
  
    // Creating the named file(FIFO) 
    // mkfifo(<pathname>, <permission>) 
    mkfifo(myfifo, 0666); 
  
    //char arr1[80], arr2[80];
    int x = 99;
    int y = 100;
    while (1) 
    { 
        // Open FIFO for write only 
        fd = open(myfifo, O_WRONLY); 
  
        // Take an input arr2ing from user. 
        // 80 is maximum length 
       //fgets(arr2, 80, stdin); 

       //scanf("%d", &x_value)
       //scanf("")
  
        // Write the input arr2ing on FIFO 
        // and close it 
        //write(fd, arr2, strlen(arr2)+1); 
        write(fd, &x, sizeof(x)); 
        

        close(fd); 

        open(myfifo, O_WRONLY);

        write(fd, &y, sizeof(y));

        close(fd);
  
        // Open FIFO for Read only 
        //fd = open(myfifo, O_RDONLY); 
  
        // Read from FIFO 
        //read(fd, &y, sizeof(y)); 
  
        // Print the read message 
        //printf("User2: %d\n", y); 
        //close(fd); 
    } 
    return 0; 
} 
