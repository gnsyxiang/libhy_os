/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_utils.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    23/01 2024 17:34
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        23/01 2024      create the file
 * 
 *     last modified: 23/01 2024 17:34
 */
#ifndef __LIBHY_OS_TYPE_INCLUDE_HY_UTILS_H_
#define __LIBHY_OS_TYPE_INCLUDE_HY_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <hy_os_type/hy_type.h>

#define HY_UTILS_IP_INT_LEN_MAX         (4)
#define HY_UTILS_IP_STR_LEN_MAX         (16)

#define HY_UTILS_MIN(_x, _y)            ((_x) < (_y) ? (_x) : (_y))                             ///< 求最小值
#define HY_UTILS_MAX(_x, _y)            ((_x) > (_y) ? (_x) : (_y))                             ///< 求最大值

#define HY_UTILS_ROUND_UP(x, size)      ((((x) + size - 1) / size ) * size)                 ///< 对size向上取整

#define HY_UTILS_UPCASE(c)              (((c) >= 'a' && (c) <= 'z') ? ((c) - 0x20) : (c))   ///< 将一个字母转换为大写

#define HY_UTILS_ARRAY_LEN(_array)      (sizeof((_array)))
#define HY_UTILS_ARRAY_ITEM_LEN(_array) (sizeof((_array)[0]))
#define HY_UTILS_ARRAY_CNT(_array)      (hy_u32_t)(sizeof((_array)) / sizeof((_array)[0]))    ///< 求数组元素的个数

#define HY_UTILS_IS_POWER_OF_2(x)       ((x) != 0 && (((x) & ((x) - 1)) == 0))              ///< 判断x是否为2^n，是返回1，否返回0

/**
 * @brief 求整除后的商
 *
 * @param _num 整数
 * @param _div 除数
 * @return 返回商
 */
#define HY_UTILS_DIV(_num, _div)                ((_num) / (_div))

/**
 * @brief 求整除8后的商
 *
 * @param _num 整数
 * @return 返回商
 */
#define HY_UTILS_DIV_8(_num)                    (HY_UTILS_DIV(_num, 8))

/**
 * @brief 求整除后的余数
 *
 * @param _num 整数
 * @param _div 除数
 * @return 返回余数
 */
#define HY_UTILS_REMAINDER(_num, _remainder)    ((_num) % (_remainder))

/**
 * @brief 求整除8后的余数
 *
 * @param _num 整数
 * @return 返回余数
 */
#define HY_UTILS_REMAINDER_8(_num)              (HY_UTILS_REMAINDER(_num, 8))

#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && defined(__ORDER_BIG_ENDIAN__)
/**
 * @brief 判断字节序
 */
#define HY_UTILS_DETERMINE_BYTE_ORDER()                     \
do {                                                        \
    if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) {        \
        LOGI("order little endian \n");                     \
    } else if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__) {    \
        LOGI("order big endian \n");                        \
    } else {                                                \
        LOGE("unable to determine machine byte order \n");  \
    }                                                       \
} while (0)
#else
#error "unable to determine machine byte order"
#endif

#if 1
/**
 * @brief 把num向上对齐到2^n幂
 *
 * @param num 数字
 *
 * @return 对齐到2^n幂的数字
 */
static inline hy_u32_t HyUtilsNumTo2N(hy_u32_t num)
{
    hy_u32_t i = 1;
    hy_u32_t num_tmp = num;

    if (num == 0) {
        return 0;
    } else {
        while (num >>= 1) {
            i <<= 1;
        }

        return (i < num_tmp) ? i << 1U : i;
    }
}
#else
// 内核的实现方式
static inline hy_s32_t Hy_fls(hy_s32_t x)
{
    hy_s32_t r;

    __asm__("bsrl %1,%0\n\t"
            "jnz 1f\n\t"
            "movl $-1,%0\n"
            "1:" : "=r" (r) : "rm" (x));

    return (r + 1);
}

/**
 * @brief 把num向上对齐到2^n幂
 *
 * @param num 数字
 *
 * @return 对齐到2^n幂的数字
 */
static inline hy_u32_t Hy_roundup_pow_of_two(hy_u32_t x)
{
    return 1UL << Hy_fls(x - 1);
}
#endif

/**
 * @brief 把IP字符串转化为整数
 *
 * @param ip_str ip字符串
 * @param ip_num ip整数
 */
void HyUtilsIpStr2Int(const char *ip_str, hy_u32_t *ip_num);

/**
 * @brief 把ip整数转化为ip字符串
 *
 * @param ip_num ip整数
 * @param ip_str ip字符串
 * @param ip_str_len ip字符串的长度
 */
void HyUtilsIpInt2Str(hy_u32_t ip_num, char *ip_str, hy_u32_t ip_str_len);

/**
 * @brief hex转化成整数，然后再转化成字符串
 *
 * @param addr 需要转换的hex数据地址
 * @param addr_len 需要转化的长度
 * @param str 转化成功后的字符串
 * @param str_len 字符串的长度
 */
void HyUtilsHex2Int2Str(char *addr, hy_u32_t addr_len, char *str, hy_u32_t str_len);

/**
 * @brief 字符串转化成整数，然后在转化成hex
 *
 * @param str 需要转化的字符串
 * @param str_len 字符串的长度
 * @param addr 转化成功后的hex数据地址
 * @param addr_len hex数据地址的长度
 */
void HyUtilsStr2Int2Hex(char *str, hy_u32_t str_len, char *addr, hy_u32_t addr_len);

/**
 * @brief 把比特数组转化成整数
 *
 * @param binary_str 比特数组
 * @param len 长度
 *
 * @return 返回整数
 */
hy_u32_t HyUtilsBitStr2Dec(char *bit_str, hy_u32_t len);

/**
 * @brief 把整数转化成bit数组
 *
 * @param num 整数
 * @param num_len 整数个数
 * @param bit_str bit数组地址
 * @param str_len bit数组长度
 */
void HyUtilsDec2BitStr(hy_u32_t num, hy_u32_t num_len, char *bit_str, hy_u32_t str_len);

/**
 * @brief 检查网络大小端序
 *
 * @return 返回1为小端序，返回0为大端序
 */
hy_s32_t HyUtilsCheckEndianness(void);

#ifdef __cplusplus
}
#endif

#endif
