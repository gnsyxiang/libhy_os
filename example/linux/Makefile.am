# ===============================================================
# 
# Release under GPLv-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    14/12 2023 16:47
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        14/12 2023      create the file
# 
#     last modified: 14/12 2023 16:47
# ===============================================================

# ------------------
# defining variables
# ------------------
_no_error_com   =
_define_com     =
_include_com    = $(include_com)
_sources_com    =
_cppflags_com   =
_cflags_com     =
_cxxflags_com   =
_ldflags_com    =
_ldadd_com      =
bin_PROGRAMS    =

# ---------------------
# add necessary content
# ---------------------
_no_error_com   +=

_define_com     +=

_include_com    += \
                   -I$(top_srcdir)/include/$(run_os) \
                   -I$(top_srcdir)/include

_sources_com    +=

_cppflags_com   += \
                   $(_no_error_com) \
                   $(_include_com) \
                   $(_define_com)

_cflags_com     +=

_cxxflags_com   +=

_ldflags_com    +=

_ldadd_com      += \
                   $(top_builddir)/src/lib@project_name@.la \
                   $(ldadd_com)

if COMPILE_LIBHY_LOG
_ldadd_com      += $(HY_LOG_LIBS)
endif

bin_PROGRAMS                    += hy_can_demo
hy_can_demo_SOURCES             = $(_sources_com) hy_can_demo.c
hy_can_demo_CPPFLAGS            = $(_cppflags_com)
hy_can_demo_CFLAGS              = $(_cflags_com)
hy_can_demo_CXXFLAGS            = $(_cxxflags_com)
hy_can_demo_LDFLAGS             = $(_ldflags_com)
hy_can_demo_LDADD               = $(_ldadd_com)

bin_PROGRAMS                    += hy_gpio_isr_demo
hy_gpio_isr_demo_SOURCES        = $(_sources_com) hy_gpio_isr_demo.c
hy_gpio_isr_demo_CPPFLAGS       = $(_cppflags_com)
hy_gpio_isr_demo_CFLAGS         = $(_cflags_com)
hy_gpio_isr_demo_CXXFLAGS       = $(_cxxflags_com)
hy_gpio_isr_demo_LDFLAGS        = $(_ldflags_com)
hy_gpio_isr_demo_LDADD          = $(_ldadd_com)

bin_PROGRAMS                    += hy_template_demo
hy_template_demo_SOURCES        = $(_sources_com) hy_template_demo.c
hy_template_demo_CPPFLAGS       = $(_cppflags_com)
hy_template_demo_CFLAGS         = $(_cflags_com)
hy_template_demo_CXXFLAGS       = $(_cxxflags_com)
hy_template_demo_LDFLAGS        = $(_ldflags_com)
hy_template_demo_LDADD          = $(_ldadd_com)

bin_PROGRAMS                    += hy_thread_demo
hy_thread_demo_SOURCES          = $(_sources_com) hy_thread_demo.c
hy_thread_demo_CPPFLAGS         = $(_cppflags_com)
hy_thread_demo_CFLAGS           = $(_cflags_com)
hy_thread_demo_CXXFLAGS         = $(_cxxflags_com)
hy_thread_demo_LDFLAGS          = $(_ldflags_com)
hy_thread_demo_LDADD            = $(_ldadd_com)

bin_PROGRAMS                    += hy_zone_demo
hy_zone_demo_SOURCES            = $(_sources_com) hy_zone_demo.c
hy_zone_demo_CPPFLAGS           = $(_cppflags_com)
hy_zone_demo_CFLAGS             = $(_cflags_com)
hy_zone_demo_CXXFLAGS           = $(_cxxflags_com)
hy_zone_demo_LDFLAGS            = $(_ldflags_com)
hy_zone_demo_LDADD              = $(_ldadd_com)
