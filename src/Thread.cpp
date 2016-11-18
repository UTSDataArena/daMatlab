#include "../Thread.h"

mThread::Thread::Thread(){};

mThread::Thread::~Thread(){};

bool mThread::Thread::start() {
    return (pthread_create(&m_thread, NULL, callRun, this) == 0);
}

bool mThread::Thread::join() {
    return (pthread_join(m_thread, NULL) == 0);
}

bool mThread::Thread::cancel(){
    return (pthread_cancel(m_thread) == 0);
}

void mThread::Thread::run(){};

void * mThread::Thread::callRun(void * this_ptr) {
    static_cast<Thread*>(this_ptr)->run();    
    return NULL;
}
