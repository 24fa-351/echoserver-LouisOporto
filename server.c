#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>

// Usage: ./server <port>
int main(int argc, char* argv[]) {

  if (argc < 2) {
    printf("Usage: %s <port>\n", argv[0]);
    return 1;
  }

  printf("Starting server...\n");
  

  
  return 0;
}