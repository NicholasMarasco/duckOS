.global memzero
/* Subroutine memzero
*   zero out a block of memory of specified length
* Params:
*   x0 - address of memory to zero
*   x1 - number of bytes to zero
* Return: Nothing
* Modify: x0-x1 are destroyed
*/
memzero:
  str  xzr, [x0], #8    // Store zero reg into memory at x0
                        //   then advance to next 8 bytes
  subs x1, x1, #8       // Subtract 8 bytes from x1 and set flags
                        //   we should be on a double word boundary
                        //   so this should be safe from a storage overlay
  bgt  memzero          // Go back if there's more to clear
  ret                   // Head on back to caller
