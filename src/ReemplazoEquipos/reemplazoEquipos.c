#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "algoritmo_reemplazo.h"

GtkWidget       *windowInitialReplace;
GtkWidget       *windowTableData;
GtkWidget       *windowFinalTable;
GtkWidget       ***tableData;
GtkWidget       *g_tableData;
GtkWidget       *g_frame_manualEntry;
GtkWidget       *g_frame_fileEntry;
GtkWidget       *g_filechooser_btn;
GtkWidget       *g_entry_fileName;
GtkWidget       *g_spin_usefulLife;
GtkWidget       *g_spinbutton_timeLimit;
GtkWidget       *g_entry_initialCost;
GtkWidget       *g_scrolledwindow_initialTableData;
GtkWidget       *g_scrolledwindow_finalTable;
GtkWidget       *g_scrolledwindow_optimalSolution;
FILE            *file_tableData;

const char *rowHeaderInitialTable[3] = {"Año","Precio de Reventa","Mantenimiento"};
const char *rowHeaderFinalTable[4] = {"t","G(t)","Próximo","Ganancia"};
int usefulLife = 0;
InitialTable *initialData;

int main(int argc, char *argv[]) {
    GtkBuilder      *builder; 
    /*--- CSS -----------------*/
    GtkCssProvider  *provider;
    GdkDisplay      *display;
    GdkScreen       *screen;
    /*-------------------------*/
 
    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_reemplazoEquipos.glade", NULL);
 
    windowInitialReplace = GTK_WIDGET(gtk_builder_get_object(builder, "window_initial_replace"));
    gtk_builder_connect_signals(builder, NULL);

    windowTableData = GTK_WIDGET(gtk_builder_get_object(builder, "window_tableData"));
    gtk_builder_connect_signals(builder, NULL);

    windowFinalTable = GTK_WIDGET(gtk_builder_get_object(builder, "window_finalTable"));
    gtk_builder_connect_signals(builder, NULL);

    g_scrolledwindow_initialTableData = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_initialTableData"));
    g_scrolledwindow_finalTable = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_finalTable"));
    g_scrolledwindow_optimalSolution = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_optimalSolution"));

    g_frame_fileEntry = GTK_WIDGET(gtk_builder_get_object(builder, "frame_fileEntry"));
    gtk_widget_hide(g_frame_fileEntry);
    g_frame_manualEntry = GTK_WIDGET(gtk_builder_get_object(builder, "frame_manualEntry"));
    gtk_widget_hide(g_frame_manualEntry);

    g_filechooser_btn = GTK_WIDGET(gtk_builder_get_object(builder, "filechooser_btn"));
    GtkFileFilter *filter = gtk_file_filter_new ();
    gtk_file_filter_add_pattern (filter, "*.cvs");
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER(g_filechooser_btn),filter);

    g_spin_usefulLife = GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton_usefulLife"));
    gtk_spin_button_set_range (GTK_SPIN_BUTTON(g_spin_usefulLife),1,10);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(g_spin_usefulLife),1,3);

    g_spinbutton_timeLimit = GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton_timeLimit"));
    gtk_spin_button_set_range (GTK_SPIN_BUTTON(g_spinbutton_timeLimit),1,30);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(g_spinbutton_timeLimit),1,3);

    g_entry_initialCost = GTK_WIDGET(gtk_builder_get_object(builder, "entry_initialCost"));
    g_entry_fileName = GTK_WIDGET(gtk_builder_get_object(builder, "entry_fileName"));

    /*---------------- CSS ------------------------------------------------*/
    provider = gtk_css_provider_new ();
    display = gdk_display_get_default ();
    screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gsize bytes_written, bytes_read;

    const gchar* home = "style/reemplazoEquipos.css";  
    GError *error = 0;
  
    gtk_css_provider_load_from_path (provider,
        g_filename_to_utf8(home, strlen(home), &bytes_read, &bytes_written, &error),NULL);
    /*---------------- END CSS ------------------------------------------------*/
 
    g_object_unref(builder);
    gtk_widget_show(windowInitialReplace);                
    gtk_main();
 
    return 0;
}
 
void on_window_initial_replace_destroy() {
  gtk_main_quit();
}

void on_window_tableData_destroy() {
  gtk_main_quit();
}

void on_window_finalTable_destroy() {
  free(tableData);
  gtk_main_quit();
}

void createTableData() {
  tableData = calloc(usefulLife,sizeof(GtkWidget**));

  g_tableData = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_initialTableData), g_tableData);

  char rowNumber[4];

  for(int j = 0; j < usefulLife; j++) {
    tableData[j] = calloc(3,sizeof(GtkWidget*));
  }

  for(int row =0; row < usefulLife; row++) 
  {
    for(int column=0; column < 3; column++) 
    {
      tableData[row][column] = gtk_entry_new();
      gtk_grid_attach (GTK_GRID (g_tableData),tableData[row][column] , column, row, 1, 1);
      if (row == 0){
        gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),rowHeaderInitialTable[column]);
        gtk_widget_set_name(tableData[row][column],"header");
        gtk_widget_set_sensitive(tableData[row][column],FALSE);
      }
      if (column ==0 && row!=0){
        sprintf(rowNumber, "%d", row);
        gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),rowNumber);
        gtk_widget_set_name(tableData[row][column],"header");
        gtk_widget_set_sensitive(tableData[row][column],FALSE);
      }
    }
  }
  gtk_widget_show_all(windowTableData);
}

void createTableDataFile(int Matriz[usefulLife-1][2]){
  tableData = calloc(usefulLife,sizeof(GtkWidget**));

  g_tableData = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_initialTableData), g_tableData);

  char rowNumber[4];

  for(int j = 0; j < usefulLife; j++) {
    tableData[j] = calloc(3,sizeof(GtkWidget*));
  }

  for(int row =0; row < usefulLife; row++) 
  {
    for(int column=0; column < 3; column++) 
    {
      char str[10];
      tableData[row][column] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY(tableData[row][column]),10);
      gtk_grid_attach (GTK_GRID (g_tableData),tableData[row][column] , column, row, 1, 1);
      if (row == 0){
        gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),rowHeaderInitialTable[column]);
        gtk_widget_set_name(tableData[row][column],"header");
        gtk_widget_set_sensitive(tableData[row][column],FALSE);
      }
      if (column ==0 && row!=0){
        sprintf(rowNumber, "%d", row);
        gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),rowNumber);
        gtk_widget_set_name(tableData[row][column],"header");
        gtk_widget_set_sensitive(tableData[row][column],FALSE);
      }
      if (column !=0 && row!=0) {
        sprintf(str, "%d", Matriz[row-1][column-1]);
        gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),str);
      }
    }
  }
  gtk_widget_show_all(windowTableData);
}

void createFinalTableData(int timeLimit,FinalTable finalData[timeLimit+1]) {
  free(tableData);
  int ptimeLimit = timeLimit + 2;
  tableData = calloc(ptimeLimit,sizeof(GtkWidget**));

  g_tableData = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_finalTable), g_tableData);

  char rowNumber[3];

  for(int j = 0; j < ptimeLimit; j++) {
    tableData[j] = calloc(4,sizeof(GtkWidget*));
  }

  for(int row = 0; row < ptimeLimit; row++) 
  {
    for(int column=0; column < 4; column++) 
    {
      tableData[row][column] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY(tableData[row][column]),10);
      gtk_widget_set_sensitive(tableData[row][column],FALSE);
      gtk_grid_attach (GTK_GRID (g_tableData),tableData[row][column] , column, row, 1, 1);
      if (row == 0) {
        gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),rowHeaderFinalTable[column]);
        gtk_widget_set_name(tableData[row][column],"header");
        gtk_widget_set_sensitive(tableData[row][column],FALSE);
      }
    }
    if (row!=0) {
      FinalTable data = finalData[row-1];
      char gt[50];
      char prox[50];
      char profit[50];

      strcpy(prox," ");

      sprintf(rowNumber, "%d", row -1);
      gtk_entry_set_text (GTK_ENTRY(tableData[row][0]),rowNumber);
      gtk_widget_set_name(tableData[row][0],"header");

      sprintf(gt, "%d", data.value);
      gtk_entry_set_text (GTK_ENTRY(tableData[row][1]), gt);
      gtk_widget_set_name(tableData[row][1],"allEntries");
        
      
      for (int i=0;i<data.position;i++){
        char aux[3];
        sprintf(aux, "%d", data.year[i]);
        strcat(prox,aux);
        if (i<data.position-1){
          strcat(prox,",");
        }

      }
      
      gtk_entry_set_text (GTK_ENTRY(tableData[row][2]),prox);
      gtk_widget_set_name(tableData[row][2],"allEntries");

      sprintf(profit, "%d", data.profit);
      gtk_entry_set_text (GTK_ENTRY(tableData[row][3]), profit);
      gtk_widget_set_name(tableData[row][3],"allEntries");
      memset(prox,'\0',strlen(prox));
      memset(profit,'\0',strlen(profit));
    }
  }
  gtk_widget_show_all(windowFinalTable);
}

void createFile(char *fileName) {
  file_tableData = fopen(fileName,"w+");

  for(int row =0; row < usefulLife; row++) 
  {
    for(int column=0; column < 3; column++) 
    {
      fprintf(file_tableData,"%s;",(gtk_entry_get_text(GTK_ENTRY(tableData[row][column]))));
    }
    fprintf(file_tableData,"\n");
  }
  fclose(file_tableData);
}

void createObjects() {
  initialData = calloc(usefulLife,sizeof(InitialTable));
  for (int row = 1; row < usefulLife;row ++)
  {
    InitialTable data;
    data.year = atoi(gtk_entry_get_text(GTK_ENTRY(tableData[row][0])));
    data.sale = atoi(gtk_entry_get_text(GTK_ENTRY(tableData[row][1])));
    data.maintenance = atoi(gtk_entry_get_text(GTK_ENTRY(tableData[row][2])));
    initialData[row-1] = data;
  }
}

void createOptimalSolution() {
  GtkWidget *g_lblSolution = gtk_label_new (" ");
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_optimalSolution), g_lblSolution);

  gtk_widget_set_name(g_lblSolution,"label");

  gtk_widget_show_all(windowFinalTable);
}
void on_btn_manualEntry_clicked() {
  gtk_widget_hide(g_frame_fileEntry);
  gtk_widget_show(g_frame_manualEntry);
}

void on_btn_fileEntry_clicked() {
  gtk_widget_hide(g_frame_manualEntry);
  gtk_widget_show(g_frame_fileEntry);
}

void on_btn_usefulLife_clicked() {
  usefulLife = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(g_spin_usefulLife)) + 1;
  createTableData();
  gtk_widget_hide(windowInitialReplace);
  gtk_widget_show_now(windowTableData);
}

void on_btn_getFile_clicked() {
  usefulLife = countUsefulLifeFiles (gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(g_filechooser_btn)));
  int matrixData[usefulLife-1][2];
  startFill(matrixData,gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(g_filechooser_btn)));
  createTableDataFile(matrixData);

  gtk_widget_hide(windowInitialReplace);
  gtk_widget_show_now(windowTableData);
}

void on_btn_getTableData_clicked() {
  int initialCost = atoi(gtk_entry_get_text (GTK_ENTRY(g_entry_initialCost)));
  timeLimit = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(g_spinbutton_timeLimit));

  int lenName = strlen(gtk_entry_get_text (GTK_ENTRY(g_entry_fileName))) + 23;
  
  char fileName[lenName]; 
  strcpy(fileName,"examples/Reemplazo/");
  strcat(fileName, gtk_entry_get_text (GTK_ENTRY(g_entry_fileName)));
  strcat(fileName, ".cvs");
  
  createFile(fileName);
  setTotalObjectsCount(usefulLife-1, initialCost, timeLimit);
  createObjects();

  FinalTable *finalData = calloc (timeLimit+1,sizeof(FinalTable));
  replaceAlgorithm(initialData,finalData);
  free(initialData);
  createFinalTableData(timeLimit,finalData);
  planes(finalData);
  createOptimalSolution();

  gtk_widget_hide(windowTableData);
  gtk_widget_show_now(windowFinalTable);
  free(finalData);
}

