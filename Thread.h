#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

namespace mThread {
    
    class Thread {
        
    public:
        Thread();
        virtual ~Thread();
        bool start();
        bool join();
        bool cancel();
        
    protected:
        virtual void run();
        
    private:
        static void * callRun(void * This);
        pthread_t m_thread;
    };
}

#endif
