#include <stdio.h>
#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
using namespace std;
     
int main()
{
  key_t key = ftok("shmfile",65);
  //Display *dpy = XOpenDisplay(0);
  //while (1) {
    int x;
    
    // ftok to generate unique key 
  
    // shmget returns an identifier in shmid 
    //int shmid = shmget(key,1024,0666|IPC_EXCL); 
  
    // shmat to attach to shared memory 
    //int *str = (int*) shmat(shmid,(void*)0,0); 
    int shmid = shmget(key,1024,0666|IPC_CREAT); 
  
    // shmat to attach to shared memory 
    char *str = (char*) shmat(shmid,(void*)0,0); 
  
    printf("Data read from memory: %s\n",str); 
      
    //detach from shared memory  
    shmdt(str); 
    
    // destroy the shared memory 
    shmctl(shmid,IPC_RMID,NULL); 
     
    printf("Data read from memory: %d\n",str[0]); 

    // Window root_window;
    // root_window = XRootWindow(dpy, 0);
    // XSelectInput(dpy, root_window, KeyReleaseMask);
    // XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, x, 100);
    // XFlush(dpy);
    // XSync(dpy, False);
  //}
}