#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "algoritmo_matrix.h"

GtkWidget       *windowInitial;
GtkWidget       *windowSecond;
GtkWidget       *windowFinal;
GtkWidget       ***initialTable;
GtkWidget       *g_initialTable;
GtkWidget       ***tableM;
GtkWidget       *g_tableM;
GtkWidget       ***tableP;
GtkWidget       *g_tableP;
GtkWidget       *g_frame_manualEntry;
GtkWidget       *g_frame_fileEntry;
GtkWidget       *g_filechooser_btn;
GtkWidget       *g_entry_fileName;
GtkWidget       *g_spinbutton_matrixNumber;
GtkWidget       *g_scrolledwindow_initialTable;
GtkWidget       *g_scrolledwindow_tableM;
GtkWidget       *g_scrolledwindow_tableP;
GtkWidget       *g_scrolledwindow_solution;
FILE            *file_tableData;


int main(int argc, char *argv[]) {
    GtkBuilder      *builder; 
    /*--- CSS -----------------*/
    GtkCssProvider  *provider;
    GdkDisplay      *display;
    GdkScreen       *screen;
    /*-------------------------*/
 
    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_matrix.glade", NULL);
 
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
    g_scrolledwindow_tableM = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_tableM"));
    g_scrolledwindow_tableP = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_tableP"));
    g_scrolledwindow_solution = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_solution"));
    

    g_filechooser_btn = GTK_WIDGET(gtk_builder_get_object(builder, "filechooser_btn"));
    GtkFileFilter *filter = gtk_file_filter_new ();
    gtk_file_filter_add_pattern (filter, "*.cvs");
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER(g_filechooser_btn),filter);

    g_spinbutton_matrixNumber = GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton_matrixNumber"));
    gtk_spin_button_set_range (GTK_SPIN_BUTTON(g_spinbutton_matrixNumber),1,100);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(g_spinbutton_matrixNumber),1,3);

    g_entry_fileName = GTK_WIDGET(gtk_builder_get_object(builder, "entry_fileName"));
    /*---------------- CSS ------------------------------------------------*/
    provider = gtk_css_provider_new ();
    display = gdk_display_get_default ();
    screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gsize bytes_written, bytes_read;

    const gchar* home = "style/matrix.css";  
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

  for(int column=0; column < inputMatrixNumber + 1; column++) 
  {
    int dValue = gtk_spin_button_get_value_as_int ( GTK_SPIN_BUTTON(initialTable[1][column]) );
    fprintf(file_tableData,"%d;", dValue);
  }
  fprintf(file_tableData,"\n");

  fclose(file_tableData);
}

void createInitialTable() {
  int keys = inputMatrixNumber + 1;
  initialTable = calloc(2,sizeof(GtkWidget**));

  g_initialTable = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_initialTable), g_initialTable);

  for(int j = 0; j < 2; j++) {
    initialTable[j] = calloc(keys,sizeof(GtkWidget*));
  }

  for(int row =0; row < 2; row++) 
  {
    for(int column=0; column < keys; column++) 
    {
      char str[4];
      char dimensionName[10] = "d";
      sprintf(str, "%d", column);
      strcat(dimensionName, str);

      if (row == 0){
        initialTable[row][column] = gtk_entry_new();
        gtk_grid_attach (GTK_GRID (g_initialTable),initialTable[row][column] , column, row, 1, 1);
        gtk_entry_set_text (GTK_ENTRY(initialTable[row][column]),dimensionName);
        gtk_widget_set_name(initialTable[row][column],"header");
        gtk_widget_set_sensitive(initialTable[row][column],FALSE);
      }
      else{
        initialTable[row][column] = gtk_spin_button_new_with_range(1,10000,1);
        gtk_grid_attach (GTK_GRID (g_initialTable),initialTable[row][column] , column, row, 1, 1);
      }
    }
  }

  gtk_widget_show_all(windowSecond); 
}

void createTableDataFile() {
  int keys = inputMatrixNumber + 1;
  initialTable = calloc(2,sizeof(GtkWidget**));

  g_initialTable = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_initialTable), g_initialTable);

  for(int j = 0; j < 2; j++) {
    initialTable[j] = calloc(keys,sizeof(GtkWidget*));
  }

  for(int row =0; row < 2; row++) 
  {
    for(int column=0; column < keys; column++) 
    {
      char str[4];
      char dimensionName[10] = "d";
      sprintf(str, "%d", column);
      strcat(dimensionName, str);

      if (row == 0){
        initialTable[row][column] = gtk_entry_new();
        gtk_grid_attach (GTK_GRID (g_initialTable),initialTable[row][column] , column, row, 1, 1);
        gtk_entry_set_text (GTK_ENTRY(initialTable[row][column]),dimensionName);
        gtk_widget_set_name(initialTable[row][column],"header");
        gtk_widget_set_sensitive(initialTable[row][column],FALSE);
      }
      else{
        initialTable[row][column] = gtk_spin_button_new_with_range(1,10000,1);
        gtk_grid_attach (GTK_GRID (g_initialTable),initialTable[row][column] , column, row, 1, 1);
        gtk_spin_button_set_value (GTK_SPIN_BUTTON(initialTable[row][column]), dValueArray[column]);
      }
    }
  }

  gtk_widget_show_all(windowSecond); 
}

void createTableM() {
  int keys = inputMatrixNumber + 1;
  tableM = calloc(keys,sizeof(GtkWidget**));

  g_tableM = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_tableM), g_tableM);

  for(int j = 0; j < keys; j++) {
    tableM[j] = calloc(keys,sizeof(GtkWidget*));
  }

  for(int row =0; row < keys; row++) 
  {
    for(int column=0; column < keys; column++) 
    {
      char str[10];
      tableM[row][column] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY(tableM[row][column]),4);
      gtk_grid_attach (GTK_GRID (g_tableM),tableM[row][column] , column, row, 1, 1);
      gtk_widget_set_sensitive(tableM[row][column],FALSE);

      if (row == 0 && column != 0){
        sprintf(str, "%d", column);
        gtk_entry_set_text (GTK_ENTRY(tableM[row][column]),str);
        gtk_widget_set_name(tableM[row][column],"header");
      }
      if (column == 0 && row != 0){
        sprintf(str, "%d", row);
        gtk_entry_set_text (GTK_ENTRY(tableM[row][column]),str);
        gtk_widget_set_name(tableM[row][column],"header");
      }
    }
  }
  gtk_widget_set_name(tableM[0][0],"header");
  gtk_widget_show_all(windowFinal);
}

void createTableP() {
  int keys = inputMatrixNumber + 1;
  tableP = calloc(keys,sizeof(GtkWidget**));

  g_tableP = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_tableP), g_tableP);

  for(int j = 0; j < keys; j++) {
    tableP[j] = calloc(keys,sizeof(GtkWidget*));
  }

  for(int row =0; row < keys; row++) 
  {
    for(int column=0; column < keys; column++) 
    {
      char str[10];
      tableP[row][column] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY(tableP[row][column]),4);
      gtk_grid_attach (GTK_GRID (g_tableP),tableP[row][column] , column, row, 1, 1);
      gtk_widget_set_sensitive(tableP[row][column],FALSE);

      if (row == 0 && column != 0){
        sprintf(str, "%d", column);
        gtk_entry_set_text (GTK_ENTRY(tableP[row][column]),str);
        gtk_widget_set_name(tableP[row][column],"header");
      }
      if (column == 0 && row != 0){
        sprintf(str, "%d", row);
        gtk_entry_set_text (GTK_ENTRY(tableP[row][column]),str);
        gtk_widget_set_name(tableP[row][column],"header");
      }
    }
  }
  gtk_widget_set_name(tableP[0][0],"header");
  gtk_widget_show_all(windowFinal);
}

void displaySolution() {
  GtkWidget* solutionText = gtk_label_new ("Aca iria la solucion");
  gtk_widget_set_name(solutionText,"newLabel");
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_solution), solutionText);
  gtk_widget_show_all(windowFinal);
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

void on_btn_getMatrixNumber_clicked() {
  inputMatrixNumber = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(g_spinbutton_matrixNumber));
  createInitialTable();
  
  gtk_widget_hide(windowInitial);
  gtk_widget_show_now(windowSecond);
}

void on_btn_getFile_clicked(){
  inputMatrixNumber = getMatrixNumber( gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(g_filechooser_btn)) );
  setData( gtk_file_chooser_get_filename( GTK_FILE_CHOOSER(g_filechooser_btn)) );
  createTableDataFile();

  gtk_widget_hide(windowInitial);
  gtk_widget_show_now(windowSecond);
}

/***************SEGUNDA PANTALLA**********************/
void on_window_secondWindow_destroy() {
  gtk_main_quit();
}

void on_btn_getData_clicked() {
  int lenName = strlen(gtk_entry_get_text (GTK_ENTRY(g_entry_fileName))) + 22;

  char fileName[lenName]; 
  strcpy(fileName,"examples/Matrices/");
  strcat(fileName, gtk_entry_get_text (GTK_ENTRY(g_entry_fileName)));
  strcat(fileName, ".cvs");

  createFile(fileName);

  createTableM();
  createTableP();
  displaySolution();
  gtk_widget_hide(windowSecond);
  gtk_widget_show_now(windowFinal);
}

/***************TERCERA PANTALLA**********************/
void on_window_final_destroy() {
  free(initialTable);
  free(tableM);
  gtk_main_quit();
}
