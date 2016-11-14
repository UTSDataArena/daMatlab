#include "../GeometryQueue.h"

const unsigned int GeometryQueue::SIZE = 128;

GeometryQueue::GeometryQueue() {
    pthread_mutex_init(&m_mutex, NULL);
}


GeometryQueue::~GeometryQueue() {
    pthread_mutex_destroy(&m_mutex);
}


bool GeometryQueue::push(MatlabGeometry * geometry){
    pthread_mutex_lock(&m_mutex);
    bool isFull = (m_queue.size() == GeometryQueue::SIZE);
    if(!isFull)
        m_queue.push_back(geometry);     
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
    m_queue.pop_front();
    pthread_mutex_unlock(&m_mutex);
    return geometry;
}

bool GeometryQueue::isEmpty(){
    pthread_mutex_lock(&m_mutex);
    bool empty = m_queue.empty();
    pthread_mutex_unlock(&m_mutex);
    return empty;
}

void GeometryQueue::printt(){
    pthread_mutex_lock(&m_mutex);
    std::list<MatlabGeometry *>::const_iterator iterator;
    for (iterator = m_queue.begin(); iterator != m_queue.end(); ++iterator) {
        std::cout << (*iterator)->getName() << endl;;
    }
    pthread_mutex_unlock(&m_mutex);
}
