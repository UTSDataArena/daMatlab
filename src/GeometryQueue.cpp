#include "../GeometryQueue.h"

const unsigned int GeometryQueue::MAX_SIZE = 128;

GeometryQueue::GeometryQueue() {
    pthread_mutex_init(&m_mutex, NULL);
}

GeometryQueue::~GeometryQueue() {
    pthread_mutex_destroy(&m_mutex);
}

bool GeometryQueue::push(MatlabGeometry * geometry){    
    pthread_mutex_lock(&m_mutex);
    bool isFull = (m_queue.size() == GeometryQueue::MAX_SIZE);
    if(!isFull){
        m_queue.push(geometry);
    }
    pthread_mutex_unlock(&m_mutex);
    return !isFull;
}

MatlabGeometry * GeometryQueue::back(){
    pthread_mutex_lock(&m_mutex);
    MatlabGeometry * geometry = m_queue.back();
    pthread_mutex_unlock(&m_mutex);
    return geometry;
}

MatlabGeometry * GeometryQueue::pop(){
    pthread_mutex_lock(&m_mutex);
    MatlabGeometry * geometry = m_queue.front();
    m_queue.pop();
    pthread_mutex_unlock(&m_mutex);
    return geometry;
}

bool GeometryQueue::isEmpty(){
    pthread_mutex_lock(&m_mutex);
    bool empty = m_queue.empty();
    pthread_mutex_unlock(&m_mutex);
    return empty;
}

