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
#include "../Server/rabinserver.h"

/*
 * Dinky ugly server-side testing integration proxy that uses rabinserver.
 * Based on a (admittedly bad) proxy at 
    http://godlytalias.blogspot.com/2013/02/simple-proxy-server-using-c.html
*/


void error(string msg)
{
	perror(msg.c_str());
	exit(0);
}
  
int main(int argc,char* argv[])
{
	struct sockaddr_in addr_in,cli_addr,serv_addr;

	struct hostent* host;
	int sockfd,newsockfd;
	   
	if(argc<3)
		error("./lproxy <port_no> <rabin port_no>");
	   
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
	 

    cerr << "Trying to connect" <<endl;	  
    RabinServer rabin_s(atoi(argv[2]));
	/* TODO: I guess THIS is blocking. This may be a POSSIBLE BUG. */
    rabin_s.connect_to_client();
    /*****************************************/
    cerr << "Connection to intproxy established" <<endl; 
    
  while(1) {
    cerr<<"Ready..."<<endl;
	newsockfd=accept(sockfd,(struct sockaddr*)&cli_addr,&clilen);
	cerr << "Accepting connection" <<endl; 

	if(newsockfd<0)
		error("Problem in accepting connection");

    alive:
        fflush(stdout);	
		struct sockaddr_in host_addr;
		int flag=0,newsockfd1,n,port=0,i,sockfd1;
		char buffer[1000],t1[500],t2[500],t3[30];
		char* temp=NULL;
		bzero((char*)buffer,1000);
		
        if(recv(newsockfd,buffer,1000,0) < 0) {
            cerr <<"Connection lost. ERRNO "<<errno<<endl;
            continue;
        }
		   
		sscanf(buffer,"%s %s %s",t1,t2,t3); 
		bool c1 = ((strncmp(t1,"GET",3)==0)); 
		bool c2 = (strncmp(t3,"HTTP/1.1",8)==0); 
		bool c3 = (strncmp(t3,"HTTP/1.0",8)==0); 
		bool c4 = (strncmp(t2,"http://",7)==0); 

		/* Only deals with HTTP get, there are some further restrictions */
		if(c1 && (c2 || c3) && c4)
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
			
            string tmplte="XXXXXX";
            char *fname = NULL;
            /* figure out how to use mkstemp */
            //fname  = mktemp((char *)tmplte.c_str());
            if(fname == NULL) {
                fname = new char[20];
                strcpy(fname, "temporary2.file");
            }
            FILE* tmp = fopen(fname, "w+");
            
            int s = 0; /* Holds total size of the file*/
			
            do {
				bzero((char*)buffer,500);
				n=recv(sockfd1,buffer,500,0);
				
				if(!(n<=0)) {
					fwrite(buffer, n, 1, tmp);
                    s = s + n;
				}

			} while(n>0);

            fflush(tmp);
            fseek(tmp, 0L, SEEK_SET);
            char c[s];

            fread(c, s, 1, tmp);
            rabin_s.send_file((char *) c, s);
            fclose(tmp);
            remove((const char *) fname);
		}
        goto alive; // TODO: This is dumb
	}
	else
	{
        cerr <<"BAD FILE: Terminating connection"<<endl;
	    cerr <<c1<<" " <<c2<<" "<< c3 << " "<< c4<<endl;
        cerr << buffer<<endl;
        string s = "400 : BAD REQUEST\nONLY HTTP REQUESTS ALLOWED";
	    rabin_s.send_file((char *)s.c_str(),s.length());
        
        //close(newsockfd);
	}

 }
	close(newsockfd);
    rabin_s.~RabinServer();
    (void) addr_in;
    goto alive;
	return 0;
}