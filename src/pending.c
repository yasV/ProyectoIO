#include <gtk/gtk.h>
int main() {
    GtkBuilder      *builder; 
    GtkWidget       *secondWindow;
 
    gtk_init(NULL, NULL);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_pending.glade", NULL);
 
    secondWindow = GTK_WIDGET(gtk_builder_get_object(builder, "window_pending"));
    gtk_builder_connect_signals(builder, NULL);
 
    g_object_unref(builder);
 
    gtk_widget_show(secondWindow);                
    gtk_main();
 
    return 0;
}
 
// called when window is closed
void on_window_pending_destroy() {
    gtk_main_quit();
}