# ===============================================================
# 
# Release under GPLv-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    14/12 2023 16:45
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        14/12 2023      create the file
# 
#     last modified: 14/12 2023 16:45
# ===============================================================

# ------------------
# defining variables
# ------------------
_no_install_headers_com =
_install_headers_com    =

install_includedir      = $(includedir)/@project_name@

# ---------------------
# add necessary content
# ---------------------
_no_install_headers_com +=

_install_headers_com    += \
                           hy_barrier.h \
                           hy_can.h \
                           hy_file.h \
                           hy_gpio.h \
                           hy_pipe.h \
                           hy_signal.h \
                           hy_thread.h \
                           hy_thread_cond.h \
                           hy_thread_mutex.h \
                           hy_thread_sem.h \
                           hy_time.h \
                           hy_utils_linux.h \
                           hy_zone.h

# ------------------
# make install files
# ------------------
noinst_HEADERS          = $(_no_install_headers_com)
install_include_HEADERS = $(_install_headers_com)
