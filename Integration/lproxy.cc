/* Dinky ugly client-side integration proxy that uses rabinclient.
 * 
 * Based on a (admittedly bad) proxy at 
 *
 * http://godlytalias.blogspot.com/2013/02/simple-proxy-server-using-c.html
 *
 *
 *  So problems here:
 *      - Forking with an open connection presents all kinds of problems
 *      - This doesn't actually ask the other server for anything
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
	pid_t pid;
	struct sockaddr_in addr_in,cli_addr,serv_addr;

	int sockfd,newsockfd;
	   
	if(argc<5)
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

    /* Connecting to the actual proxy */
    int sockfd_;
    struct hostent *server_;
    struct sockaddr_in serv_addr_;

 
    int portno = atoi(argv[3]);
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd_ < 0)
            error("ERROR opening socket\n"); 
    
    server_ = gethostbyname(argv[2]);
    if(server_ == NULL) {
            error("ERROR: Proxy No such host\n");
            exit(0);
    }
	bzero((char *) &serv_addr_, sizeof(serv_addr_));
    serv_addr_.sin_family = AF_INET;
    bcopy((char *) server_->h_addr,
          (char *)&serv_addr_.sin_addr.s_addr,
          server_->h_length);
   serv_addr_.sin_port = htons(portno);
   if(connect(sockfd_, (struct sockaddr *) &serv_addr_, sizeof(serv_addr_)) < 0)
            error("Error connecting to proxy");
   else
       cerr << "Connected to proxy"<<endl;
    /************************************************************************/
 
    /*Connected to rabin client */
	int rport = atoi(argv[4]);
    RabinClient rabin_c(argv[2], rport);
    rabin_c.connect_to_server();
	cerr << "Connected to rabinserver" <<endl;
    /*****************************************/

    accepting:
    sleep(5);

    newsockfd=accept(sockfd,(struct sockaddr*)&cli_addr,&clilen);
	   
	if(newsockfd<0)
		error("Problem in accepting connection");
	
    
  
	pid=fork();
	/* This is ugly, it creates a new process for every connection + uses gotos to do this
	* kinda crappy programming, should iterate and use 'select' but whatever.*/
	if(pid==0)
	{
        /* Just want to forward the request */
        char buffer[1000];
		bzero((char*)buffer,1000);
		recv(newsockfd,buffer,1000,0);
        /* Have received the request here */
		
	    //send(newsockfd,"400 : TESTING THIS WILL BE DELETED\n",18,0);

        /* I want to forward buffer to the server side proxy, then code below: */
        int n = write(sockfd_, buffer, strlen(buffer));
        if(n < 0) {
            rabin_c.~RabinClient(); 
	        close(newsockfd);
	        close(sockfd);
            close(sockfd_);
            error("Could not connect to proxy");
        }
        /*** Now getting rabin response ***/
            string tmplte="XXXXXX";
            char *fname = NULL;
            //fname  = mktemp((char *)tmplte.c_str());
            if(fname == NULL) {
                fname = new char[20];
                strcpy(fname, "temporary.file");
            }
            FILE* tmp = fopen(fname, "w+");
            rabin_c.receive_file(tmp);
            
            fseek(tmp, 0L, SEEK_END);
            n = ftell(tmp);
            fseek(tmp, 0L, SEEK_SET);
            fflush(tmp);
            char c[n];
            bzero((char *)c, n);
            fread(c, n, 1, tmp);
            /************************/    
            
            send(newsockfd, c, n, 0);
            fclose(tmp);
            remove((const char *) fname);
            cerr << "Served request" <<endl;
            rabin_c.~RabinClient(); 
	        close(newsockfd);
	        close(sockfd);
            close(sockfd_);
	        _exit(0);
	}
	else
	{
	close(newsockfd);
	goto accepting;
	}

    (void) addr_in;

	return 0;
}
