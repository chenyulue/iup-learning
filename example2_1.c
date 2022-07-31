#include <stdlib.h>
#include "iup.h"

int main(int argc, char* argv[]) 
{   
/* 
Before running any of the IUP's functions, the function IupOpen must be called 
to initialize the toolkit.
*/
    IupOpen(&argc, &argv);

    IupMessage("Message Title", "Message Content.");

/*
After running the last IUP function, IupClose must be run so that the toolkit
can free internal memory and close the interface system.
*/
    IupClose();
    return EXIT_SUCCESS;
}