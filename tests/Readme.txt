
  All tests use a serial port console:

   - For ARM & Cortex - 115200 bauds(1 stop bit, 8 bit data, no parity)

         The all Cortex Nucleo(c) boards use the on-board ST-LINK virtual 
       serial port to connect to the terminal.

         The ARM(LPC2368) and Cortex-M3(LPC1766) boards use on-board UART0 
       to connect to the terminal.

   - For MSP430X - 38400 bauds(1 stop bit, 8 bit data, no parity)

         The MSP430X (MSP430F5359) board uses on-board UART1 to connect to 
       the terminal.

  To start tests, type in terminal "test_all" and press 'Enter'.
  The tests results will be dumped in the terminal.

  All available test commands - in the file \common\test_shell_func.c

