#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"
#include "hps_0.h"

#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1)

int main() {

	void *virtual_base;
	int fd;
	int loop_count;
		
	// Display mask
	int hex0;
	int hex1;
	int hex2;
	int hex3;
	int hex4;
	int hex5;
	
	// Registers pointer of 7-segment Display
	void *h2p_lw_hex0_addr;
	void *h2p_lw_hex1_addr;
	void *h2p_lw_hex2_addr;
	void *h2p_lw_hex3_addr;
	void *h2p_lw_hex4_addr;
	void *h2p_lw_hex5_addr;

	
	// map the address space for the LED registers into user space so we can interact with them.
	// we'll actually map in the entire CSR span of the HPS since we want to access various registers within that span.
	
	if (( fd = open("/dev/mem", (O_RDWR | O_SYNC ))) == -1)
	{
		printf("ERROR: could not open \"/dev/mem\"...\n" );
		return(1);
	}
	
	virtual_base = mmap(NULL, HW_REGS_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, HW_REGS_BASE);
	
	if ( virtual_base == MAP_FAILED ) {
		printf( "ERROS: mmap() failed...\n");
		close (fd);
		return(1);
	}
		
	// Get virtual address of 7-segments display controller.
	h2p_lw_hex0_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + HEX_0_BASE ) & (unsigned long) (HW_REGS_MASK));
	h2p_lw_hex1_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + HEX_1_BASE ) & (unsigned long) (HW_REGS_MASK));
	h2p_lw_hex2_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + HEX_2_BASE ) & (unsigned long) (HW_REGS_MASK));
	h2p_lw_hex3_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + HEX_3_BASE ) & (unsigned long) (HW_REGS_MASK));
	h2p_lw_hex4_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + HEX_4_BASE ) & (unsigned long) (HW_REGS_MASK));
	h2p_lw_hex5_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + HEX_5_BASE ) & (unsigned long) (HW_REGS_MASK));
	
	// Set 7-segment display mask.
	hex0 = 0x3f; // 0: 0111111 
	hex1 = 0x06; // 1: 0000110
	hex2 = 0x5b; // 2: 1011011
	hex3 = 0x4f; // 3: 1001111
	hex4 = 0x66; // 4: 1100110
	hex5 = 0x6D; // 5: 1101101
	
	
	// Assign mask to register address. Each display's segment is low-active.
	*(uint32_t *) h2p_lw_hex0_addr = ~hex0;
	*(uint32_t *) h2p_lw_hex1_addr = ~hex1;
	*(uint32_t *) h2p_lw_hex2_addr = ~hex2;
	*(uint32_t *) h2p_lw_hex3_addr = ~hex3;
	*(uint32_t *) h2p_lw_hex4_addr = ~hex4;
	*(uint32_t *) h2p_lw_hex5_addr = ~hex5;
	
	
	
	// delay loop
	loop_count = 0;
	
	while ( loop_count < 60 ) {
			
		// wait 100ms
		usleep(100*1000);
		
		// loop count
		loop_count++;
	
	} //while
		
	// clean up our memory mapping and exit
	if (munmap(virtual_base, HW_REGS_SPAN ) != 0) {
		printf("ERROR: munmap() failed...\n");
		close(fd);
		return(1);
	}
		
	close(fd);
		
	return(0);
		
}	
	
		
	
	
