#include "system.h"
#include "button.h"
#include "display.h"

int main (void)
{
    system_init ();
    display_init();
    display_pixel_set(0,0,1);
    display_pixel_set(4,0,1);
    display_pixel_set(4,6,1);
    
    while (1)
    {
    
    
    display_update();

    
    }
}
