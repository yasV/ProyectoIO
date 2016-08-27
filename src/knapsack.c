#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "algorithm_knapsack.h"

GtkWidget       *windowInitialKnapsack;
GtkWidget 			*windowTableData;
GtkWidget       ***tableData;
GtkWidget 			*g_tableData;
GtkWidget       *g_frame_manualEntry;
GtkWidget       *g_frame_fileEntry;
GtkWidget       *g_filechooser_btn;
GtkWidget 			*g_entry_knapsackCapacity;
GtkWidget 			*g_entry_totalObjects;
GtkWidget 			*g_entry_fileName;
GtkWidget 			*g_scrolledwindow_initialTableData;
FILE 						*file_tableData;

const char *alphabet[27]={"A","B","C","D","E","F","G","H","I","J","K","L","M","N","Ñ","O","P","Q","R","S","T","U","V","W","X","Y","Z"};
const char *rowHeader[4] = {"","Valor","Costo","Cantidad Disponible"};
int totalObjects = 0;

int main() {
    GtkBuilder      *builder; 
    /*--- CSS -----------------*/
    GtkCssProvider  *provider;
    GdkDisplay      *display;
    GdkScreen       *screen;
    /*-------------------------*/
    
 
    gtk_init(NULL, NULL);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_knapsack.glade", NULL);

    windowInitialKnapsack = GTK_WIDGET(gtk_builder_get_object(builder, "window_initial_knapsack"));
    gtk_builder_connect_signals(builder, NULL);

    windowTableData = GTK_WIDGET(gtk_builder_get_object(builder, "window_tableData"));
    gtk_builder_connect_signals(builder, NULL);

    g_scrolledwindow_initialTableData = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_initialTableData"));

    g_frame_fileEntry = GTK_WIDGET(gtk_builder_get_object(builder, "frame_fileEntry"));
    gtk_widget_hide(g_frame_fileEntry);
    g_frame_manualEntry = GTK_WIDGET(gtk_builder_get_object(builder, "frame_manualEntry"));
    gtk_widget_hide(g_frame_manualEntry);

    g_filechooser_btn = GTK_WIDGET(gtk_builder_get_object(builder, "filechooser_btn"));

    g_entry_knapsackCapacity = GTK_WIDGET(gtk_builder_get_object(builder, "entry_knapsackCapacity"));
    gtk_entry_set_placeholder_text (GTK_ENTRY(g_entry_knapsackCapacity),"Capacidad de la Mochila");

    g_entry_totalObjects = GTK_WIDGET(gtk_builder_get_object(builder, "entry_totalObjects"));
    gtk_entry_set_placeholder_text (GTK_ENTRY(g_entry_totalObjects),"Cantidad de Objetos");

    g_entry_fileName = GTK_WIDGET(gtk_builder_get_object(builder, "entry_fileName"));


    GtkFileFilter *filter = gtk_file_filter_new ();
    gtk_file_filter_add_pattern (filter, "*.cvs");
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER(g_filechooser_btn),filter);
    /*---------------- CSS ------------------------------------------------*/
    provider = gtk_css_provider_new ();
    display = gdk_display_get_default ();
    screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), 
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gsize bytes_written, bytes_read;

    const gchar* home = "style/style.css";  
    GError *error = 0;
  
    gtk_css_provider_load_from_path (provider,
    g_filename_to_utf8(home, strlen(home), &bytes_read, &bytes_written, &error),NULL);
    /*---------------- END CSS ------------------------------------------------*/
    
 
    g_object_unref(builder);
 
    gtk_widget_show(windowInitialKnapsack);                
    gtk_main();
 
    return 0;
}

void on_window_initial_knapsack_destroy() {
	gtk_main_quit();
}

void on_window_tableData_destroy() {
	gtk_main_quit();
}

void createTableData() {
	tableData = calloc(totalObjects,sizeof(GtkWidget**));

  g_tableData = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_initialTableData), g_tableData);

  for(int j = 0; j < totalObjects; j++) {
    tableData[j] = calloc(4,sizeof(GtkWidget*));
  }

  for(int row =0; row < totalObjects; row++) 
  {
    for(int column=0; column < 4; column++) 
    {
      tableData[row][column] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY(tableData[row][column]),10);
      //gtk_widget_set_sensitive(entradaP[row][column],FALSE);
      gtk_grid_attach (GTK_GRID (g_tableData),tableData[row][column] , column, row, 1, 1);
      if (row == 0 && column != 0){
        gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),rowHeader[column]);
        gtk_widget_set_name(tableData[row][column],"rowHeader");
        gtk_widget_set_sensitive(tableData[row][column],FALSE);
      }
      if (column ==0 && row!=0){
        gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),alphabet[row-1]);
      }
    }
  }
  gtk_widget_set_sensitive(tableData[0][0],FALSE);
  gtk_widget_set_name(tableData[0][0],"rowHeader");
  gtk_widget_show_all(windowTableData);
}

void createTableDataFile(int Matriz[][totalObjects-1]){
	tableData = calloc(totalObjects,sizeof(GtkWidget**));

  g_tableData = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_initialTableData), g_tableData);

  for(int j = 0; j < totalObjects; j++) {
    tableData[j] = calloc(4,sizeof(GtkWidget*));
  }

  for(int row =0; row < totalObjects; row++) 
  {
    for(int column=0; column < 4; column++) 
    {
      char str[10];
      tableData[row][column] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY(tableData[row][column]),10);
      gtk_grid_attach (GTK_GRID (g_tableData),tableData[row][column] , column, row, 1, 1);
      
      if (row == 0 && column != 0){
        gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),rowHeader[column]);
        gtk_widget_set_name(tableData[row][column],"rowHeader");
        gtk_widget_set_sensitive(tableData[row][column],FALSE);
      }
      if (column !=0 && row!=0 && row!=column) {
        sprintf(str, "%d", Matriz[row-1][column-1]);
        gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),str);
      }
    }
  }

  gtk_widget_set_sensitive(tableData[0][0],FALSE);
  gtk_widget_set_name(tableData[0][0],"rowHeader");
  gtk_widget_show_all(windowTableData);
}

void createFile(char *fileName) {
  file_tableData = fopen(fileName,"w+");

  for(int row =0; row < totalObjects; row++) 
  {
    for(int column=0; column < 4; column++) 
    {
      fprintf(file_tableData,"%s;",(gtk_entry_get_text(GTK_ENTRY(tableData[row][column]))));
    }
    fprintf(file_tableData,"\n");
  }
  fclose(file_tableData);
}

void on_btn_manualEntry_clicked() {
  gtk_widget_hide(g_frame_fileEntry);
  gtk_widget_show(g_frame_manualEntry);
}

void on_btn_fileEntry_clicked() {
  gtk_widget_hide(g_frame_manualEntry);
  gtk_widget_show(g_frame_fileEntry);
}

void on_btn_getEntries_clicked() {
	int entry_knapsackCapacity = atoi(gtk_entry_get_text (GTK_ENTRY(g_entry_knapsackCapacity)));
	int entry_totalObjects = atoi(gtk_entry_get_text (GTK_ENTRY(g_entry_totalObjects)));
	printf("La Capacidad de la mochila es: %d y habran %d tipos de objetos \n", entry_knapsackCapacity, 
		entry_totalObjects);
	totalObjects = entry_totalObjects + 1;
	createTableData();
	gtk_widget_hide(windowInitialKnapsack);
  gtk_widget_show_now(windowTableData);
}

void on_btn_getFile_clicked() {
	printf("Direccion Archivo: %s\n", gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(g_filechooser_btn)));
	totalObjects = countObjectsFiles (gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(g_filechooser_btn)));
	printf("Cantidad leida%d \n", totalObjects );

	int matrixData[totalObjects-1][4];
	startFill(matrixData,gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(g_filechooser_btn)));
	createTableDataFile(matrixData);
}

void on_btn_getTableData_clicked() {
	char direction[17] = "examples/Mochila/";
	char extension[4] = ".cvs";
	int lenName = strlen(gtk_entry_get_text (GTK_ENTRY(g_entry_fileName))) + 21;

	char fileName[lenName]; 
	strcpy(fileName, direction);
	strcat(fileName, gtk_entry_get_text (GTK_ENTRY(g_entry_fileName)));
	strcat(fileName, extension);
	printf("Nombre del archivo a generar: %s con un largo de: %d \n", fileName, lenName);

	createFile(fileName);
}