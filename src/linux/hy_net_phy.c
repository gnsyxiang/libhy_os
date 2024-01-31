/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_net_phy.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    30/01 2024 19:13
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        30/01 2024      create the file
 * 
 *     last modified: 30/01 2024 19:13
 */
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/sockios.h>
#include <linux/ethtool.h>
#include <linux/if.h>

#include  <hy_log/hy_log.h>

#include "hy_net_phy.h"

HyNetPhyNetlinkStatus_e HyNetPhyNetlinkStatusGet(const char *if_name)
{
    hy_s32_t skfd;
    struct ifreq ifr;
    struct ethtool_value edata;

    edata.cmd = ETHTOOL_GLINK;
    edata.data = 0;

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_data = (char *) &edata;
    strncpy(ifr.ifr_name, if_name, sizeof(ifr.ifr_name) - 1);

    do {
        skfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (-1 == skfd) {
            LOGE("socket failed \n");
            break;
        }

        if (-1 == ioctl(skfd, SIOCETHTOOL, &ifr)) {
            LOGE("ioctl SIOCETHTOOL failed \n");
            break;
        }

        close(skfd);
        return (edata.data == 1) ? HY_NET_PHY_NETLINK_STATUS_UP : HY_NET_PHY_NETLINK_STATUS_DOWN;
        return edata.data;
    } while (0);

    return HY_NET_PHY_NETLINK_STATUS_ERROR;
}
