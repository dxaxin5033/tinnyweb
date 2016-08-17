all:server 
server:server.c accept_request.c
    gcc -lpthread server.c accept_request.c -o $@                                        



.PHONY:clean

clean:
    rm -f server
    
