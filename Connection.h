#ifndef CONNECTION_H
#define CONNECTION_H

#include <exception>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <cstring>

class Connection {
    
public:
    
    Connection(); //don't use this
    Connection(const unsigned int port, const std::string & ip_address);
    void initializeConnection();
    void closeConnection();
    int read(void * buffer, size_t bufferlength);    
    
private:

    struct sockaddr_in m_myaddr;
    struct sockaddr_in m_remaddr;
    struct timeval m_timeout;
    int m_fd; 
    
    class ConnectionException : public std::exception {
        
    public:
        
        ConnectionException(const char * message) { m_message = message; }
        virtual const char * what() const throw() { return m_message; }
        
    private:
        const char * m_message;
    };
};


#endif
