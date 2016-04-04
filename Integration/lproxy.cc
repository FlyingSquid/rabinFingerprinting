/* Dinky ugly client-side integration proxy that uses rabinclient.
 * 
 * Based on a (admittedly bad) proxy at 
 *
 * http://godlytalias.blogspot.com/2013/02/simple-proxy-server-using-c.html
 *
 *
 *  So problems here:
 *      - Forking with an open connection presents all kinds of problems. I mean, most importantly that the cache is never really updated.
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

connect:

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

    while(1) {
        cerr << "Waiting to accept" <<endl;
 
        newsockfd=accept(sockfd,(struct sockaddr*)&cli_addr,&clilen);

	    if(newsockfd<0) {
            cerr << errno <<endl;
		    error("Problem in accepting connection with browser");

	    }
        cerr<<"Accepted "<< newsockfd<<endl;

    alive:
        /* Just want to forward the request */
        char buffer[1000];
		bzero((char*)buffer,1000);
        

		if (read(newsockfd,buffer,1000) < 0) {
            cerr <<"READ failed"<<endl;
            close(newsockfd);
            continue;
        }
        cerr << "Read header "<<endl;

        if(strlen(buffer) < 0) {
            send(newsockfd, "ERROR:400\n",10 , MSG_NOSIGNAL);
            goto alive;
        }


        char *alive = strstr(buffer, "Connection: keep-alive");
        /* Have received the request here */
        int n = write(sockfd_, buffer, strlen(buffer));
        if(n < 0) {
	        close(newsockfd);
            close(sockfd_);
            cerr << errno <<endl;
            cerr << "Could not write to proxy"  <<endl;
            exit(0);    //This should go
            goto connect;
        }
        cerr << "Written to proxy" <<endl;
        string tmplte="XXXXXX";
        char *fname = NULL;
        //fname  = mktemp((char *)tmplte.c_str());
        if(fname == NULL) {
                fname = new char[20];
                strcpy(fname, "temporary.file");
        }

        FILE* tmp = fopen(fname, "w+");
        rabin_c.receive_file(tmp);
        cerr << "Received file" <<endl;
            
        fseek(tmp, 0L, SEEK_END);
         n = ftell(tmp);
         fseek(tmp, 0L, SEEK_SET);
         fflush(tmp);
         char c[n];
         bzero((char *)c, n);
         fread(c, n, 1, tmp);
            /************************/    
            
        cerr<<"READY TO SEND"<<endl;
        if(send(newsockfd, c, n, MSG_NOSIGNAL) < 0) {
            cerr <<"Send ERROR, possibly hit browser timeout"<<endl;
            continue;
        }


         fclose(tmp);
         remove((const char *) fname);
         cerr << "Served request" <<endl;
         close(sockfd_);
         /* Figure out what to close here */
         if(alive != NULL) {
            cerr << "Alive" <<endl;
            cerr <<"\n"<<endl;
            goto alive;
         }

            cerr <<"Closing browser connection\n"<<endl;
	        close(newsockfd);
	}

    (void) addr_in;

	return 0;
}
