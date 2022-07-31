#include <stdlib.h>
#include "iup.h"

int main(int argc, char *argv[]) 
{
/* 
Before running any of the IUP's functions, the function IupOpen must be called 
to initialize the toolkit.
*/
    IupOpen(&argc, &argv);

    Ihandle *label = IupLabel("Label Content.");
    Ihandle *dlg = IupDialog(IupVbox(label, NULL));
    IupSetAttribute(dlg, "TITLE", "Window's Title");

    IupShowXY(dlg, IUP_CENTER, IUP_CENTER);

/* IupMainLoop tells iup to wait for events. Otherwise, the program would go on,
 end and terminate without dealing with any event.
*/
    IupMainLoop();

/*
After running the last IUP function, IupClose must be run so that the toolkit
can free internal memory and close the interface system.
*/
    IupClose();
    return EXIT_SUCCESS;
}