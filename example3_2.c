#include <stdlib.h>
#include "iup.h"

int exit_cb(void)
{
    return IUP_CLOSE;
}

int main(int argc, char *argv[]) 
{
/* 
Before running any of the IUP's functions, the function IupOpen must be called 
to initialize the toolkit.
*/
    IupOpen(&argc, &argv);

    Ihandle *multitext = IupText(NULL);
    IupSetAttribute(multitext, "MULTILINE", "YES");
    IupSetAttribute(multitext, "EXPAND", "YES");

    Ihandle *item_open = IupItem("Open", NULL);
    Ihandle *item_saveas = IupItem("Save As", NULL);
    Ihandle *item_exit = IupItem("Exit", NULL);
    IupSetCallback(item_exit, "ACTION", (Icallback)exit_cb);

    Ihandle *file_menu = IupMenu(
        item_open, 
        item_saveas,
        IupSeparator(),
        item_exit,
        NULL
    );

    Ihandle *sub1_menu = IupSubmenu("File", file_menu);
    Ihandle *menu = IupMenu(sub1_menu, NULL);

    Ihandle *vbox = IupVbox(
        multitext,
        NULL
    );

    Ihandle *dlg = IupDialog(vbox);
    IupSetAttributeHandle(dlg, "MENU", menu);
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