#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    GtkBuilder      *builder; 
    GtkWidget       *windowMain;
    /*--- CSS -----------------*/
    GtkCssProvider  *provider;
    GdkDisplay      *display;
    GdkScreen       *screen;
    /*-------------------------*/
 
    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_main.glade", NULL);
 
    windowMain = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);

    /*---------------- CSS ------------------------------------------------*/
    provider = gtk_css_provider_new ();
    display = gdk_display_get_default ();
    screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gsize bytes_written, bytes_read;

    const gchar* home = "style/style.css";  
    GError *error = 0;
  
    gtk_css_provider_load_from_path (provider,
        g_filename_to_utf8(home, strlen(home), &bytes_read, &bytes_written, &error),NULL);
    /*---------------- END CSS ------------------------------------------------*/
 
    g_object_unref(builder);
    gtk_widget_show(windowMain);                
    gtk_main();
 
    return 0;
}
 
void on_exit_clicked() {
  gtk_main_quit();
}

void on_display_floyd() {
  system("./floyd &"); 
}

void on_btn_knapsack_clicked() {
  system("./knapsack &");
}

void on_display_pending() {
  system("./pending &"); 
}

void on_btn_equipmentReplace_clicked() {
  system("./reemplazoEquipos &"); 
}
