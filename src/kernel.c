#include "mini_uart.h"

/*
 * Procedure kernel_main
 *   entry point of the kernel, has been known to do everything
 * Params: None
 * Return: None
 */
void kernel_main(void)
{
  // Initialize our custom mini uart and send a test string
  uart_init();
  uart_send_string("Welcome to duckOS, Kweh\r\n");

  // Echo strings from the user back to them
  while(1) {
    uart_send(uart_recv());
  }
}
