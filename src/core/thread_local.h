/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set ts=8 sts=4 et sw=4 tw=80: */
// Workaround for older compilers not having thread_local storage.
//
// Based on <http://stackoverflow.com/q/16147621> by
// <http://stackoverflow.com/users/1412605/pontomedon>.
//
// Licensed under CC-BY-SA <https://creativecommons.org/licenses/by-sa/3.0/>.

#ifndef XMSG_INTERNAL_THREAD_LOCAL_H
#define XMSG_INTERNAL_THREAD_LOCAL_H

#include <utility>

#include <pthread.h>
#include <stdio.h>

namespace xmsg {
namespace detail {

template <class T>
class ThreadLocal
{
private:
    static pthread_key_t key;
    static pthread_once_t once_control;

    static void make_key()
    {
        (void) pthread_key_create(&ThreadLocal::key, ThreadLocal::destructor);
    }

    static void destructor(void* obj)
    {
        delete ((T*) obj);
    }

public:
    template <typename... Args>
    static T* getThreadInstance(Args&&... args)
    {
        pthread_once(&once_control, ThreadLocal::make_key);
        T* instance = (T*) pthread_getspecific(key);
        if (!instance) {
            try {
                instance = new T(std::forward<Args>(args)...);
                pthread_setspecific(key, instance);
            } catch (const char* ex) {
                printf("Exception during thread local singleton init: %s\n", ex);
            }
        }
        return instance;
    }
};

template <class T>
pthread_key_t ThreadLocal<T>::key;

template <class T>
pthread_once_t ThreadLocal<T>::once_control = PTHREAD_ONCE_INIT;

} // end namespace detail
} // end namespace xmsg

#endif // XMSG_INTERNAL_THREAD_LOCAL_H
