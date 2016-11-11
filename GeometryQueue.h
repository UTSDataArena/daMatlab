#ifndef GEOMETRY_QUEUE_H
#define GEOMETRY_QUEUE_H

#include <pthread.h>
#include <list>
#include <iostream>
#include "MatlabGeometry.h"


/*Queue to transfer MatlabGeometry objects between DataReader thread and main.
 *GeometryQueue only handles pointers to the MatlabGeometry objects.
 */

class GeometryQueue
{ 
    
    
public:
    
    GeometryQueue();
    ~GeometryQueue();
    
    bool push(MatlabGeometry * geometry);
    MatlabGeometry * back();
    MatlabGeometry * pop();
    bool isEmpty();
    void printt();
   
private:
    static const unsigned int SIZE;
    std::list< MatlabGeometry * > m_queue;
    pthread_mutex_t m_mutex;
};

#endif
