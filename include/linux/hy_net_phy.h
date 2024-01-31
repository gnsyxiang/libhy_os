/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_net_phy.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    30/01 2024 19:10
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        30/01 2024      create the file
 * 
 *     last modified: 30/01 2024 19:10
 */
#ifndef __LIBHY_OS_INCLUDE_HY_NET_PHY_H_
#define __LIBHY_OS_INCLUDE_HY_NET_PHY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <hy_os_type/hy_type.h>

typedef enum {
    HY_NET_PHY_NETLINK_STATUS_ERROR = -1,
    HY_NET_PHY_NETLINK_STATUS_DOWN,
    HY_NET_PHY_NETLINK_STATUS_UP,
} HyNetPhyNetlinkStatus_e;

HyNetPhyNetlinkStatus_e HyNetPhyNetlinkStatusGet(const char *if_name);

#ifdef __cplusplus
}
#endif

#endif

