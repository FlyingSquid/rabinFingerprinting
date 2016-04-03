/* Dinky ugly client-side integration proxy that uses rabinclient.
 * 
 * Based on a (admittedly bad) proxy at 
 *
 * http://godlytalias.blogspot.com/2013/02/simple-proxy-server-using-c.html
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

	struct hostent* host;
	int sockfd,newsockfd;
	   
	if(argc<4)
		error("./lproxy <port_no> <rabinserver_hostname> <rabinserver_port>");
	  
	   
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
	 
	accepting:
	 
	newsockfd=accept(sockfd,(struct sockaddr*)&cli_addr,&clilen);
	   
	if(newsockfd<0)
		error("Problem in accepting connection");
	

    /* Lets make this connection persistant.  */

    int rport = atoi(argv[3]);
    RabinClient rabin_c(argv[2], rport);
    rabin_c.connect_to_server();
    /*****************************************/
  
	pid=fork();
	/* This is ugly, it creates a new process for every connection + uses gotos to do this
	* kinda crappy programming, should iterate and use 'select' but whatever.*/
	if(pid==0)
	{

		struct sockaddr_in host_addr;
		int flag=0,newsockfd1,n,port=0,i,sockfd1;
		char buffer[510],t1[300],t2[300],t3[10];
		char* temp=NULL;
		bzero((char*)buffer,500);
		recv(newsockfd,buffer,500,0);
		   
		sscanf(buffer,"%s %s %s",t1,t2,t3);
		   
		/* Only deals with HTTP get, there are some further restrictions */
		if(((strncmp(t1,"GET",3)==0))&&((strncmp(t3,"HTTP/1.1",8)==0)||(strncmp(t3,"HTTP/1.0",8)==0))&&(strncmp(t2,"http://",7)==0))
		{
			
		
		strcpy(t1,t2);
		   
		flag=0;
		   
		for(i=7;(unsigned)i<strlen(t2);i++)
		{
		if(t2[i]==':')
		{
		flag=1;
		break;
		}
		}
	   
		temp=strtok(t2,"//");
		if(flag==0)
		{
		port=80;
		temp=strtok(NULL,"/");
		}
		else
		{
		temp=strtok(NULL,":");
		}
		   
		sprintf(t2,"%s",temp);
		printf("host = %s",t2);
		host=gethostbyname(t2);
		   
		if(flag==1)
		{
		temp=strtok(NULL,"/");
		port=atoi(temp);
		}
	   
		   
		strcat(t1,"^]");
		temp=strtok(t1,"//");
		temp=strtok(NULL,"/");
		if(temp!=NULL)
		temp=strtok(NULL,"^]");
		printf("\npath = %s\nPort = %d\n",temp,port);
		   
		   
		bzero((char*)&host_addr,sizeof(host_addr));
		host_addr.sin_port=htons(port);
		host_addr.sin_family=AF_INET;
		bcopy((char*)host->h_addr,(char*)&host_addr.sin_addr.s_addr,host->h_length);
		   
		sockfd1=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		newsockfd1=connect(sockfd1,(struct sockaddr*)&host_addr,sizeof(struct sockaddr));
		sprintf(buffer,"\nConnected to %s  IP - %s\n",t2,inet_ntoa(host_addr.sin_addr));
		if(newsockfd1<0)
		error("Error in connecting to remote server");
		   
		printf("\n%s\n",buffer);
		//send(newsockfd,buffer,strlen(buffer),0);
		bzero((char*)buffer,sizeof(buffer));
		if(temp!=NULL)
		sprintf(buffer,"GET /%s %s\r\nHost: %s\r\nConnection: close\r\n\r\n",temp,t3,t2);
		else
		sprintf(buffer,"GET / %s\r\nHost: %s\r\nConnection: close\r\n\r\n",t3,t2);
		 
		 
		n=send(sockfd1,buffer,strlen(buffer),0);
		printf("\n%s\n",buffer);
		if(n<0)
		    error("Error writing to socket");
		else{
			
            // In theory, this should work. This requires rabinserver
            // on the other side though along with a proxy that uses it
            string tmplte="XXXXXX";
            char *fname = NULL;
            /* figure out how to use mkstemp */
            //fname  = mktemp((char *)tmplte.c_str());
            if(fname == NULL) {
                fname = new char[20];
                strcpy(fname, "temporary.file");
            }
            FILE* tmp = fopen(fname, "w+");
            rabin_c.receive_file(tmp);
            
            fseek(tmp, 0L, SEEK_END);
            int n = ftell(tmp);
            fseek(tmp, 0L, SEEK_SET);
            fflush(tmp);
            char c[n];
            bzero((char *)c, n);
            fread(c, n, 1, tmp);
            
            send(newsockfd, c, n, 0);
            fclose(tmp);
            remove((const char *) fname);
            
        /*
			do {
				bzero((char*)buffer,500);
				n=recv(sockfd1,buffer,500,0);
				
				if(!(n<=0)) {
					send(newsockfd,buffer,n,0);
				}
			} while(n>0); */
		}
	}
	else
	{
	    send(newsockfd,"400 : BAD REQUEST\nONLY HTTP REQUESTS ALLOWED",18,0);
	}
	close(sockfd1);
	close(newsockfd);
	close(sockfd);
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
