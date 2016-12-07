#ifndef DATAREADER_H
#define DATAREADER_H

#include <omega.h>
#include <pthread.h>
#include <iostream>
#include "Thread.h"
#include "Connection.h"
#include "GeometryQueue.h"
#include "MatlabGeometry.h"


class DataReader : public mThread::Thread {
    
public:
    
    DataReader(); //don't use this
    DataReader(const unsigned int port, const std::string & ip_address, GeometryQueue * queue, pthread_mutex_t * mutex);
    ~DataReader();
    void run();
    
private:
    
    enum STATE {HEADER, DATA} m_state;
    enum TYPE {VERTEX, FACE, COLOR, VERTEX_NORMAL, FACE_NORMAL} m_type;
    
    pthread_mutex_t * m_mutex;
    
    Connection m_connection;
    GeometryQueue * m_queue;
    MatlabGeometry * m_matlabGeometry;
    
    bool m_create;
    
    void readData(const std::string & data, const unsigned int & count);
    void readHeader(const std::string & data, unsigned int & count);    
    void parseData(const std::string & data, float vector[]);
    void receiveData();
    
    struct Flags {
        
        static const std::string H_MODEL_NAME;
        static const std::string H_CAM_POS;
        static const std::string H_CAM_UP; 
        static const std::string H_TYPE;
        
        static const std::string D_VERTEX;
        static const std::string D_VERTEX_NORMAL;
        static const std::string D_FACE_NORMAL;
        static const std::string D_FACE;   
        static const std::string D_COLOR;
        
        static const std::string H_NEXT; 
        static const std::string H_CLEAR; 
        static const std::string H_ADD;    
        
    };
    
};

#endif
