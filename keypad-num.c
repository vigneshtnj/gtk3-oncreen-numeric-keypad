#include "keypad-num.h"

#define KEY0 0
#define KEY1 1
#define KEY2 2
#define KEY3 3
#define KEY4 4
#define KEY5 5
#define KEY6 6
#define KEY7 7
#define KEY8 8
#define KEY9 9
#define KEYCLEAR 10
#define KEYENTER 11
#define KEYCLOSE 12
#define KEYUP 13
#define KEYDOWN 14

typedef struct
{
    u_int32_t data;
    u_int32_t min;
    u_int32_t max;
    GtkWidget *sourceWidget;
    callbackFn fn;
} keypad_data;

GtkWidget *keypadBtn[15];
GtkWidget *label_range, *label_entry, *keypad_window;
GError *errork = NULL;

static keypad_data keypad_source;
static callbackFn callbackFn_source;
static gboolean FIRST_FLAG = TRUE;
gchar buff[20];

GdkCursor *k_hideCursor;
GdkWindow *k_gdk_window;
GdkDisplay *k_display;
GtkBuilder *keypad;

void keypadpress_handler(GtkWidget *widget);
void int_concat(gchar num);
void int_backspace();
void keypad_enter()

static void keypadWindowRealize(GtkWidget *window, gpointer data)
{
    k_display = gdk_display_get_default();
    k_hideCursor = gdk_cursor_new_for_display(k_display, GDK_BLANK_CURSOR);
    k_gdk_window = gtk_widget_get_window(keypad_window);
    gdk_window_set_cursor(k_gdk_window, k_hideCursor);
}

void keypad_init()
{
    keypad = gtk_builder_new();
    if (gtk_builder_add_from_file(keypad, "share/keypad.ui", &errork) == 0)
    {
        g_printerr("Error loading file: %s\n", errork->message);
        g_clear_error(&errork);
    }
    keypad_window = (GtkWidget *)gtk_builder_get_object(keypad, "_keypad");
    keypadBtn[KEY1] = (GtkWidget *)gtk_builder_get_object(keypad, "_key1");
    keypadBtn[KEY2] = (GtkWidget *)gtk_builder_get_object(keypad, "_key2");
    keypadBtn[KEY3] = (GtkWidget *)gtk_builder_get_object(keypad, "_key3");
    keypadBtn[KEY4] = (GtkWidget *)gtk_builder_get_object(keypad, "_key4");
    keypadBtn[KEY5] = (GtkWidget *)gtk_builder_get_object(keypad, "_key5");
    keypadBtn[KEY6] = (GtkWidget *)gtk_builder_get_object(keypad, "_key6");
    keypadBtn[KEY7] = (GtkWidget *)gtk_builder_get_object(keypad, "_key7");
    keypadBtn[KEY8] = (GtkWidget *)gtk_builder_get_object(keypad, "_key8");
    keypadBtn[KEY9] = (GtkWidget *)gtk_builder_get_object(keypad, "_key9");
    keypadBtn[KEY0] = (GtkWidget *)gtk_builder_get_object(keypad, "_key0");
    keypadBtn[KEYENTER] = (GtkWidget *)gtk_builder_get_object(keypad, "_keyok");
    keypadBtn[KEYCLEAR] = (GtkWidget *)gtk_builder_get_object(keypad, "_keyclear");
    keypadBtn[KEYCLOSE] = (GtkWidget *)gtk_builder_get_object(keypad, "_keyclose");
    keypadBtn[KEYUP] = (GtkWidget *)gtk_builder_get_object(keypad, "_keyup");
    keypadBtn[KEYDOWN] = (GtkWidget *)gtk_builder_get_object(keypad, "_keydown");
    label_range = (GtkWidget *)gtk_builder_get_object(keypad, "_keyrange");
    label_entry = (GtkWidget *)gtk_builder_get_object(keypad, "_keydata");

    gtk_widget_set_events(keypad_window, GDK_FOCUS_CHANGE_MASK);
    g_signal_connect(G_OBJECT(keypad_window), "focus-out-event", G_CALLBACK(keypad_close), NULL);
    gtk_builder_connect_signals(keypad, NULL);
    //g_signal_connect(keypad_window, "realize", G_CALLBACK(keypadWindowRealize), NULL);        //uncomment to hide pointer
}

void keypad_open(u_int32_t min, u_int32_t max, u_int32_t data, GtkWidget *source, callbackFn fn)
{
    FIRST_FLAG = TRUE;
    char str[20];

    keypad_source.min = min;
    keypad_source.max = max;
    keypad_source.data = data;
    keypad_source.sourceWidget = source;

    gtk_entry_set_text(GTK_ENTRY(label_entry), "");
    gtk_label_set_text(GTK_LABEL(label_range), "");

    if (GTK_IS_ENTRY(source))
    {
        if (gtk_entry_get_input_purpose(GTK_ENTRY(source)) == GTK_INPUT_PURPOSE_PASSWORD)
        {
            gtk_entry_set_input_purpose(GTK_ENTRY(label_entry), GTK_INPUT_PURPOSE_PASSWORD);
            gtk_entry_set_visibility(GTK_ENTRY(label_entry), FALSE);
        }
        else
        {
            gtk_entry_set_input_purpose(GTK_ENTRY(label_entry), GTK_INPUT_PURPOSE_FREE_FORM);
            gtk_entry_set_visibility(GTK_ENTRY(label_entry), TRUE);
            sprintf(str, "Min: %d\nMax: %d", keypad_source.min, keypad_source.max);
            gtk_label_set_text(GTK_LABEL(label_range), str);
            sprintf(str, "%d", keypad_source.data);
            gtk_entry_set_text(GTK_ENTRY(label_entry), str);
        }
    }
    else if (GTK_IS_LABEL(source))
    {
        gtk_entry_set_input_purpose(GTK_ENTRY(label_entry), GTK_INPUT_PURPOSE_FREE_FORM);
        gtk_entry_set_visibility(GTK_ENTRY(label_entry), TRUE);
        sprintf(str, "Min: %d\nMax: %d", keypad_source.min, keypad_source.max);
        gtk_label_set_text(GTK_LABEL(label_range), str);
        sprintf(str, "%d", keypad_source.data);
        gtk_entry_set_text(GTK_ENTRY(label_entry), str);
    }

    gtk_widget_show_all(keypad_window);
    callbackFn_source = fn;
}

void keypad_close()
{
    FIRST_FLAG = FALSE;
    gtk_widget_hide(keypad_window);
}

void keypad_enter()
{
    if (keypad_source.data >= keypad_source.min)
    {
        if (keypad_source.data <= keypad_source.max)
        {
            keypad_close();
            callbackFn_source(keypad_source.data, keypad_source.sourceWidget);
        }
    }
    else
    {
        keypad_close();
    }
}

void keypadpress_handler(GtkWidget *widget)
{
    gchar *temp, num;
    if (GTK_IS_BUTTON(widget))
    {
        if (widget == keypadBtn[KEYCLEAR])
        {
            FIRST_FLAG = FALSE;
            int_backspace();
        }
        else if (widget == keypadBtn[KEYUP])
        {
            if (keypad_source.data < keypad_source.max)
            {
                FIRST_FLAG = FALSE;
                keypad_source.data++;
                sprintf(buff, "%d", keypad_source.data);
                gtk_entry_set_text(GTK_ENTRY(label_entry), buff);
            }
        }
        else if (widget == keypadBtn[KEYDOWN])
        {
            if (keypad_source.data > keypad_source.min)
            {
                FIRST_FLAG = FALSE;
                keypad_source.data--;
                sprintf(buff, "%d", keypad_source.data);
                gtk_entry_set_text(GTK_ENTRY(label_entry), buff);
            }
        }
        else if (gtk_button_get_label(GTK_BUTTON(widget)) != NULL)
        {
            temp = (gchar *)gtk_button_get_label(GTK_BUTTON(widget));
            num = temp[0] - '0';
            if ((num < 10) && (num > -1))
            {
                int_concat(num);
            }
        }
    }
    else
    {
        //error
        g_print("keypad-num.c,L:113 :something went wrong \n");
    }
}

void int_concat(gchar num)
{
    u_int32_t value;
    if (FIRST_FLAG == TRUE)
    {
        value = num;
        FIRST_FLAG = FALSE;
    }
    else
    {
        value = (keypad_source.data * 10) + num;
    }

    if ((value <= keypad_source.max))
    {
        keypad_source.data = value;
        sprintf(buff, "%d", keypad_source.data);
        gtk_entry_set_text(GTK_ENTRY(label_entry), buff);
    }
    else
    {
        keypad_source.data = num;
        sprintf(buff, "%d", num);
        gtk_entry_set_text(GTK_ENTRY(label_entry), buff);
    }
}

void int_backspace()
{
    u_int32_t value;
    value = keypad_source.data / 10;
    keypad_source.data = C_CLAMP(value, keypad_source.min, keypad_source.max, 0);

    if (keypad_source.data != 0)
    {
        sprintf(buff, "%d", keypad_source.data);
        gtk_entry_set_text(GTK_ENTRY(label_entry), buff);
    }
    else
    {
        gtk_entry_set_text(GTK_ENTRY(label_entry), "");
    }
}
