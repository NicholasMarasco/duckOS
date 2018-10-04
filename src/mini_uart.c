#include "utils.h"
#include "peripherals/mini_uart.h"
#include "peripherals/gpio.h"

/*
 * Procedure uart_send
 *   writes a character to the UART
 * Params: c - character to write
 * Return: None
 */
void uart_send(char c)
{
  // Bit 5 of AUX_MU_LSR_REG will be on when the transmitter is empty, meaning
  // it can be written to, so wait until then before sending
  while (1) {
    if (get32(AUX_MU_LSR_REG) & 0x20)
      break;
  }
  put32(AUX_MU_IO_REG, c);
}

/*
 * Procedure uart_recv
 *   reads a character from the UART
 * Params: None
 * Return: character value read from the UART
 */
char uart_recv(void)
{
  // Bit 1 of AUX_MU_LSR_REG will be on when the data is ready to be read,
  // so wait for that before reading
  while (1){
    if (get32(AUX_MU_LSR_REG) & 0x01)
      break;
  }
  // AND with 0xFF to clear all but the byte being returned
  return get32(AUX_MU_IO_REG) & 0xFF;
}

/*
 * Procedure uart_send_string
 *   send a sequence of characters to the UART
 * Params: str - sequence of chars to send
 * Return: None
 */
void uart_send_string(char* str)
{
  // Send null terminated string
  for (int i = 0; str[i] != '\0'; i++){
    uart_send((char) str[i]);
  }
}

/*
 * Procedure uart_init
 *   initialize the uart for transmission
 * Params: None
 * Return: None
 */
void uart_init(void)
{
  unsigned int selector;

  /* Grab the memory mapped GPIO alternate function register 1, which controls
   * pins 10-19. The serial cable is attached to pins 14 and 15, which also have
   * TXD1 and RXD1 (UART1 Transmit and Receive Data respectively) as
   * alternative functions. (Fuck if I know the difference between UART0 and
   * UART1. It looks like they're just different UART ports, but I have no idea
   * if they function differently or not. If I was saucy I could try setting the
   * alternative function to UART0 and try to use it, but I'm not...right now)
   */
  selector  = get32(GPFSEL1);
  /* Bits 12-14 control pin 14 functions, so clear that before setting
   * 0000 0000 0000 0000 0000 0000 0000 0111 7
   * 0000 0000 0000 0000 0111 0000 0000 0000 << (SHIFT LEFT) 12
   * 1111 1111 1111 1111 1000 1111 1111 1111 ~ (NOT)
   * & (AND) will preserve every bit except the ones that are zero
   */
  selector &= ~(7<<12);
  /* Set bit 13 to turn on alternate function 5
   * 1<<13 would also work, but the documentation shows setting the triple of
   * bits to 010, so 2<<12 is arguably more readable
   */
  selector |= 2<<12;
  // Bits 15-17 control pin 15, so similar case as above
  selector &= ~(7<<15);
  selector |= 2<<15;
  // Write the new value back to the register
  put32(GPFSEL1, selector);

  /* Set the GPIO Pull Up/Down register to zero (disable pulling control signal)
   * Since pin 14 and 15 are connected and being used for IO, then having them
   * always be on or off would probably be bad
   */
  put32(GPPUD, 0);
  // Wait 150 cycles for the control signal to be set up in the hardware
  delay(150);
  /* GPIO Pull Up/Down Clock Register 0 sends the control signal to
   * the specificed pin. (CLK0 controls pins 0-31, CLK1 controls the rest)
   * So tell CLK0 to update the control signal on pins 14 and 15
   */
  put32(GPPUDCLK0, (1<<14)|(1<<15));
  // Wait another 150 cycles for the hardware to update the signal
  delay(150);
  /* At this point you would write zero to GPPUD to remove the control signal.
   * Since the control signal wrote was zero, there's nothing to remove.
   * Now write zero to GPPUDCLK0 to remove the clock value, so stops trying to
   * set pins 14 and 15. I'm not 100% sure about that, but it sounds good.
   */
  put32(GPPUDCLK0, 0);

  // If bit 0 is on, mini UART is enabled (along with its registers accessible)
  put32(AUX_ENABLES, 1);
  // Temporarily disable the mini UART for configuration (along with auto flow)
  put32(AUX_MU_CNTL_REG, 0);
  // Disable receive and transmit interrupts because it's easier
  put32(AUX_MU_IER_REG, 0);
  // Set bits 0 and 1 to on to set the UART to work in 8 bit mode
  put32(AUX_MU_LCR_REG, 3);
  // Set the RTS line to high which means something if caring about auto flow
  put32(AUX_MU_MCR_REG, 0);
  // Set the mini UART baud rate counter
  // Adjust BAUD_RATE in peripherals/mini_uart.h
  put32(AUX_MU_BAUD_REG, BAUD_REG_VAL);
  // Turn on the bits to activate the transmiter and receiver
  put32(AUX_MU_CNTL_REG, 3);
}
