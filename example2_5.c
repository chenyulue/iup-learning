#include <stdlib.h>
#include "iup.h"

int btn_exit_cb(Ihandle *self)
{
    /* Exits the main loop */
    return IUP_CLOSE;
}

int main(int argc, char *argv[]) 
{
/* 
Before running any of the IUP's functions, the function IupOpen must be called 
to initialize the toolkit.
*/
    IupOpen(&argc, &argv);

/* The second parameter is a global name for a callback which use is deprecated,
so simply set it to NULL.
*/
    Ihandle *label = IupLabel("Click the button to close.");
    Ihandle *button = IupButton("OK", NULL);
    Ihandle *vbox = IupVbox(
        label,
        button,
        NULL
    );
    IupSetAttribute(vbox, "ALIGNMENT", "ARIGHT");
/* GAP defines a space in pixels between every element inside the box. */
    IupSetAttribute(vbox, "GAP", "10");
/* MARGIN defines a margin in pixels horizontally and vertically. */
    IupSetAttribute(vbox, "MARGIN", "10x10");

    Ihandle *dlg = IupDialog(vbox);
    IupSetAttribute(dlg, "TITLE", "Layout");

/* Registers callbacks. The callback function needs to be casted as Icallback.
*/
    IupSetCallback(button, "ACTION", (Icallback)btn_exit_cb);

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