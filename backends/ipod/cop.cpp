
#include "backends/ipod/sdl-common.h"

#ifdef USE_COP
/*
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>
*/
#define outl(a, b) (*(volatile unsigned int *)(b) = (a))
#define inl(a) (*(volatile unsigned int *)(a))

/*
#define DMA_BUFFER        0x40000040
#define COP_HANDLER     0x4001501C
#define COP_STATUS     0x40015020
#define COP_RUNNING    (1 << 0)
#define COP_LINE_REQ    (1 << 1)
#define COP_LINE_REQ2    (2 << 1)
*/
/*
#define COP_HANDLER            0x4001501C
#define COP_STATUS            0x40015020
#define COP_CONTROL            0x60007004

 //#define COP_HANDLER     0x40016FF0
//#define COP_STATUS      0x40016FF4

#define COP_RUNNING            (1 << 0)
#define COP_LINE_REQ        (1 << 1)
#define COP_LINE_REQ_CLEAR    (2 << 1)

#define outl(a, b) \
	(*(volatile unsigned int *)(b) = (a))

#define inl(a) \
	(*(volatile unsigned int *)(a))

#define ipod_cop_clear_frameready() \
	outl(inl(COP_STATUS) &~ COP_LINE_REQ_CLEAR, COP_STATUS)

static OSystem_IPOD* graphics_system = NULL;

void ipod_cop_update_screen() {
	outl(inl(COP_STATUS) | COP_LINE_REQ, COP_STATUS);
}

void ipod_cop_sync_screen()
{
	assert(graphics_system);
	while (inl(COP_STATUS) & COP_RUNNING) // COP is active
	{
		// Get stuck in loop until told to update screen
		while ((inl(COP_STATUS) & COP_LINE_REQ) == 0);
		graphics_system->internUpdateScreen_Real();
		outl(inl(COP_STATUS) &~ COP_LINE_REQ, COP_STATUS);
	}	
}

void ipod_cop_execute(void (*function)())
{
	outl((unsigned int)function, COP_HANDLER);
	outl(0x0, COP_CONTROL);
}

void ipod_init_cop(OSystem_IPOD *ipod_g_system)
{
	printf("Starting ipod_init_cop\n");
	graphics_system = ipod_g_system;
	ipod_cop_clear_frameready();
	printf("FrameReady\n");
	ipod_cop_execute(ipod_cop_sync_screen);
	printf("COP executed\n");
	outl(COP_RUNNING, COP_STATUS); // Start COP?
	printf("Ending ipod_init_cop\n");
}
*/


/*
int cop_execute(void (*function)())
{
        outl((unsigned int)function, COP_HANDLER);
        outl(0x0, 0x60007004);
        return 1;    
}
void cop_lcd_test()
{
    while (inl(COP_STATUS) & COP_RUNNING)
    {
        while ((inl(COP_STATUS) & COP_LINE_REQ)==0);    
	    graphics_system->internUpdateScreen_Real();
            outl(inl(COP_STATUS) &~ COP_LINE_REQ, COP_STATUS);
    }
}
#define CACHE_CTL        (*(volatile unsigned long *)(0x6000c000))
#define CACHE_DISABLE    0
void ipod_init_cop(OSystem_IPOD *ipod_g_system)
{
    graphics_system = ipod_g_system;
    outl(inl(COP_STATUS) &~ COP_LINE_REQ2, COP_STATUS); // clear frameready
    cop_execute(cop_lcd_test);    
    outl(COP_RUNNING, COP_STATUS);

}
void cop_set(void (*function)())
{
}
void cop_end()
{
    outl(inl(COP_STATUS) &~ COP_RUNNING, COP_STATUS); // set to 0
    outl(inl(COP_STATUS) | COP_LINE_REQ, COP_STATUS);
}

void ipod_cop_update_screen() {
	outl(inl(COP_STATUS) | COP_LINE_REQ, COP_STATUS);
}
*/

#define COP_HANDLER            0x4001501C
#define COP_STATUS            0x40015020
#define COP_CONTROL            0x60007004
 
#define COP_RUNNING            (1 << 0)
#define COP_LINE_REQ        (1 << 1)
#define COP_LINE_REQ_CLEAR    (2 << 1)
 
#define ipod_cop_clear_frameready() \
    outl(inl(COP_STATUS) &~ COP_LINE_REQ_CLEAR, COP_STATUS)
 
 static OSystem_IPOD* graphics_system = NULL;


// Waits for main update_screen to call "ipod_cop_update_screen();"
static void ipod_cop_sync_screen()
{
    while (inl(COP_STATUS) & COP_RUNNING) // COP is active
    {
        // Get stuck in loop until told to update screen
        while ((inl(COP_STATUS) & COP_LINE_REQ) == 0);
	graphics_system->internUpdateScreen_Real();
        outl(inl(COP_STATUS) &~ COP_LINE_REQ, COP_STATUS);
    }    
}
 
// Call this now instead of ipod_update_screen()
void ipod_cop_update_screen()
{
    // Envokes ipod_cop_sync_screen() to synchronize
	ipod_cop_sync_screen();
    outl(inl(COP_STATUS) | COP_LINE_REQ, COP_STATUS);
}
 
void ipod_cop_execute(void (*function)())
{
    outl((unsigned int)function, COP_HANDLER);
    outl(0x0, COP_CONTROL);
}
 
// Call before ipod_init_video()
void ipod_init_cop(OSystem_IPOD *ipod_g_system)
{

	graphics_system = ipod_g_system;

    ipod_cop_clear_frameready();
    ipod_cop_execute(ipod_cop_sync_screen);    
    outl(COP_RUNNING, COP_STATUS); // Start COP?
}
 
// Doesn't seem to work so I don't actually call this function
void ipod_exit_cop()
{
    // Causes iPod to freeze sometimes ?
    //outl(inl(COP_STATUS) &~ COP_RUNNING, COP_STATUS); // Stop COP
    //outl(inl(COP_STATUS) | COP_LINE_REQ, COP_STATUS);
}


#endif //USE_COP

