#include <stdlib.h>
#include "iup.h"

int main(int argc, char *argv[]) 
{
/* 
Before running any of the IUP's functions, the function IupOpen must be called 
to initialize the toolkit.
*/
    IupOpen(&argc, &argv);

    Ihandle *multitext = IupText(NULL);
    Ihandle *vbox = IupVbox(
        multitext,
        NULL
    );
    IupSetAttribute(multitext, "MULTILINE", "YES");
    IupSetAttribute(multitext, "EXPAND", "YES");

    Ihandle *dlg = IupDialog(vbox);
    IupSetAttribute(dlg, "TITLE", "Simple Notepad");

/* IupText is a control that does not fit its size to its contents, so we have 
to set an initial size for the dialog, or else the result would be a very small 
dialog.
*/
    IupSetAttribute(dlg, "SIZE", "QUARTERxQUARTER");

    IupShowXY(dlg, IUP_CENTER, IUP_CENTER);

/* The SIZE attribute will work as a minimum size, so we reset the USERSIZE 
attribute, after the dialog is shown, to avoid this limitation.
*/
    IupSetAttribute(dlg, "USERSIZE", NULL);

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