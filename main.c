// gcc -rdynamic `pkg-config --cflags gtk+-3.0` -o main main.c keypad-num.c `pkg-config --libs gtk+-3.0`

#include "main.h"
#include "keypad-num.h"

GtkWidget *userNameIp, *passwordIp;
GError *error = NULL;

void keypad_handler(GtkWidget *widget, gpointer p);
void keypad_callback(u_int32_t value, GtkWidget *widget);

/**
 * @brief This is on-click signal function called from kepad.ui file
 * 
 * @param widget Pointer to source widget
 * @param p pointer to user data
 */
void keypad_handler(GtkWidget *widget, gpointer p)
{
    if (widget == userNameIp)
    {
        keypad_open(USERNAME_MIN, USERNAME_MAX, 0, widget, keypad_callback);
    }
    else if (widget == passwordIp)
    {
        keypad_open(PASSWORD_MIN, PASSWORD_MAX, 0, widget, keypad_callback);
    }
}

/**
 * @brief This is the callback function to process the entered value
 * 
 * @param value Entered value
 * @param widget Source widget
 */
void keypad_callback(u_int32_t value, GtkWidget *widget)
{
    gchar tempBuff[100];
    sprintf(tempBuff, "%d", value);

    //display entered value in text field
    gtk_entry_set_text(GTK_ENTRY(widget), tempBuff);

    // process the value
    if (widget == userNameIp)
    {
        g_print("Entered value on username : %d \n", value);
    }
    else if (widget == passwordIp)
    {
        g_print("Entered value on password : %d \n", value);
    }
}

int main(int argc, char *argv[])
{
    GObject *window;
    GtkBuilder *builder;

    gtk_init(&argc, &argv); 
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "share/css/keypadnum.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    /* Construct a GtkBuilder instance and load our UI description */
    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "share/build.ui", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }
    window = gtk_builder_get_object(builder, "_window");
    userNameIp = (GtkWidget *)gtk_builder_get_object(builder, "_usernameInput");
    passwordIp = (GtkWidget *)gtk_builder_get_object(builder, "_passwordInput");
    keypad_init();

    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();
}
