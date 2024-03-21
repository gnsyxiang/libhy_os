/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_file.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/01 2024 14:27
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/01 2024      create the file
 * 
 *     last modified: 25/01 2024 14:27
 */
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "hy_file.h"

#include "hy_assert.h"
#include "hy_mem.h"
#include "hy_string.h"

#if 0
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);

成功返回文件描述符，失败返回-1

O_RDONLY　　只读打开
O_WRONLY　  只写打开
O_RDWR      读写打开

O_APPEND    将写入追加到文件的尾端
O_CREAT     若文件不存在，则创建它。使用该选项时，需要第三个参数mode，用来指定新文件的访问权限位
O_EXCL      如果同时指定了O_CREAT，而文件已经存在，则会出错
O_TRUNC     如果此文件存在，而且为只写或读写模式成功打开，则将其长度截短为0
O_NOCTTY    如果pathname指的是终端设备，则不将该设备分配作为此进程的控制终端
O_NONBLOCK  如果pathname指的是一个FIFO文件、块设备文件或字符设备文件，则此选项将文件的本次打开操作和后续的I/O操作设置为非阻塞模式
#endif

hy_s32_t HyFileIsExist(const char *file)
{
    HY_ASSERT_RET_VAL(!file, -1);

    return access(file, F_OK);
}

hy_s32_t HyFileRemove(const char *file)
{
    HY_ASSERT_RET_VAL(!file, -1);

    return remove(file);
}

hy_s64_t HyFileGetLen(const char *file)
{
    HY_ASSERT_RET_VAL(!file, -1);

    struct stat st;

    if (stat(file, &st) == 0) {
        return st.st_size;
    } else {
        return -1;
    }

#if 0
    FILE *fp = NULL;
    hy_s64_t len = 0;

    do {
        fp = fopen(file, "r");
        if (!fp) {
            LOGES("fopen %s failed, fp: %p \n", file, fp);
            break;
        }

        fseek(fp, 0, SEEK_END);
        len = ftell(fp);
        if (len == -1) {
            LOGES("ftell failed, len: -1 \n");
            break;
        }

        fclose(fp);

        return len;
    } while (0);

    if (fp) {
        fclose(fp);
    }

    return -1;
#endif
}

hy_u64_t HyFileGetLen_2(hy_s32_t fd)
{
    struct stat file_stat;

    if (fstat(fd, &file_stat) == -1) {
        LOGE("get file len error \n");
        return -1;
    } else {
        return file_stat.st_size;
    }
}

// int main() {
//     const char* filename = "example.txt";
//     int file = open(filename, O_RDONLY);
//     if (file != -1) {
//         char buffer[1024];
//         ssize_t bytesRead;
//         while ((bytesRead = read(file, buffer, sizeof(buffer))) > 0) {
//             write(STDOUT_FILENO, buffer, bytesRead);
//         }
//         close(file);
//     } else {
//         printf("Failed to open file: %s\n", filename);
//     }
//     return 0;
// }

hy_s32_t HyFileGetContent(const char *file, char **buf)
{
    HY_ASSERT_RET_VAL(!file, -1);
    HY_ASSERT_RET_VAL(!buf, -1);

    hy_s32_t fd = -1;
    struct stat st;
    char *buffer = NULL;

    do {
        fd = open(file, O_RDONLY);
        if (fd < 0) {
            LOGES("open failed \n");
            break;
        }

        if (0 != stat(file, &st)) {
            LOGES("stat failed \n");
            break;
        }

        buffer = HY_MEM_CALLOC_BREAK(char *, st.st_size);

        if (-1 == read(fd, buffer, st.st_size)) {
            LOGE("read failed \n");
            break;
        }

        *buf = buffer;

        close(fd);

        return st.st_size;
    } while (0);

    if (fd) {
        close(fd);
    }

    if (buffer) {
        free(buffer);
    }

    return -1;

#if 0
    FILE *fp = NULL;
    char *buf = NULL;
    long len = 0;

    do {
        fp = fopen(file, "r");
        if (!fp) {
            LOGES("fopen %s failed, fp: %p \n", file, fp);
            break;
        }

        fseek(fp, 0, SEEK_END);
        len = ftell(fp);
        if (len == -1) {
            LOGES("ftell failed, len: -1 \n");
            break;
        }

        buf = malloc(len);
        if (!buf) {
            LOGES("malloc failed, buf: %p \n", buf);
            break;
        }

        fseek(fp, 0, SEEK_SET);
        len = fread(buf, 1, len, fp);

        *content = buf;
        fclose(fp);

        return len;
    } while (0);

    if (fp) {
        fclose(fp);
    }

    if (buf) {
        free(buf);
    }

    return -1;
#endif
}

hy_s32_t HyFileGetContent2(const char *file, char *buf, hy_u32_t content_len)
{
    HY_ASSERT_RET_VAL(!file, -1);

    hy_s32_t fd = -1;
    struct stat st;

    do {
        fd = open(file, O_RDONLY);
        if (fd < 0) {
            LOGES("open failed \n");
            break;
        }

        if (0 != stat(file, &st)) {
            LOGES("stat failed \n");
            break;
        }

        content_len = (content_len >= st.st_size) ? st.st_size : content_len;

        if (-1 == read(fd, buf, content_len)) {
            LOGE("read failed \n");
            break;
        }

        close(fd);

        return content_len;
    } while (0);

    if (fd) {
        close(fd);
    }

    return -1;
}

hy_s32_t HyFileRead(hy_s32_t fd, void *buf, hy_u32_t len)
{
    hy_s32_t ret = 0;
    hy_u32_t cnt = 0;

    do {
        ret = read(fd, buf, len);
        if (ret < 0) {
            if (EINTR == errno || EAGAIN == errno || EWOULDBLOCK == errno) {
                LOGW("read failed, errno: %d(%s) \n", errno, strerror(errno));

                usleep(1 * 1000);
                if (++cnt >= 5) {
                    LOGE("try %d times \n", cnt);
                    ret = -1;
                    break;
                }
            } else {
                ret = -1;
                LOGES("read failed \n");
                break;
            }
        } else if (ret == 0) {
            LOGES("opposite fd close, fd: %d \n", fd);
            ret = -1;
            break;
        } else {
            break;
        }
    } while (1);

    return ret;
}

hy_s32_t HyFileReadN(hy_s32_t fd, void *buf, hy_u32_t len)
{
    hy_s32_t ret;
    hy_u32_t nleft;
    hy_u32_t offset = 0;

    nleft = len;

    while (nleft > 0) {
        ret = read(fd, buf + offset, nleft);
        // printf("file_wrapper->read, len: %d \n", ret);
        if (ret < 0) {
            if (EINTR == errno || EAGAIN == errno || EWOULDBLOCK == errno) {
                ret = 0;
            } else {
                LOGES("read failed \n");
                return -1;
            }
        } else if (ret == 0) {
            LOGES("opposite fd close, fd: %d \n", fd);
            break;
        } else {
            nleft  -= ret;
            offset += ret;
        }
    }

    return offset;
}

hy_s32_t HyFileReadTimeout(hy_s32_t fd, void *buf, hy_u32_t len, hy_u32_t ms)
{
    hy_u32_t ret;
    fd_set rfds;
    struct timeval time;

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    time.tv_sec = ms / 1000;
    time.tv_usec = (ms % 1000) * 1000;

    ret = select(fd + 1, &rfds, NULL, NULL, &time);
    switch (ret) {
        case -1:
            LOGES("select error \n");
            break;
        case 0:
            LOGT("select timeout \n");
            break;
        default:
            ret = HyFileRead(fd, buf, len);
            break;
    }

    return ret;
}

hy_s32_t HyFileReadNTimeout(hy_s32_t fd, void *buf, hy_u32_t len, hy_u32_t ms)
{
    hy_s32_t ret;
    fd_set rfds;
    struct timeval time;

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    time.tv_sec = ms / 1000;
    time.tv_usec = (ms % 1000) * 1000;

    ret = select(fd + 1, &rfds, NULL, NULL, &time);
    switch (ret) {
        case -1:
            LOGES("select error \n");
            break;
        case 0:
            LOGT("select timeout \n");
            break;
        default:
            ret = HyFileReadN(fd, buf, len);
            break;
    }

    return ret;
}

hy_s32_t HyFileWrite(hy_s32_t fd, const void *buf, hy_u32_t len)
{
    hy_s32_t ret;
    hy_u32_t cnt = 0;

    do {
        ret = write(fd, buf, len);
        if (ret < 0 && (EINTR == errno || EAGAIN == errno || EWOULDBLOCK == errno)) {
            LOGW("try again, errno: %d(%s) \n", errno, strerror(errno));

            usleep(1 * 1000);
            if (++cnt >= 5) {
                LOGE("try %d times \n", cnt);
                return -1;
            }
        } else if (ret == -1) {
            LOGES("opposite fd close, fd: %d \n", fd);
            return -1;
        } else {
            return ret;
        }
    } while (1);
}

hy_s32_t HyFileWriteN(hy_s32_t fd, const void *buf, hy_u32_t len)
{
    hy_s32_t ret;
    hy_u32_t nleft;
    const void *ptr;
    hy_u32_t cnt = 0;

    ptr   = buf;
    nleft = len;

    while (nleft > 0) {
        do {
            ret = write(fd, ptr, nleft);
            if (ret < 0 && (EINTR == errno || EAGAIN == errno || EWOULDBLOCK == errno)) {
                LOGW("try again, errno: %d(%s) \n", errno, strerror(errno));

                usleep(1 * 1000);
                if (cnt++ < 5) {
                    LOGE("try %d times \n", cnt);
                    return -1;
                }
            } else if (ret == -1) {
                LOGES("opposite fd close, fd: %d \n", fd);
                return -1;
            } else {
                nleft -= ret;
                ptr   += ret;
                break;
            }
        } while (1);
    }

    return len;
}

static hy_s32_t _set_fcntl(hy_s32_t fd, hy_s32_t arg)
{
    hy_s32_t flags;

    if ((flags = fcntl(fd, F_GETFL, 0)) == -1) {
        flags = 0;
    }

    return fcntl(fd, F_SETFL, flags | arg);
}

hy_s32_t HyFileBlockStateSet(hy_s32_t fd, HyFileBlockState_e state)
{
    hy_s32_t flags;

    flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        LOGE("fcntl failed \n");
        return -1;
    }

    if (HY_FILE_BLOCK_STATE_BLOCK == state) {
        return fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);
    } else {
        return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }
}

HyFileBlockState_e HyFileBlockStateGet(hy_s32_t fd)
{
    hy_s32_t flag;

    flag = fcntl(fd, F_GETFL, 0);
    if (flag < 0) {
        LOGES("fcntl failed \n");
    }

    if (flag & O_NONBLOCK) {
        return HY_FILE_BLOCK_STATE_NOBLOCK;
    } else {
        return HY_FILE_BLOCK_STATE_BLOCK;
    }
}

/*
 * 作用: 当fork子进程后，仍然可以使用fd。
 *       但执行exec后系统就会自动关闭进程中的fd
 */
hy_s32_t file_close_on_exec(hy_s32_t fd)
{
    return _set_fcntl(fd, FD_CLOEXEC);
}

hy_s32_t HyFileSaveBuf(const char *path, const char *buf, hy_s32_t len)
{
    FILE *fp = NULL;
    hy_s32_t ret;

    do {
        fp = fopen(path, "w");
        if (!fp) {
            LOGES("fopen failed \n");
            break;
        }

        ret = fwrite(buf, len, 1, fp);
        if (ret != 1) {
            LOGES("fwrite failed, ret: %d \n", ret);
            break;
        }

        fclose(fp);
        return 0;
    } while(0);

    fclose(fp);
    return -1;
}
