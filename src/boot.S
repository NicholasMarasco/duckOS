#include "mm.h"

.section ".text.boot"
.global _start

/* Subroutine _start
 *   entry point for boot code
 *   gives control to processor 0 and hangs all others
 * Params: None
 * Return: None, since this OS is omnipresent
 * Modify: x0 is destroyed
 */
_start:
  mrs x0, mpidr_el1     // get multiprocessor affinity register value
  and x0, x0, #0xFF     // x0 will be 0 only when on processor 0
  cbz x0, master        // start from processor 0
  b   proc_hang         // hang all others

/* Subroutine proc_hang
 *   Death state for all ye who enter here
 * Params: None
 * Return: Never
 */
proc_hang:
  b   proc_hang         // Go directly to jail

/* Subroutine master
 *   Set up for kernel code
 * Params: None
 * Return: None
 * Modify: Destroys x0-x1
 *         Changes sp
 */
master:
  adr x0, bss_begin     // Parm 1: address of bss for memzero
  adr x1, bss_end       // address of bss_end
  sub x1, x1, x0        // Parm 2: length of bss
  bl  memzero           // initialize bss memory
  mov sp, #LOW_MEMORY   // place stack at low memory
  bl  kernel_main       // let the kernel take over
