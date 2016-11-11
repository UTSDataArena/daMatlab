#ifndef CONNECTION_H
#define CONNECTION_H


#include <exception>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>



class Connection {
    
public:
    
    Connection();
    void initializeConnection();
    void closeConnection();
    int read(void * buffer, size_t bufferlength);    
    
private:
    
    
    class ConnectionException : public std::exception {
    
    public:

        ConnectionException(const char * message) { m_message = message; }
        virtual const char * what() const throw() { return m_message; }
        
    private:
        const char * m_message;
    };
    
    struct sockaddr_in m_myaddr;
    struct sockaddr_in m_remaddr;
    struct timeval m_timeout;
    int m_fd;
    unsigned int m_port;
    unsigned int m_address;
};


#endif
