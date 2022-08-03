#include <stdlib.h>
#include <stdio.h>
#include "iup.h"

/* Global variable - to be used inside the menu callbacks */
Ihandle *multitext = NULL;

char *read_file(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        IupMessagef("Error", "Can't open file: %s", filename);
        return NULL;
    }

    /* Calculate file size */
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    /* Allocate memory for the file contents + nul terminator */
    char *str = malloc(size + 1);
    /* Read all data at once */
    fread(str, size, 1, file);
    /* Set the nul terminator */
    str[size] = '\0';

    if (ferror(file))
    {
        IupMessagef("Error", "Fail when reading from file: %s", filename);
    }

    fclose(file);
    return str;
}

void write_file(const char *filename, const char *str, int count)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        IupMessagef("Error", "Can't open file: %s", filename);
        return;
    }

    fwrite(str, 1, count, file);

    if (ferror(file))
    {
        IupMessagef("Error", "Fail when writing to file: %s", filename);
    }

    fclose(file);
}

int open_cb(void)
{
    Ihandle *filedlg = IupFileDlg();
    IupSetAttribute(filedlg, "DIALOGTYPE", "OPEN");
    IupSetAttribute(filedlg, "EXTFILTER", "Text Files|*.txt|All Files|*.*|");

/* IupPopup is similar to IupShow, but it restricts the user interaction only 
in the specified dialog. 
*/
    IupPopup(filedlg, IUP_CENTER, IUP_CENTER);

/* IupGetInt is used to get the integer attribute of a handle, which can 
simplify the status check. 
*/
    if (IupGetInt(filedlg, "STATUS") != -1)
    {
        char *filename = IupGetAttribute(filedlg, "VALUE");
        char *str = read_file(filename);
        if (str)
        {
            /* IupSetStrAttribute will duplicate the string internally and not
            depend on the given pointer. */
            IupSetAttribute(multitext, "VALUE", str);
            free(str);
        }
    }

/* IupDestroy remove filedlg from memory. */
    IupDestroy(filedlg);
    return IUP_DEFAULT;
}

int saveas_cb(void)
{
    Ihandle *filedlg = IupFileDlg();
    IupSetAttribute(filedlg, "DIALOGTYPE", "SAVE");
    IupSetAttribute(filedlg, "EXTFILTER", "Text Files|*.txt|All Files|*.*|");

    IupPopup(filedlg, IUP_CENTER, IUP_CENTER);

    if (IupGetInt(filedlg, "STATUS") != -1)
    {
        char *filename = IupGetAttribute(filedlg, "VALUE");
        char *str = IupGetAttribute(multitext, "VALUE");
        int count = IupGetInt(multitext, "COUNT");
        write_file(filename, str, count);
    }

    IupDestroy(filedlg);
    return IUP_DEFAULT;
}

int font_cb(void)
{
    Ihandle *fontdlg = IupFontDlg();
    char *font = IupGetAttribute(multitext, "FONT");
    IupSetStrAttribute(fontdlg, "VALUE", font);
    IupPopup(fontdlg, IUP_CENTER, IUP_CENTER);

    if (IupGetInt(fontdlg, "STATUS") == 1)
    {
        char *chosed_font = IupGetAttribute(fontdlg, "VALUE");
        IupSetStrAttribute(multitext, "FONT", chosed_font);
    }

    IupDestroy(fontdlg);
    return IUP_DEFAULT;
}

int about_cb(void)
{
    IupMessage("About", "Simple Notepad\n\nAuthors:\n  Chenyu Lue");
    return IUP_DEFAULT;
}

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

    multitext = IupText(NULL);
    IupSetAttribute(multitext, "MULTILINE", "YES");
    IupSetAttribute(multitext, "EXPAND", "YES");

    Ihandle *item_open = IupItem("Open...", NULL);
    Ihandle *item_saveas = IupItem("Save As...", NULL);
    Ihandle *item_exit = IupItem("Exit", NULL);
    Ihandle *item_font = IupItem("Font...", NULL);
    Ihandle *item_about = IupItem("About...", NULL);

    IupSetCallback(item_exit, "ACTION", (Icallback)exit_cb);
    IupSetCallback(item_open, "ACTION", (Icallback)open_cb);
    IupSetCallback(item_saveas, "ACTION", (Icallback)saveas_cb);
    IupSetCallback(item_font, "ACTION", (Icallback)font_cb);
    IupSetCallback(item_about, "ACTION", (Icallback)about_cb);

    Ihandle *file_menu = IupMenu(
        item_open,
        item_saveas,
        IupSeparator(),
        item_exit,
        NULL);
    Ihandle *format_menu = IupMenu(
        item_font,
        NULL
    );
    Ihandle *help_menu = IupMenu(
        item_about,
        NULL
    );

    Ihandle *sub_menu_file = IupSubmenu("File", file_menu);
    Ihandle *sub_menu_format = IupSubmenu("Format", format_menu);
    Ihandle *sub_menu_help = IupSubmenu("Help", help_menu);
    Ihandle *menu = IupMenu(
        sub_menu_file,
        sub_menu_format,
        sub_menu_help,
        NULL
        );

    Ihandle *vbox = IupVbox(
        multitext,
        NULL);

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