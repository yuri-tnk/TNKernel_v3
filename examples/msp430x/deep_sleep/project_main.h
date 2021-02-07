#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H

extern TN_DQUEUE * queueShellEvents;

 //-- Shell - uses UART1

#define  uart_tx_str(str)    uart1_tx_str(str)
#define  uart_tx_char(ch)    uart1_tx_char(ch)

#define  uart_tx_str_func    uart1_tx_str
#define  uart_tx_char_func   uart1_tx_char

#endif /* #ifndef PROJECT_MAIN_H */

