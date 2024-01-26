/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_thread_mutex.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    24/01 2024 15:07
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        24/01 2024      create the file
 * 
 *     last modified: 24/01 2024 15:07
 */
#include <stdio.h>

#include <hy_log/hy_log.h>

#include "hy_thread_mutex.h"

#include "hy_assert.h"
#include "hy_mem.h"

void HyThreadMutexDestroy(HyThreadMutex_s **handle_pp)
{
    HY_ASSERT_RET(!handle_pp || !*handle_pp);

    HyThreadMutex_s *handle = *handle_pp;

    if (0 != pthread_mutex_destroy(&handle->mutex)) {
        LOGES("pthread_mutex_destroy failed, handle: %p \n", handle);
    }

    LOGI("thread mutex destroy, handle: %p \n", handle);
    HY_MEM_FREE_PP(handle_pp);
}

HyThreadMutex_s *HyThreadMutexCreate(HyThreadMutexConfig_s *mutex_c)
{
    HY_ASSERT_RET_VAL(!mutex_c, NULL);

    HyThreadMutex_s *handle = NULL;

    do {
        handle = HY_MEM_MALLOC_BREAK(HyThreadMutex_s *, sizeof(*handle));

        if (0 != pthread_mutex_init(&handle->mutex, NULL)) {
            LOGES("pthread_mutex_init failed \n");
            break;
        }

        LOGI("thread mutex create, handle: %p \n", handle);
        return handle;
    } while (0);

    LOGE("thread mutex create failed \n");
    HyThreadMutexDestroy((HyThreadMutex_s **)&handle);
    return NULL;
}
