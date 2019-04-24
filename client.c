#include"stdio.h"  
#include"stdlib.h"  
#include"sys/types.h"  
#include"sys/socket.h"  
#include"string.h"  
#include"netinet/in.h" 
#include"unistd.h" 
#include"netdb.h"
#include"arpa/inet.h"
#include"signal.h"
#include "stdbool.h"
#define BUF_SIZE 4096

#include <fcntl.h>
#include <errno.h>


#define PMAX 4096


char * recover_filename(FILE * f) {
int fd;
char fd_path[255];
char * filename = malloc(255);
ssize_t n;

fd = fileno(f);
sprintf(fd_path, "/proc/self/fd/%d", fd);
n = readlink(fd_path, filename, 255);
if (n < 0)
return NULL;
filename[n] = '\0';
return filename;
}

int main(int argc, char *argv[]) {  
struct sockaddr_in addr, cl_addr;  
int sockfd, ret,ret1;  
char buffer[BUF_SIZE];  
struct hostent * server;
char * serverAddr;
char ch;
 int bytes_read = 0;
int bytes_written = 0;
char buf[PMAX] = {0};
char cmd[PMAX] = {0};
FILE *pp = stdin;
char *c = recover_filename(pp);
// printf("%s\n", c);
snprintf (cmd, PMAX, "%s %s", "file", c);
int x=0;


if ((argc != 5) && (argc != 6)) {
printf("Usage: -h sever_host_ip_address -p server_listening_port >\n");
exit(1);  
}

serverAddr = argv[2]; 
// int PORT = argv[4];

sockfd = socket(AF_INET, SOCK_STREAM, 0);  
if (sockfd < 0) {  
printf("Error creating socket!\n");  
exit(1);  
}  
// printf("Socket created...\n");   

memset(&addr, 0, sizeof(addr));  
addr.sin_family = AF_INET;  
addr.sin_addr.s_addr = inet_addr(serverAddr);
addr.sin_port = atoi(argv[4]);     

ret = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));  
if (ret < 0) {  
printf("Error connecting to the server!\n");  
exit(1);  
}  

bytes_written =1; 
memset(buffer, 0, BUF_SIZE); 
if (isatty(fileno(stdin))){
while (fgets(buffer, BUF_SIZE, stdin) != NULL) { 
  
if(buffer[0]==0x0A)
{
  close(sockfd);
  break;
}

ret = sendto(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &addr, sizeof(addr)); 

if (ret < 0) {  
printf("Error sending data!\n\t-%s", buffer);  
exit(1);
}
}
return 0;
}


else if ((pp = popen (cmd, "r"))) 
{
if (fgets (buf, PMAX, pp)){

      pclose (pp);
      
      
      if (strstr (buf, "ASCII")) {
          // printf("%s\n", buf);
          
          // printf("%d\n", x);
        // ret = sendto(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *) &addr, sizeof(addr));
        
          while (fgets(buffer, BUF_SIZE, stdin) != NULL) { 

if(buffer[0]==0x0A)
{
  close(sockfd);
  break;
}
// char tmp[2] = "1";
//     strcat(tmp,buffer);
//     strcpy(buffer,tmp);
//     memmove (buffer, buffer+1, strlen (buffer+1) + 1);
          // printf("%s\n", buffer);
ret = sendto(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &addr, sizeof(addr)); 

if (ret < 0) {  
printf("Error sending data!\n\t-%s", buffer);  
exit(1);
}
}
      }
      else {
          // printf("%s\n", "no"); 
          // printf("%d\n", x); 
          FILE *const fs = fdopen(dup(fileno(stdin)), "rb");
  while (!feof(fs) && bytes_written > 0){
    if(buffer[0]=='\n')
  {
    close(sockfd);
    exit(1);
   break;
  }
  // char tmp[2] = "0";
  //   strcat(tmp,buffer);
  //   strcpy(buffer,tmp);
  //   memmove (buffer, buffer+1, strlen (buffer+1) + 1);
  bytes_read = fread(buffer, 1, BUF_SIZE, stdin);
  if (bytes_read >0){
  if(ferror(fs))
  {
      perror("Error while reading from file.");
      close(sockfd);
      fclose(fs);
      exit(1);
  }
  if (bytes_read < BUF_SIZE){
        bytes_written = write(sockfd, buffer, bytes_read);
        close(sockfd);          
        break;
      }
  bytes_written = write(sockfd, buffer, bytes_read);
}
  else{
    printf("error");
    close(sockfd);
    break;
  }
  }


          return 0;

close(sockfd);
}
}
}
}


  




