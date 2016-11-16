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
    
    DataReader();
    DataReader(GeometryQueue * queue, pthread_mutex_t * mutex);
    ~DataReader();
    void run();

private:
    
    enum STATE {HEADER, DATA};
    enum TYPE {VERTEX, FACE, COLOR, VERTEX_NORMAL, FACE_NORMAL};
    
    static const string H_MODEL_NAME;
    static const string H_CAMPOS;
    static const string H_CAMUP; 
    static const string H_TYPE;
    
    static const string D_VERTEX;
    static const string D_VERTEX_NORMAL;
    static const string D_FACE_NORMAL;
    static const string D_FACE;   
    static const string D_COLOR;
    
    static const string H_NEXT; 
    static const string H_CLEAR; 
    static const string H_ADD; 
    
    void readData(const string & data, const unsigned int & count);
    void readHeader(const string & data, unsigned int & count);    
    void parseData(const string & data, float vector[]);
    void receiveData();
    
    pthread_mutex_t * m_mutex;
    
    Connection m_connection;
    GeometryQueue * m_queue;
    MatlabGeometry * m_matlabGeometry;
    
    bool m_create;
    STATE m_state;
    TYPE m_type;
    
};

#endif
