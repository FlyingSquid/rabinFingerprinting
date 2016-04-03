all: server client integration

server: 
		$(MAKE) -C Server all

client: 
		$(MAKE) -C Client all

integration: 
		$(MAKE) -C Integration all

