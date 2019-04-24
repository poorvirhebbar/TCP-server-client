 
#include"stdio.h"
#include"stdlib.h"
#include"unistd.h"

#include"sys/types.h"
#include"sys/socket.h"
#include"string.h"
#include"netinet/in.h"
#include"netdb.h"
#include"arpa/inet.h"
#include"signal.h"

// #define PORT 1234
#define BUF_SIZE 4096
#define CLADDR_LEN 1000

int main(int argc, char *argv[]) {
 // printf("YOlobroo");
 struct sockaddr_in addr, cl_addr;
 int sockfd, len, ret, ret1,newsockfd,n;
 char buffer[BUF_SIZE];
 pid_t childpid;
 char clientAddr[CLADDR_LEN];
 int bytes_read = 0;

  if (argc != 3) {
  printf("Usage: ./server -p listening_port>\n");
  exit(1);  
 }

 sockfd = socket(AF_INET, SOCK_STREAM, 0);
 if (sockfd < 0) {
  printf("Error creating socket!\n");
  exit(1);
 }

 memset(&addr, 0, sizeof(addr));
 addr.sin_family = AF_INET;
 addr.sin_addr.s_addr = INADDR_ANY;
 addr.sin_port = atoi(argv[2]);
 
 ret = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
 if (ret < 0) {
  printf("Error binding!\n");
  exit(1);
 }
 // printf("Binding done...\n");

 // printf("Waiting for a connection...\n");
 listen(sockfd, 5);

 for (;;) { //infinite loop
  len = sizeof(cl_addr);
  newsockfd = accept(sockfd, (struct sockaddr *) &cl_addr, &len);
  if (newsockfd < 0) {
   printf("Error accepting connection!\n");
   exit(1);
  }

  inet_ntop(AF_INET, &(cl_addr.sin_addr), clientAddr, CLADDR_LEN);
  if ((childpid = fork()) == 0) { 

   close(sockfd); 


   for (;;) {
    memset(buffer, 0, BUF_SIZE);


FILE *const fr = fdopen(dup(fileno(stdout)), "w");
// FILE *request = fdopen(newsockfd, "wb");

while((bytes_read = read(newsockfd,buffer,BUF_SIZE)) > 0)
{	
	// printf("%s", buffer);
   fwrite(buffer, 1, bytes_read, stdout);
   if(ferror(fr))
   {
       perror("Error while writing to file");
       close(newsockfd);
       fclose(fr);
       exit(1);
   }
   memset(buffer, 0, BUF_SIZE);
}
if(bytes_read < 0)
    printf("error reading data");
    }
  }
  close(newsockfd);
 }
 return 0;
}

