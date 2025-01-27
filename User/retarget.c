#include "gd32f30x.h"
#include "logicpi.h"

#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

uint32_t host_com;

void semihosting_init(uint32_t com)
{
    host_com = com;
    pi_com_init(host_com);
    // 禁用缓冲区，一有输出立即发送
    setvbuf(stdout, NULL, _IONBF, 0);
}

// 标准输入通过UART_ReceiveData读入，每次仅能读取一个字符
int _read(int f, char *ptr, int len)
{
    if (f == STDIN_FILENO) {
        while (usart_flag_get(host_com, USART_FLAG_RBNE) == RESET)
            ;
        ptr[0] = usart_data_receive(host_com);
        return 1;
    }
    errno = EBADF;
    return -1;
}

// 标准输出通过UART_SendData逐字符发出
int _write(int f, char *ptr, int len)
{
    if (f == STDOUT_FILENO || f == STDERR_FILENO) {
        int i;
        for (i = 0; i < len; i++) {
            usart_data_transmit(host_com, (uint16_t)ptr[i]);
            while (RESET == usart_flag_get(host_com, USART_FLAG_TBE));
        }
        return len;
    }
    errno = EBADF;
    return -1;
}

// 禁用读写指针移动
int _lseek(int f, int ptr, int dir)
{
    (void)f;
    (void)ptr;
    (void)dir;

    errno = EBADF;
    return -1;
}

// 假关闭，直接返回关闭成功
int _close(int f)
{
    if (f >= STDIN_FILENO && f <= STDERR_FILENO)
        return 0;

    errno = EBADF;
    return -1;
}

// 仅标准输入输出与终端绑定，其余均不可用
int _isatty(int f)
{
    if (f >= STDIN_FILENO && f <= STDERR_FILENO)
        return 1;

    errno = EBADF;
    return 0;
}

// 标准输入输出为显示字符的终端设备，其余输入输出无信息
int _fstat(int f, struct stat *st)
{
    if (f >= STDIN_FILENO && f <= STDERR_FILENO) {
        st->st_mode = S_IFCHR;
        return 0;
    }

    errno = EBADF;
    return 0;
}