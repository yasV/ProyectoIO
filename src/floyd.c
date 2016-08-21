#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

GtkWidget   *g_spin_totalNodes;
GtkWidget   *windowFloyd;
GtkWidget   *g_tableDzero;
GtkWidget   ***entrada;
int countNodes = 0;
const char *alphabet[27]={"A","B","C","D","E","F","G","H","I","J",
"K","L","M","N","Ñ","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

int main() {
    GtkBuilder      *builder; 
    /*--- CSS -----------------*/
    GtkCssProvider  *provider;
    GdkDisplay      *display;
    GdkScreen       *screen;
    /*-------------------------*/
 
    gtk_init(NULL, NULL);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_floyd.glade", NULL);
 
    windowFloyd = GTK_WIDGET(gtk_builder_get_object(builder, "window_floyd"));
    gtk_builder_connect_signals(builder, NULL);

    g_spin_totalNodes = GTK_WIDGET(gtk_builder_get_object(builder, "spn_totalNodes"));
    gtk_spin_button_set_range (GTK_SPIN_BUTTON(g_spin_totalNodes),1,10);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(g_spin_totalNodes),1,3);

    g_tableDzero = GTK_WIDGET(gtk_builder_get_object(builder, "grid_tableZero"));


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
    gtk_widget_show(windowFloyd);                
    gtk_main();
 
    return 0;
}
 
// called when window is closed
void on_window_floyd_destroy() {
    gtk_main_quit();
}

void createTableZero(int nodes) {
  entrada = calloc(nodes+1,sizeof(GtkWidget**));
  countNodes = nodes;

  for (int index = 0; index < nodes+1; ++index)
  {
    gtk_grid_remove_row(GTK_GRID (g_tableDzero),index);
    gtk_grid_remove_column(GTK_GRID (g_tableDzero),index);
  }

  for(int j = 0; j < nodes+1; j++) {
    entrada[j] = calloc(nodes+1,sizeof(GtkWidget*));
  }

  for(int row =0; row < nodes+1; row++) 
  {
    for(int column=0; column < nodes+1; column++) 
    {
      entrada[row][column] = gtk_entry_new();
      gtk_grid_attach (GTK_GRID (g_tableDzero),entrada[row][column] , column, row, 1, 1);

      if(row==column && row != 0 && column != 0){
        gtk_entry_set_text (GTK_ENTRY(entrada[row][column]),"0");
      }
      if (row == 0 && column != 0){
        gtk_entry_set_text (GTK_ENTRY(entrada[row][column]),alphabet[column-1]);
      }
      if (column ==0 && row!=0){
        gtk_entry_set_text (GTK_ENTRY(entrada[row][column]),alphabet[row-1]);
      }
    }
  }

  gtk_widget_show_all(g_tableDzero); 
}

void on_btn_getData_clicked() {
  printf("Nodos obtenidos: %d\n", countNodes);
  for(int row =0; row < countNodes+1; row++) 
  {
    for(int column=0; column < countNodes+1; column++) 
    {
      printf("Valor obtenido de [%d][%d] :: %s \n",row,column,gtk_entry_get_text(GTK_ENTRY(entrada[row][column])));
    }
  }
  free(entrada);
}

void on_btn_getNodes_clicked() {
    int nodes = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(g_spin_totalNodes));
    printf("Numero obtenido del spin = %d \n", nodes );
    createTableZero(nodes);
}