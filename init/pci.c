/**
 * pci.c Enum the PCI deivce.
 * 2016.6.20
 *
 */
#include <libc.h>
#include <io.h>

#define PCI_MAX_BUS  255
#define PCI_MAX_DEV  31
#define PCI_MAX_FUN  7

#define CONFIG_ADDR 0x0CF8
#define CONFIG_DATA 0x0CFC

#define PCICFG_REG_VID  0x0
#define PCICFG_REG_DID  0x2
#define PCICFG_REG_CMD  0x4
#define PCICFG_REG_STAT 0x6
#define PCICFG_REG_RID  0x8


int printf(const char *fmt,...);

void Enum_pci_device( void )
{
	unsigned int bus, dev, fun;
	unsigned int addr, data;
	char buff[32];

	for( bus = 0; bus <= PCI_MAX_BUS-250; bus++)
	{
		for( dev = 0; dev <= PCI_MAX_DEV; dev++)
		{
			for( fun = 0; fun <= PCI_MAX_FUN; fun++)
			{
				addr = 0x80000000L |(bus<<16)|(dev<<11)|(fun<<8);
				outl(addr, CONFIG_ADDR);
				data = inl(CONFIG_DATA);

				if (( data != 0xFFFFFFFF ) && (data != 0) )
				{
					sprintf(buff,"%02x:%02x:%02x %04x:%04x\n",
											bus,
											dev,
											fun,
											data & 0xFFFF,
											data >>16
					);
					printf(buff);

					addr = 0x80000000L |(bus<<16)|(dev<<11)|(fun<<8)|PCICFG_REG_RID;
					outl( addr, CONFIG_ADDR );
					data = inl( CONFIG_DATA);

					if ( data & 0xFF )
					{
						sprintf(buff, "(rev %02x)\n", data & 0xFF);
						printf(buff);
					}else{
					//	printf("\n");
					}
				}
			}
		}
	}
}
