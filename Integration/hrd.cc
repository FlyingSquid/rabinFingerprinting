/* Dinky ugly client-side header reader.
 * 
 * http://godlytalias.blogspot.com/2013/02/simple-proxy-server-using-c.html
 *
 *
 *
*/


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netdb.h>
#include<errno.h>
#include "../Client/rabinclient.h"

void error(string msg)
{
	perror(msg.c_str());
	exit(0);
}
  
int main(int argc,char* argv[])
{
	struct sockaddr_in addr_in,cli_addr,serv_addr;

	int sockfd,newsockfd;
	   
	if(argc<2)
    	error("./lproxy <port_no> <sproxy_hostname> <sproxy_port> <rabinserver_port>");

	bzero((char*)&serv_addr,sizeof(serv_addr));
	bzero((char*)&cli_addr, sizeof(cli_addr));
	   
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(atoi(argv[1]));
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	   
	  
	sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sockfd<0)
		error("Problem in initializing socket");
	   
	if(bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
		error("Error on binding");
	  
	  
	listen(sockfd,50);
	socklen_t clilen=sizeof(cli_addr);
    

    int proxy_sock_fd;

    struct hostent *proxy = gethostbyname(argv[2]);
    if(proxy == NULL)
        error("ERROR: Proxy no such host");

    int proxy_port = atoi(argv[3]);
    int rabin_port = atoi(argv[4]);

    struct sockaddr_in proxy_addr;

    bzero((char *) &proxy_addr, sizeof(proxy_addr));
           proxy_addr.sin_family = AF_INET;
    
    bcopy((char *) proxy->h_addr,
          (char *)&proxy_addr.sin_addr.s_addr,
          proxy->h_length);
    proxy_addr.sin_port = htons(proxy_port);


    /* The Rabin connection will be persistent for now,
     * potential robustness imporvement if it can be reopened */ 
    RabinClient rabin_c(argv[2], rabin_port);
    rabin_c.connect_to_server();
    

  while(1) {
      cerr<<"Ready"<<endl;
      proxy_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
      newsockfd=accept(sockfd,(struct sockaddr*)&cli_addr,&clilen);

	  if(newsockfd<0) {
		    error("Problem in accepting connection with browser");
	  }
      fprintf(stderr,"Accepted %d\n",newsockfd);

        /* Just want to forward the request */
        char buffer[1000];
		bzero((char*)buffer,1000);

		if (read(newsockfd,buffer,1000) < 0) {
            error("READ failed");
        }
        printf("Header was:\n\n %s\n",buffer);

        cerr <<"Trying to connect to proxy"<<endl;

      
      if(connect(proxy_sock_fd, (struct sockaddr *) &proxy_addr,
                    sizeof(proxy_addr)) < 0) {
            cerr << "Could not connect to proxy "<<errno << endl;

            send(newsockfd, "<html><h1> Could not connect to the proxy </h1></html>",54 , MSG_NOSIGNAL);
            close(newsockfd);
            continue;
            
      } else {
            cerr << "Connected to proxy, now forwarding header"<<endl;
      }
      int n = write(proxy_sock_fd, buffer, strlen(buffer));

    if(n <= 0) {
         send(newsockfd, "<html><h1> Could not write to the proxy </h1></html>",52 , MSG_NOSIGNAL);
         cerr << "Write failed" << n <<endl;
        continue;
	}
    cerr << "Now receiving"<<endl;


/*    do {
		bzero((char*)buffer,1000);
		n=recv(proxy_sock_fd,buffer,1000,0);
	    cerr << buffer <<endl;	
        if(n > 0) {
            send(newsockfd, buffer, n, MSG_NOSIGNAL);
		}
	} while(n>0);*/


    unsigned blocks_recvd = 0;
    do {
        FILE*tmp = fopen("temporary.file", "w+");
        blocks_recvd = rabin_c.receive_file(tmp);
        if(blocks_recvd > 1) {
            /* read the response into a buffer c */
            fseek(tmp, 0L, SEEK_END);
            n = ftell(tmp);
            fseek(tmp, 0L, SEEK_SET);
            char c[n];
            bzero((char *)c, n);
            fread(c, n, 1, tmp);
            /*************************************/
            cerr<<"Sending"<<endl;
            send(newsockfd, c, n, MSG_NOSIGNAL);
        }
    } while (blocks_recvd > 1); // 1 block will be used to denote eof

    close(proxy_sock_fd);
    close(newsockfd);

   }

    (void) addr_in;

	return 0;
}
