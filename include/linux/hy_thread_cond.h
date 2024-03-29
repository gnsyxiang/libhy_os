/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_thread_cond.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    24/01 2024 15:03
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        24/01 2024      create the file
 * 
 *     last modified: 24/01 2024 15:03
 */
#ifndef __LIBHY_OS_INCLUDE_HY_THREAD_COND_H_
#define __LIBHY_OS_INCLUDE_HY_THREAD_COND_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>
#include <pthread.h>

#include <hy_os_type/hy_type.h>

/**
 * @brief 配置参数
 */
typedef struct {
    hy_s32_t                    reserved;           ///< 预留
} HyThreadSaveCondConfig_s;

/**
 * @brief 配置参数
 */
typedef struct {
    HyThreadSaveCondConfig_s    save_c;             ///< 配置参数
} HyThreadCondConfig_s;

typedef struct HyThreadCond_s {
    pthread_cond_t              cond;
} HyThreadCond_s;

/**
 * @brief 创建条件变量模块
 *
 * @param cond_c 配置参数
 *
 * @return 成功返回句柄，失败返回NULL
 */
HyThreadCond_s *HyThreadCondCreate(HyThreadCondConfig_s *cond_c);

/**
 * @brief 创建条件变量模块宏
 *
 * @return 成功返回句柄，失败返回NULL
 */
#define HyThreadCondCreate_m()                      \
({                                                  \
    HyThreadCondConfig_s cond_c;                    \
    HY_MEMSET(&cond_c, sizeof(cond_c));             \
    HyThreadCondCreate(&cond_c);                    \
 })

/**
 * @brief 销毁条件变量模块
 *
 * @param handle 句柄的地址（二级指针）
 */
void HyThreadCondDestroy(HyThreadCond_s **handle_pp);

/**
 * @brief 发送信号
 *
 * @param handle 句柄
 *
 * @return 成功返回0，失败返回-1
 */
static inline hy_s32_t HyThreadCondSignal(HyThreadCond_s *handle)
{
    assert(handle);

    return pthread_cond_signal(&handle->cond) == 0 ? 0 : -1;
}

#define HyThreadCondSignal_m(_handle)               \
do {                                                \
    if (0 != HyThreadCondSignal(_handle)) {         \
        LOGES("HyThreadCondSignal failed \n");      \
    }                                               \
} while (0)

/**
 * @brief 广播信号
 *
 * @param handle 句柄
 *
 * @return 成功返回0，失败返回-1
 */
static inline hy_s32_t HyThreadCondBroadcast(HyThreadCond_s *handle)
{
    assert(handle);

    return pthread_cond_broadcast(&handle->cond) == 0 ? 0 : -1;
}

#define HyThreadCondBroadcast_m(_handle)            \
do {                                                \
    if (0 != HyThreadCondBroadcast(_handle)) {      \
        LOGES("HyThreadCondBroadcast failed \n");   \
    }                                               \
} while (0)

struct HyThreadMutex_s;
/**
 * @brief 等待信号
 *
 * @param handle 句柄
 * @param mutex_h 锁句柄
 * @param timeout_ms 超时时间，0表示阻塞等待
 *
 * @return 成功返回0，失败返回-1
 */
hy_s32_t HyThreadCondWait(HyThreadCond_s *handle, struct HyThreadMutex_s *mutex_h, hy_u32_t timeout_ms);

#define HyThreadCondWait_m(_handle, _mutex_h, _timeout_ms)          \
do {                                                                \
    if (0 != HyThreadCondWait(_handle, _mutex_h, _timeout_ms)) {    \
        LOGES("HyThreadCondWait failed \n");                        \
    }                                                               \
} while (0)

#ifdef __cplusplus
}
#endif

#endif

