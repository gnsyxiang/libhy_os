/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_thread.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    24/01 2024 14:49
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        24/01 2024      create the file
 * 
 *     last modified: 24/01 2024 14:49
 */
#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <string.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
#include <sys/prctl.h>

#include "hy_assert.h"
#include "hy_mem.h"

#include "hy_thread.h"

#include "config.h"

struct HyThread_s {
    HyThreadSaveConfig_s    save_c;
    hy_u32_t                is_exit;

    pthread_t               id;

    pthread_key_t           thread_key;
    hy_s32_t                is_init_key;
};

static void *_thread_cb(void *args)
{
    HyThread_s *handle = (HyThread_s *)args;
    HyThreadSaveConfig_s *save_c = &handle->save_c;
    hy_s32_t ret = 0;

    LOGI("<%s> thread loop start \n", save_c->name);

#ifdef HAVE_PTHREAD_SETNAME_NP
    pthread_setname_np(handle->id, save_c->name);
    // ret = prctl(PR_SET_NAME, name);
#endif

    while (0 == ret) {
        ret = save_c->thread_loop_cb(save_c->args);

        // pthread_testcancel();
    }

    handle->is_exit = 1;
    LOGI("%s thread loop stop \n", save_c->name);

    if (HY_THREAD_DETACH_MODE_YES == save_c->detach_mode) {
        HyThreadDestroy(&handle);
    }

    return NULL;
}

const char *HyThreadGetName(HyThread_s *handle)
{
    HY_ASSERT_RET_VAL(!handle, NULL);

    // int pthread_getname_np(pthread_t thread, char *name, size_t len);
    // ret = prctl(PR_GET_NAME, name);

    return handle->save_c.name;
}

pthread_t HyThreadGetId(HyThread_s *handle)
{
    HY_ASSERT_RET_VAL(!handle, -1);

    // pthread_self();

    return handle->id;
}

hy_s32_t HyThreadAttachCPU(hy_s32_t cpu_index)
{
    int cpu_num = sysconf(_SC_NPROCESSORS_CONF);
    if (cpu_index < 0 || cpu_index >= cpu_num) {
        LOGE("cpu index ERROR! \n");
        return -1;
    }

    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpu_index, &mask);

    if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) < 0) {
        LOGE("set affinity np ERROR! \n");
        return -1;
    }

    return 0;
}

hy_s32_t HyThreadKeySet(HyThread_s *handle, HyThreadKeyCreateCb_t key_create,
                        HyThreadKeyDestroyCb_t key_destroy)
{
    HY_ASSERT(handle);
    HY_ASSERT(key_create);
    HY_ASSERT(key_destroy);

    if (!handle->is_init_key) {
        if (0 != pthread_key_create(&handle->thread_key, key_destroy)) {
            LOGES("pthread_key_create failed \n");

            return -1;
        }
        handle->is_init_key = 1;
    }

    if (0 != pthread_setspecific(handle->thread_key, key_create())) {
        LOGES("pthread_setspecific failed \n");

        return -1;
    }

    return 0;
}

void *HyThreadKeyGet(HyThread_s *handle)
{
    HY_ASSERT(handle);

    return pthread_getspecific(handle->thread_key);
}

void HyThreadDestroy(HyThread_s **handle_pp)
{
    HyThread_s *handle = *handle_pp;
    hy_u32_t cnt = 0;

    HY_ASSERT_RET(!handle_pp || !*handle_pp);

    if (handle->save_c.destroy_mode == HY_THREAD_DESTROY_MODE_FORCE) {
        if (!handle->is_exit) {
            while (++cnt <= 9) {
                usleep(200 * 1000);
            }

            LOGW("force cancellation \n");
            pthread_cancel(handle->id);
        }
    }

    pthread_join(handle->id, NULL); // 触发线程资源的回收

    if (handle->is_init_key) {
        pthread_key_delete(handle->thread_key);
    }

    LOGI("%s thread destroy, handle: %p \n", handle->save_c.name, handle);
    HY_MEM_FREE_PP(handle_pp);
}

HyThread_s *HyThreadCreate(HyThreadConfig_s *thread_c)
{
    HyThread_s *handle = NULL;
    pthread_attr_t attr;
    struct sched_param param;

    HY_ASSERT_RET_VAL(!thread_c, NULL);

    do {
        handle = HY_MEM_MALLOC_BREAK(HyThread_s *, sizeof(*handle));
        HY_MEMCPY(&handle->save_c, &thread_c->save_c, sizeof(handle->save_c));

        if (0 != pthread_attr_init(&attr)) {
            LOGES("pthread_attr_init failed \n");
            break;
        }

        if (thread_c->policy != HY_THREAD_POLICY_SCHED_OTHER) {
            //设置继承的调度策略
            //必需设置为PTHREAD_EXPLICIT_SCHED，否则设置线程的优先级会被忽略
            if (0 != pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED)) {
                LOGES("pthread_attr_setinheritsched failed \n");
                break;
            }
            if (0 != pthread_attr_setschedpolicy(&attr, thread_c->policy)) {
                LOGES("pthread_attr_setschedpolicy failed \n");
                break;
            }

            // pthread_attr_getschedpolicy(&attr, &policy);
            // sched_get_priority_min(policy);

            param.sched_priority = thread_c->priority;
            if (0 != pthread_attr_setschedparam(&attr, &param)) {
                LOGES("pthread_attr_setschedparam failed \n");
                break;
            }
        }

        if (HY_THREAD_DETACH_MODE_YES == handle->save_c.detach_mode) {
            if (0 != pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)) {
                LOGES("set detach state failed \n");
                break;
            }
        }

        if (0 != pthread_create(&handle->id, &attr, _thread_cb, handle)) {
            LOGES("pthread create failed \n");
            break;
        }

        if (0 != pthread_attr_destroy(&attr)) {
            LOGES("destroy attr failed \n");
            break;
        }

        LOGI("%s thread create, handle: %p \n", handle->save_c.name, handle);
        return handle;
    } while (0);

    LOGE("%s thread create failed \n", thread_c->save_c.name);
    HyThreadDestroy(&handle);
    return NULL;
}

/*
 * pthread_t pthread_self(void)     <进程级别>是pthread 库给每个线程定义的进程内唯一标识，是 pthread 库维护的，是进程级而非系统级
 * syscall(SYS_gettid)              <系统级别>这个系统全局唯一的“ID”叫做线程PID（进程ID），或叫做TID（线程ID），也有叫做LWP（轻量级进程=线程）的。
 */

// 设置的名字可以在proc文件系统中查看: cat /proc/PID/task/tid/comm

// 如果当前线程没有被设定成DETACHED的话，
// 线程结束后，需要使用pthread_join来触发该一小段内存回收。
// pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

#if 0
    hal_hy_s32_t sched_priority[][2] = {
        {HAL_THREAD_PRIORITY_NORMAL,    50},
        {HAL_THREAD_PRIORITY_LOW,       30},
        {HAL_THREAD_PRIORITY_HIGH,      70},
        {HAL_THREAD_PRIORITY_REALTIME,  99},
        {HAL_THREAD_PRIORITY_IDLE,      10},
    };

    struct sched_param param;
    pthread_attr_setschedpolicy(&attr, SCHED_RR);
    param.sched_priority = sched_priority[config->priority][1];
    pthread_attr_setschedparam(&attr, &param);
#endif

#if 0
出现如下提示，表示线程资源没有释放，可能的原因如下: 

1, 创建的是非分离线程，线程结束后，需要使用pthread_join来触发该一小段内存回收。
2, 创建的是分离线程，但是主线程优先执行完退出程序，导致被创建的线程没有执行完，导致资源的泄露

==40360== HEAP SUMMARY:
==40360==     in use at exit: 272 bytes in 1 blocks
==40360==   total heap usage: 3 allocs, 2 frees, 1,344 bytes allocated
==40360==
==40360== 272 bytes in 1 blocks are possibly lost in loss record 1 of 1
==40360==    at 0x4C31B25: calloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==40360==    by 0x40134A6: allocate_dtv (dl-tls.c:286)
==40360==    by 0x40134A6: _dl_allocate_tls (dl-tls.c:530)
==40360==    by 0x4E44227: allocate_stack (allocatestack.c:627)
==40360==    by 0x4E44227: pthread_create@@GLIBC_2.2.5 (pthread_create.c:644)
==40360==    by 0x108F1C: HalLinuxThreadInit (hal_linux_thread.c:111)
==40360==    by 0x108CC1: HalThreadInit (hal_thread.c:85)
==40360==    by 0x108AD4: main (main.c:50)
==40360==
==40360== LEAK SUMMARY:
==40360==    definitely lost: 0 bytes in 0 blocks
==40360==    indirectly lost: 0 bytes in 0 blocks
==40360==      possibly lost: 272 bytes in 1 blocks
==40360==    still reachable: 0 bytes in 0 blocks
==40360==         suppressed: 0 bytes in 0 blocks
#endif
