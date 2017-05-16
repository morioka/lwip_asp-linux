#include <kernel.h>
#include "kernel_cfg.h"
#include "debug.h"

#include "lwip/api.h" 
#include "lwip/tcpip.h"

extern void http_server_netconn_init(void);
extern void  ethernetif_input(struct netif *netif);

void task_lwip(intptr_t exinf)
{
	struct ip_addr xIpAddr, xNetMast, xGateway;
	struct netif EMAC_if;
	extern err_t ethernetif_init( struct netif *netif );

	dly_tsk(3 * 1000);

	debug_init();
	tcpip_init( NULL, NULL );

	/* Create and configure the EMAC interface. */
	IP4_ADDR(&xIpAddr, 192, 168, 1, 150);
	IP4_ADDR(&xNetMast, 255, 255, 255 ,0);
	IP4_ADDR(&xGateway, 192, 168, 1, 1);
	netif_add(&EMAC_if, &xIpAddr, &xNetMast, &xGateway, NULL, ethernetif_init, tcpip_input);

	/* make it the default interface */
	netif_set_default(&EMAC_if);

	/* bring it up */
	netif_set_up(&EMAC_if);

	http_server_netconn_init();

	while(1)
	{
		wai_sem(SEM_RECV);
		ethernetif_input(&EMAC_if);
	}
}
