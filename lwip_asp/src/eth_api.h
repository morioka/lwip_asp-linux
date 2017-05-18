#ifndef LWIP_ASP_LINUX
extern void eth_init(u8_t *hwaddr); 
#else
extern void eth_init(struct netif *netif);
#endif
extern int eth_input_len(void);
extern u16_t eth_input_buf(void *payload, u16_t len);
extern void eth_input_ack(void);
extern void eth_input_end(void);
extern void eth_output_start(void);
extern void eth_output(void *payload, u16_t len);
extern void eth_output_end(void);

#ifdef LWIP_ASP_LINUX
extern u16_t eth_input_buf_netif(struct netif *netif, struct pbuf *p);
extern void eth_output_netif(struct netif *netif, struct pbuf *p);
#endif
