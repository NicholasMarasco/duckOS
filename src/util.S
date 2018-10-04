.global put32
/* Subroutine put32
*   store a 32 bit value to an address
* Params:
*   x0 - address to write value to
*   w1 - 32 bit value to write
* Return: None
*/
put32:
  str w1, [x0]          // Write the value out to the address
  ret                   // Return to caller

.global get32
/* Subroutine get32
*   load a 32 bit value from an address
* Params:
*   x0 - address to load from
* Return: 32 bit value in w0
* Modify: x0 is destroyed
*/
get32:
  ldr w0, [x0]          // Load the value
  ret                   // Return to caller

.global delay
/* Subroutine delay
*   wait a specified number of cycles
* Params:
*   x0 - number of cycles to wait
* Return: None
* Modify: x0 is destroyed
*/
delay:
  subs x0, x0, #1       // Subtract 1 from x0 and set flags
  bne  delay            // If result was not 0, go back
  ret                   // Otherwise return to caller
