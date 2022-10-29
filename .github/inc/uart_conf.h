
#ifndef UART_CONF_H_
#define UART_CONF_H_

#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

void Setup_UART(void);

UART_HandleTypeDef	hUART;

#endif /* UART_CONF_H_ */
