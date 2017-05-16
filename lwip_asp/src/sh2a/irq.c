#include "iodefine.h"
#include "irq.h"

void irq_init(void)
{
	/* ~IRQ0/PE0 */
	PORT.PECR0.BIT.PE0MD = 3;

	/* ~IRQ2/PE2 */
	PORT.PECR0.BIT.PE2MD = 3;

	return;
}
