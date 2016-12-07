/******************************************************************************
 * Based on http://stackoverflow.com/a/1151615 (Jeremy Friesner)
 *****************************************************************************/

#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

/*
 * We are using pthread since C++11 is not available
 */
namespace mThread {
    
    class Thread {
        
    public:
        Thread();
        virtual ~Thread();
        bool start();
        bool join();
        bool cancel();
        
    protected:
        virtual void run() = 0;
        
    private:
        static void * helper(void * this_ptr);
        pthread_t m_thread;
    };
}

#endif
