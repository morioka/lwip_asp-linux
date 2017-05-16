#if 0
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

#if 1
extern struct pbuf * eth_input_buf_zerocopy();
extern void eth_output_zerocopy(struct netif *netif, struct pbuf *p);
#endif
