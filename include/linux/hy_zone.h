/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_zone.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/01 2024 17:26
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/01 2024      create the file
 * 
 *     last modified: 25/01 2024 17:26
 */
#ifndef __LIBHY_OS_INCLUDE_HY_ZONE_H_
#define __LIBHY_OS_INCLUDE_HY_ZONE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <hy_os_type/hy_type.h>

#define HY_ZONE_INFO_PATH_LEN_MAX   (64)
#define HY_ZONE_INFO_NAME_LEN_MAX   (32)

/* @fixme: <22-04-14, uos> 只增加了几个时区，在正式使用前需要补全所有时区 */

/**
 * @brief 时区
 */
typedef enum {
    HY_ZONE_TYPE_EAST,  ///< 东时区
    HY_ZONE_TYPE_WEST,  ///< 西时区
} HyZoneType_e;

/**
 * @brief 时区号
 */
typedef enum {
    HY_ZONE_NUM_0,
    HY_ZONE_NUM_1,
    HY_ZONE_NUM_2,
    HY_ZONE_NUM_3,
    HY_ZONE_NUM_4,
    HY_ZONE_NUM_5,
    HY_ZONE_NUM_6,
    HY_ZONE_NUM_7,
    HY_ZONE_NUM_8,
    HY_ZONE_NUM_9,
    HY_ZONE_NUM_10,
    HY_ZONE_NUM_11,
    HY_ZONE_NUM_12,
} HyZoneNum_e;

/**
 * @brief 时区结构体
 *
 * @note
 * 1，优先使用zoneinfo_path，这个时直接的链接文件
 * 2，其次使用zoneinfo_name，这个是各地标准时间的缩写，可以直接用putenv("TZ=CST-8")执行
 * 3，最后使用指定时区及其夏令时
 */
typedef struct {
    HyZoneType_e        type;                                       ///< 时区
    HyZoneNum_e         num;                                        ///< 时区号
    hy_s32_t            daylight;                                   ///< 夏令时
    hy_s32_t            utc_s;                                      ///< 对应时区转化到秒
    char                zoneinfo_path[HY_ZONE_INFO_PATH_LEN_MAX];   ///< 链接文件
    char                zoneinfo_name[HY_ZONE_INFO_NAME_LEN_MAX];   ///< 各地标准时间
} HyZoneInfo_s;

/**
 * @brief 配置结构体
 */
typedef struct {
    HyZoneInfo_s        zone_info;                                  ///< 时区结构体
    char                zone_file_paht[HY_ZONE_INFO_PATH_LEN_MAX];  ///< 链接文件路径
} HyZoneSaveConfig_s;

/**
 * @brief 配置结构体
 */
typedef struct {
    HyZoneSaveConfig_s  save_c;                                     ///< 配置结构体
} HyZoneConfig_s;

/**
 * @brief 创建模块
 *
 * @param zone_c 配置结构体
 *
 * @return 成功返回句柄，失败返回NULL
 *
 * @note 句柄没有用到，这样定义接口是为了与其他接口兼容
 */
void *HyZoneCreate(HyZoneConfig_s *zone_c);

/**
 * @brief 销毁模块
 *
 * @param handle 句柄的地址(二级指针)
 *
 * @note 句柄没有用到，这样定义接口是为了与其他接口兼容，可以直接传NULL
 */
void HyZoneDestroy(void **handle);

/**
 * @brief 设置时区
 *
 * @param zone_info 时区结构体
 *
 * @return 成功返回0，失败返回-1
 */
hy_s32_t HyZoneSet(HyZoneInfo_s *zone_info);

/**
 * @brief 获取时区
 *
 * @param zone_info 时区结构体
 *
 * @return 成功返回0，失败返回-1
 */
hy_s32_t HyZoneGet(HyZoneInfo_s *zone_info);

#ifdef __cplusplus
}
#endif

#endif

