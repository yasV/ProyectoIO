#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "algoritmo_arboles.h"

GtkWidget       *windowInitial;
GtkWidget       *windowSecond;
GtkWidget       *windowFinal;
GtkWidget       ***initialTable;
GtkWidget       *g_initialTable;
GtkWidget       *g_frame_manualEntry;
GtkWidget       *g_frame_fileEntry;
GtkWidget       *g_filechooser_btn;
GtkWidget       *g_entry_fileName;
GtkWidget       *g_spinbutton_numberKeys;
GtkWidget       *g_scrolledwindow_initialTable;
FILE            *file_tableData;

const char *rowHeader[2] = {"Texto","Peso"};
Objects *KeyList;

int main(int argc, char *argv[]) {
    GtkBuilder      *builder; 
    /*--- CSS -----------------*/
    GtkCssProvider  *provider;
    GdkDisplay      *display;
    GdkScreen       *screen;
    /*-------------------------*/
 
    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_tree.glade", NULL);
 
    windowInitial = GTK_WIDGET(gtk_builder_get_object(builder, "window_initial"));
    gtk_builder_connect_signals(builder, NULL);

    windowSecond = GTK_WIDGET(gtk_builder_get_object(builder, "window_secondWindow"));
    gtk_builder_connect_signals(builder, NULL);

    windowFinal = GTK_WIDGET(gtk_builder_get_object(builder, "window_final"));
    gtk_builder_connect_signals(builder, NULL);

    g_frame_fileEntry = GTK_WIDGET(gtk_builder_get_object(builder, "frame_fileEntry"));
    gtk_widget_hide(g_frame_fileEntry);
    g_frame_manualEntry = GTK_WIDGET(gtk_builder_get_object(builder, "frame_manualEntry"));
    gtk_widget_hide(g_frame_manualEntry);

    g_scrolledwindow_initialTable = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_initialTable"));

    g_filechooser_btn = GTK_WIDGET(gtk_builder_get_object(builder, "filechooser_btn"));
    GtkFileFilter *filter = gtk_file_filter_new ();
    gtk_file_filter_add_pattern (filter, "*.cvs");
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER(g_filechooser_btn),filter);

    g_spinbutton_numberKeys = GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton_numberKeys"));
    gtk_spin_button_set_range (GTK_SPIN_BUTTON(g_spinbutton_numberKeys),1,100);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(g_spinbutton_numberKeys),1,3);

    g_entry_fileName = GTK_WIDGET(gtk_builder_get_object(builder, "entry_fileName"));
    /*---------------- CSS ------------------------------------------------*/
    provider = gtk_css_provider_new ();
    display = gdk_display_get_default ();
    screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gsize bytes_written, bytes_read;

    const gchar* home = "style/arboles.css";  
    GError *error = 0;
  
    gtk_css_provider_load_from_path (provider,
        g_filename_to_utf8(home, strlen(home), &bytes_read, &bytes_written, &error),NULL);
    /*---------------- END CSS ------------------------------------------------*/
 
    g_object_unref(builder);
    gtk_widget_show(windowInitial);                
    gtk_main();
 
    return 0;
}

/***************FUNCIONES**********************/

void createFile(char *fileName) {
  file_tableData = fopen(fileName,"w+");

  for(int row =0; row < inputNumberKeys + 1; row++) 
  {
    for(int column=0; column < 2; column++) 
    {
      fprintf(file_tableData,"%s;",(gtk_entry_get_text(GTK_ENTRY(initialTable[row][column]))));
    }
    fprintf(file_tableData,"\n");
  }
  fclose(file_tableData);
}

void createInitialTable() {
  int keys = inputNumberKeys + 1;
  initialTable = calloc(keys,sizeof(GtkWidget**));

  g_initialTable = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_initialTable), g_initialTable);

  for(int j = 0; j < keys; j++) {
    initialTable[j] = calloc(2,sizeof(GtkWidget*));
  }

  for(int row =0; row < keys; row++) 
  {
    for(int column=0; column < 2; column++) 
    {
      initialTable[row][column] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY(initialTable[row][column]),8);
      gtk_grid_attach (GTK_GRID (g_initialTable),initialTable[row][column] , column, row, 1, 1);

      if (row == 0){
        gtk_entry_set_text (GTK_ENTRY(initialTable[row][column]),rowHeader[column]);
        gtk_widget_set_name(initialTable[row][column],"header");
        gtk_widget_set_sensitive(initialTable[row][column],FALSE);
      }
    }
  }

  gtk_widget_show_all(windowSecond); 
}

void createTableDataFile(Objects Matriz[inputNumberKeys]){
  int keys = inputNumberKeys + 1;
  initialTable = calloc(keys,sizeof(GtkWidget**));

  g_initialTable = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_initialTable), g_initialTable);

  for(int j = 0; j < keys; j++) {
    initialTable[j] = calloc(2,sizeof(GtkWidget*));
  }

  for(int row =0; row < keys; row++) 
  {
    for(int column=0; column < 2; column++) 
    {
      char str[10];
      initialTable[row][column] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY(initialTable[row][column]),8);
      gtk_grid_attach (GTK_GRID (g_initialTable),initialTable[row][column] , column, row, 1, 1);

      if (row == 0){
        gtk_entry_set_text (GTK_ENTRY(initialTable[row][column]),rowHeader[column]);
        gtk_widget_set_name(initialTable[row][column],"header");
        gtk_widget_set_sensitive(initialTable[row][column],FALSE);
      }
      if( row != 0) {
        Objects key = Matriz[row-1];

        if(column == 0){
          gtk_entry_set_text (GTK_ENTRY(initialTable[row][column]),key.texto); 
        }
        if(column == 1) {
          sprintf(str, "%d", key.peso);
          gtk_entry_set_text (GTK_ENTRY(initialTable[row][column]),str);
        }
      }
    }
  }

  gtk_widget_show_all(windowSecond); 
}

void createObjects() {
  KeyList = calloc(inputNumberKeys,sizeof(Objects));

  for (int row = 1; row < inputNumberKeys + 1;row ++)
  {
    Objects key;
    strcpy(key.texto, gtk_entry_get_text(GTK_ENTRY(initialTable[row][0])) );
    key.peso = atoi(gtk_entry_get_text(GTK_ENTRY(initialTable[row][1])));
    key.probabilidad = 0;
    KeyList[row-1] = key;
  }
}

/***************PRIMER PANTALLA**********************/
void on_window_initial_destroy() {
	gtk_main_quit();
}

void on_btn_manualEntry_clicked() {
  gtk_widget_hide(g_frame_fileEntry);
  gtk_widget_show(g_frame_manualEntry);
}

void on_btn_fileEntry_clicked() {
  gtk_widget_hide(g_frame_manualEntry);
  gtk_widget_show(g_frame_fileEntry);
}

void on_btn_getNumberKeys_clicked() {
	inputNumberKeys = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(g_spinbutton_numberKeys));

  createInitialTable();
  
  gtk_widget_hide(windowInitial);
  gtk_widget_show_now(windowSecond);
}

void on_btn_getFile_clicked(){
  inputNumberKeys = countRowsFile( gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(g_filechooser_btn)) );

  Objects matrixDataFile[inputNumberKeys];
  startFill(matrixDataFile, gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(g_filechooser_btn)) );

  createTableDataFile(matrixDataFile);

	gtk_widget_hide(windowInitial);
  gtk_widget_show_now(windowSecond);
}

/***************SEGUNDA PANTALLA**********************/
void on_window_secondWindow_destroy() {
  gtk_main_quit();
}

void on_btn_getData_clicked() {
  int lenName = strlen(gtk_entry_get_text (GTK_ENTRY(g_entry_fileName))) + 21;

  char fileName[lenName]; 
  strcpy(fileName,"examples/Arboles/");
  strcat(fileName, gtk_entry_get_text (GTK_ENTRY(g_entry_fileName)));
  strcat(fileName, ".cvs");

  createFile(fileName);
  createObjects();

  gtk_widget_hide(windowSecond);
  free(initialTable);
  gtk_widget_show_now(windowFinal);
  
}
/***************TERCERA PANTALLA**********************/
void on_window_final_destroy() {
  gtk_main_quit();
}