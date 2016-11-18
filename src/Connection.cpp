#include "../Connection.h"

Connection::Connection(){}

Connection::Connection(const unsigned int port, const std::string & ip_address){
    
    memset((char *)&m_myaddr, 0, sizeof(m_myaddr)); 
    memset((char *)&m_remaddr, 0, sizeof(m_remaddr));
    
    m_myaddr.sin_family = AF_INET; 
    if(inet_pton(AF_INET, ip_address.c_str(), &m_myaddr.sin_addr.s_addr) != 1){
        throw ConnectionException("Connection: IP address is inavlid"); 
    }
    m_myaddr.sin_port = htons(port); 
    
    m_timeout.tv_sec = 1;
    m_timeout.tv_usec = 0;
    
}

void Connection::initializeConnection(){
    
    if ((m_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { 
        throw ConnectionException("Connection: Creating socket failed"); 
    } 
    if (bind(m_fd, (struct sockaddr *)&m_myaddr, sizeof(m_myaddr)) < 0) { 
        throw ConnectionException("Connection: Binding of socket failed");
    }
}

void Connection::closeConnection(){
    
    if( close(m_fd) < 0) {
        throw ConnectionException("Connection: Closing socket failed");
    }
}

int Connection::read(void * buffer, size_t bufferlength){
    
    int reads;
    socklen_t addrlen = sizeof(m_remaddr);
    fd_set socks;
    
    FD_ZERO(&socks);
    FD_SET(m_fd, &socks);
    
    reads = select(m_fd+1, &socks, (fd_set *) 0,  (fd_set *) 0, &m_timeout);
    
    if (reads < 0) {
        throw ConnectionException("Connection: Select failed");
    } else if (reads == 0) {
        return 0;
    } else {
        if (FD_ISSET(m_fd, &socks)){
            return recvfrom(m_fd, buffer, bufferlength, 0, (struct sockaddr *)&m_remaddr, &addrlen); 
        }
    } 
    
}




