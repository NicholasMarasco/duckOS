
#ifndef __MINI_UART_H
#define __MINI_UART_H

void uart_init(void);
char uart_recv(void);
void uart_send(char c);
void uart_send_string(char* str);

#endif /* __MINI_UART_H */
