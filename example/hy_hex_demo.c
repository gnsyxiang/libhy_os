/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_hex_demo.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/01 2024 09:28
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/01 2024      create the file
 * 
 *     last modified: 25/01 2024 09:28
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hy_hex.h"

#include "hy_assert.h"
#include "hy_mem.h"
#include "hy_string.h"

int main(int argc, char *argv[])
{
    HyLogConfig_s log_c;
    HY_MEMSET(&log_c, sizeof(HyLogConfig_s));
    log_c.port                      = 56789;
    log_c.fifo_len                  = 10 * 1024;
    log_c.config_file               = "../res/hy_log/zlog.conf";
    log_c.save_c.level              = HY_LOG_LEVEL_INFO;
    log_c.save_c.output_format      = HY_LOG_OUTFORMAT_ALL_NO_PID_ID;

    char *buf = "1234567890abcdefghi";
    HY_HEX_ASCII(buf, HY_STRLEN(buf));
    HY_HEX(buf, HY_STRLEN(buf));

    hy_u32_t a = 0x5ffabf;

    HY_HEX_BIT(&a, sizeof(a));

    sleep(3);

    HyLogDeInit();

    return 0;
}

