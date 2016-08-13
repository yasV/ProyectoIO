#include <gtk/gtk.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    GtkBuilder      *builder; 
    GtkWidget       *windowMain;
 
    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_main.glade", NULL);
 
    windowMain = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);
 
    g_object_unref(builder);
 
    gtk_widget_show(windowMain);                
    gtk_main();
 
    return 0;
}
 
// called when window is closed
void on_window_main_destroy() {
    gtk_main_quit();
}

void on_btn_exit_clicked() {
    gtk_main_quit();
}

void on_display_pending() {
    system("./pending &"); 
}
