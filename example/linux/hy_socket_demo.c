/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_socket_demo.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    20/02 2024 10:41
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        20/02 2024      create the file
 * 
 *     last modified: 20/02 2024 10:41
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <hy_log/hy_log.h>

#include "hy_mem.h"
#include "hy_string.h"
#include "hy_module.h"
#include "hy_utils.h"
#include "hy_signal.h"

#include "config.h"

#include "hy_socket.h"

#define _APP_NAME "hy_socket_client_demo"

typedef struct {
    hy_s32_t    is_exit;
    hy_s32_t    socket_fd;
} _main_context_s;

static void _signal_error_cb(void *args)
{
    _main_context_s *context = args;
    context->is_exit = 1;

    LOGE("------error cb\n");
}

static void _signal_user_cb(void *args)
{
    _main_context_s *context = args;
    context->is_exit = 1;

    LOGW("------user cb\n");
}

static void _bool_module_destroy(_main_context_s **context_pp)
{
    HyModuleDestroyBool_s bool_module[] = {
        {"signal",          HySignalDestroy },
        {"log",             HyLogDeInit     },
    };

    HY_MODULE_RUN_DESTROY_BOOL(bool_module);
}

static hy_s32_t _bool_module_create(_main_context_s *context)
{
    HyLogConfig_s log_c;
    HY_MEMSET(&log_c, sizeof(HyLogConfig_s));
    log_c.port                      = 56789;
    log_c.fifo_len                  = 10 * 1024;
    log_c.config_file               = "../res/hy_log/zlog.conf";
    log_c.save_c.level              = HY_LOG_LEVEL_INFO;
    log_c.save_c.output_format      = HY_LOG_OUTFORMAT_ALL_NO_PID_ID;

    hy_s8_t signal_error_num[HY_SIGNAL_NUM_MAX_32] = {
        SIGQUIT, SIGILL, SIGTRAP, SIGABRT, SIGFPE,
        SIGSEGV, SIGBUS, SIGSYS, SIGXCPU, SIGXFSZ,
    };

    hy_s8_t signal_user_num[HY_SIGNAL_NUM_MAX_32] = {
        SIGINT, SIGTERM, SIGUSR1, SIGUSR2,
    };

    HySignalConfig_t signal_c;
    HY_MEMSET(&signal_c, sizeof(signal_c));
    HY_MEMCPY(signal_c.error_num, signal_error_num, sizeof(signal_error_num));
    HY_MEMCPY(signal_c.user_num, signal_user_num, sizeof(signal_user_num));
    signal_c.save_c.app_name        = _APP_NAME;
    signal_c.save_c.coredump_path   = "./";
    signal_c.save_c.error_cb        = _signal_error_cb;
    signal_c.save_c.user_cb         = _signal_user_cb;
    signal_c.save_c.args            = context;

    HyModuleCreateBool_s bool_module[] = {
        {"log",         &log_c,         (HyModuleCreateBoolCb_t)HyLogInit,          HyLogDeInit},
        {"signal",      &signal_c,      (HyModuleCreateBoolCb_t)HySignalCreate,     HySignalDestroy},
    };

    HY_MODULE_RUN_CREATE_BOOL(bool_module);
}

static void _handle_module_destroy(_main_context_s **context_pp)
{
    // note: 增加或删除要同步到HyModuleCreateHandle_s中
    HyModuleDestroyHandle_s module[] = {
        {NULL, NULL, NULL},
    };

    HY_MODULE_RUN_DESTROY_HANDLE(module);
}

static hy_s32_t _handle_module_create(_main_context_s *context)
{
    // note: 增加或删除要同步到HyModuleDestroyHandle_s中
    HyModuleCreateHandle_s module[] = {
        {NULL, NULL, NULL, NULL, NULL},
    };

    HY_MODULE_RUN_CREATE_HANDLE(module);
}

static void _tcp_server_test(_main_context_s *context)
{
    do {
        context->socket_fd = HySocketCreate(HY_SOCKET_DOMAIN_TCP);
        if (-1 == context->socket_fd) {
            LOGE("HySocketCreate failed \n");
            break;
        }

        if (-1 == HySocketListen(context->socket_fd, NULL, 8899)) {
            LOGE("HySocketConnect failed \n");
            break;
        }

        struct sockaddr_in client_addr;
        hy_s32_t new_fd;
        char buf[16];
        while (!context->is_exit) {
            new_fd = HySocketAccept(context->socket_fd, &client_addr);
            if (new_fd) {
                HY_MEMSET(buf, sizeof(buf));

                read(new_fd, buf, sizeof(buf));
                LOGI("buf: %s \n", buf);

                HySocketDestroy(&new_fd);
            }
        }

        HySocketDestroy(&context->socket_fd);

        return;
    } while(0);

    if (context->socket_fd) {
        HySocketDestroy(&context->socket_fd);
    }
}

static void _tcp_client_test(_main_context_s *context)
{
    do {
        context->socket_fd = HySocketCreate(HY_SOCKET_DOMAIN_TCP);
        if (-1 == context->socket_fd) {
            LOGE("HySocketCreate failed \n");
            break;
        }

        if (-1 == HySocketConnect(context->socket_fd, "192.168.0.15", 56789)) {
            LOGE("HySocketConnect failed \n");
            break;
        }

        const char *buf = "hello world";
        write(context->socket_fd, buf, strlen(buf));

        HySocketDestroy(&context->socket_fd);

        return;
    } while(0);

    if (context->socket_fd) {
        HySocketDestroy(&context->socket_fd);
    }
}

int main(int argc, char *argv[])
{
    _main_context_s *context = NULL;
    do {
        context = HY_MEM_MALLOC_BREAK(_main_context_s *, sizeof(*context));

        struct {
            const char *name;
            hy_s32_t (*create)(_main_context_s *context);
        } create_arr[] = {
            {"_bool_module_create",     _bool_module_create},
            {"_handle_module_create",   _handle_module_create},
        };
        for (hy_u32_t i = 0; i < HY_UTILS_ARRAY_CNT(create_arr); i++) {
            if (create_arr[i].create) {
                if (0 != create_arr[i].create(context)) {
                    LOGE("%s failed \n", create_arr[i].name);
                }
            }
        }

        LOGE("version: %s, data: %s, time: %s \n", VERSION, __DATE__, __TIME__);

        // _tcp_client_test(context);
        // _tcp_server_test(context);

        while (!context->is_exit) {
            HySocketClientTCPWriteOnceTimeout("192.168.0.15", 56789, 500,
                                              "hello world", HY_STRLEN("hello world"));
            sleep(1);
        }
    } while (0);

    void (*destroy_arr[])(_main_context_s **context_pp) = {
        _handle_module_destroy,
        _bool_module_destroy
    };
    for (hy_u32_t i = 0; i < HY_UTILS_ARRAY_CNT(destroy_arr); i++) {
        if (destroy_arr[i]) {
            destroy_arr[i](&context);
        }
    }
    HY_MEM_FREE_PP(&context);

    return 0;
}

