/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_utils_linux.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/01 2024 17:20
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/01 2024      create the file
 * 
 *     last modified: 25/01 2024 17:20
 */
#ifndef __LIBHY_OS_INCLUDE_HY_UTILS_LINUX_H_
#define __LIBHY_OS_INCLUDE_HY_UTILS_LINUX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <hy_os_type/hy_type.h>

/**
 * @brief 执行命令
 *
 * @param _cmd 命令
 *
 * @return 无
 */
#define HY_UTILS_SYSTEM_m(_cmd, _ret)                       \
    ({                                                      \
        hy_s32_t ret = 0xffffffff;                          \
        do {                                                \
            if (HY_STRLEN(_cmd) <= 0) {                     \
                break;                                      \
            }                                               \
            ret = system(_cmd);                             \
            if (_ret == ret) {                              \
                LOGI("system cmd: %s \n", _cmd);            \
                break;                                      \
            }                                               \
            LOGE("system cmd failed, cmd: %s \n", _cmd);    \
        } while (0);                                        \
        (ret == _ret) ? 0 : -1;                             \
     })

/**
 * @brief 生成随机数
 *
 * @param range 限定随机数的范围
 *
 * @return 生成1到range之间的任意一个数
 */
hy_u32_t HyUtilsRandomNum(hy_u32_t range);

#ifdef __cplusplus
}
#endif

#endif

