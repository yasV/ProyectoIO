#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "algorithm_knapsack.h"

GtkWidget       *windowInitialKnapsack;
GtkWidget 			*windowTableData;
GtkWidget       *windowFinalTable;
GtkWidget       ***tableData;
GtkWidget       ***tableFinalData;
GtkWidget 			*g_tableData;
GtkWidget       *g_tableFinalData;
GtkWidget       *g_frame_manualEntry;
GtkWidget       *g_frame_fileEntry;
GtkWidget       *g_filechooser_btn;
GtkWidget 			*g_entry_knapsackCapacity;
GtkWidget 			*g_entry_totalObjects;
GtkWidget 			*g_entry_fileName;
GtkWidget 			*g_scrolledwindow_initialTableData;
GtkWidget       *g_scrolledwindow_finalTable;
GtkWidget       *g_scrolledwindow_optimalSolution;
GtkWidget       *g_scrolledwindow_itemValue;
GtkWidget       *g_scrolledwindow_itemCost;
FILE 						*file_tableData;

const char *alphabet[27]={"A","B","C","D","E","F","G","H","I","J","K","L","M","N","Ñ","O","P","Q","R","S","T","U","V","W","X","Y","Z"};
const char *rowHeader[4] = {"","Valor","Costo","Cantidad Disponible"};
int totalObjects = 0;
int entry_knapsackCapacity = 0;
char **header;





Objects *objectProblem;


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

    windowFinalTable = GTK_WIDGET(gtk_builder_get_object(builder, "window_finalTable"));
    gtk_builder_connect_signals(builder, NULL);

    g_scrolledwindow_initialTableData = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_initialTableData"));
    g_scrolledwindow_finalTable = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_finalTable"));
    g_scrolledwindow_optimalSolution = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_optimalSolution"));
    g_scrolledwindow_itemValue = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_itemValue"));
    g_scrolledwindow_itemCost = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_itemCost"));
    

    g_frame_fileEntry = GTK_WIDGET(gtk_builder_get_object(builder, "frame_fileEntry"));
    gtk_widget_hide(g_frame_fileEntry);
    g_frame_manualEntry = GTK_WIDGET(gtk_builder_get_object(builder, "frame_manualEntry"));
    gtk_widget_hide(g_frame_manualEntry);

    g_filechooser_btn = GTK_WIDGET(gtk_builder_get_object(builder, "filechooser_btn"));

    g_entry_knapsackCapacity = GTK_WIDGET(gtk_builder_get_object(builder, "entry_knapsackCapacity"));

    g_entry_totalObjects = GTK_WIDGET(gtk_builder_get_object(builder, "entry_totalObjects"));

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

    const gchar* home = "style/knapsack.css";  
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
  free(tableData);
	gtk_main_quit();
}

void on_window_finalTable_destroy() {
  free(tableFinalData);
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
      gtk_grid_attach (GTK_GRID (g_tableData),tableData[row][column] , column, row, 1, 1);
      if (row == 0 && column != 0){
        gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),rowHeader[column]);
        gtk_widget_set_name(tableData[row][column],"rowHeader");
        gtk_widget_set_sensitive(tableData[row][column],FALSE);
      }
      if (column ==0 && row!=0){
        gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),alphabet[row-1]);
      }
      if (column ==3 && row!=0){
        gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),"INF");
      }
    }
  }
  gtk_widget_set_sensitive(tableData[0][0],FALSE);
  gtk_widget_set_name(tableData[0][0],"rowHeader");
  gtk_widget_show_all(windowTableData);
}

void createTableDataFile(int Matriz[totalObjects-1][3]){
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
      
      if(row!=0 && column==0){
        gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),header[row]); 
      }

      if (row == 0 && column != 0){
        gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),rowHeader[column]);
        gtk_widget_set_name(tableData[row][column],"rowHeader");
        gtk_widget_set_sensitive(tableData[row][column],FALSE);
      }
      if (column !=0 && row!=0) {
        sprintf(str, "%d", Matriz[row-1][column-1]);
        gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),str);
      }
    }
  }

  gtk_widget_set_sensitive(tableData[0][0],FALSE);
  gtk_widget_set_name(tableData[0][0],"rowHeader");
  gtk_widget_show_all(windowTableData);
}

void createFinalTable(Objects finalMatrix[entry_knapsackCapacity+1][totalObjects-1],int knapsackCapacity) {
  tableFinalData = calloc(knapsackCapacity,sizeof(GtkWidget**));

  g_tableFinalData = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_finalTable), g_tableFinalData);

  for(int j = 0; j < knapsackCapacity; j++) {
    tableFinalData[j] = calloc(totalObjects,sizeof(GtkWidget*));
  }

  for(int row =0; row < knapsackCapacity; row++) 
  {
    for(int column=0; column < totalObjects; column++) 
    {
      char rowStr[10];
      char value[10];
      char total[10];

      tableFinalData[row][column] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY(tableFinalData[row][column]),10);
      gtk_widget_set_sensitive(tableFinalData[row][column],FALSE);
      gtk_grid_attach (GTK_GRID (g_tableFinalData),tableFinalData[row][column] , column, row, 1, 1);
      if (row == 0 && column != 0){
        gtk_entry_set_text (GTK_ENTRY(tableFinalData[row][column]),header[column]);
        gtk_widget_set_name(tableFinalData[row][column],"rowHeader");
      }
      if (column ==0 && row!=0){
        sprintf(rowStr, "%d", row-1);
        gtk_entry_set_text (GTK_ENTRY(tableFinalData[row][column]),rowStr);
        gtk_widget_set_name(tableFinalData[row][column],"rowHeader");
      }
      if (column != 0 && row != 0) {
        Objects object = finalMatrix[row-1][column-1];
        

        char setText[100000];
        char xValue[10];
        sprintf(value, "%d", object.value);
        sprintf(total, "%d", object.totalObjects);
        sprintf(xValue, "%d", column);

        strcpy(setText, value);
        strcat(setText, ", x");
        strcat(setText, xValue);
        strcat(setText, " = ");
        strcat(setText, total);
  
        gtk_entry_set_text (GTK_ENTRY(tableFinalData[row][column]),setText);
        if (strcmp(object.color, "R") == 0) {
          gtk_widget_set_name(tableFinalData[row][column],"notGetObject");
        }
        else {
          gtk_widget_set_name(tableFinalData[row][column],"getObject");
        }
      }
    }
  }
  gtk_widget_set_sensitive(tableFinalData[0][0],FALSE);
  gtk_widget_set_name(tableFinalData[0][0],"rowHeader");
  gtk_widget_show_all(windowFinalTable);
}

void createOptimalSolutionLabel(int optimalSolution[totalObjects]) {
  char setText[100000];
  char setCost[100000];
  char setValue[100000];
  char xvalue[5];
  char arrayValue[5];
  char arrayCost[5];
  char arrayText[5];
  char knapValue[5];

  strcpy(setText, "Z = ");
  sprintf(arrayText, "%d", optimalSolution[0]);
  strcat(setText,arrayText);

  strcpy(setValue, "Z = ");
  strcpy(setCost, " ");

  for (int i = 1; i < totalObjects; ++i)
  {
    Objects object = objectProblem[i-1];

    //Solucion Optima
    sprintf(arrayText, "%d", optimalSolution[i]);
    sprintf(xvalue, "%d", i);
    strcat(setText, "  X" );
    strcat(setText,xvalue);
    strcat(setText, " = " );
    strcat(setText,arrayText);

    //Maximizar
    sprintf(arrayValue, "%d", object.value);
    sprintf(xvalue, "%d", i);
    strcat(setValue,arrayValue);
    strcat(setValue, "  X" );
    strcat(setValue,xvalue);
    strcat(setValue, " + " );
    
    //Sujeto a
    sprintf(arrayCost, "%d", object.cost);
    sprintf(xvalue, "%d", i);
    strcat(setCost,arrayCost);
    strcat(setCost, "  X" );
    strcat(setCost,xvalue);
    strcat(setCost, " + " );
  }

  strcat(setCost, "  <= " );
  sprintf(knapValue, "%d", entry_knapsackCapacity);
  strcat(setCost, knapValue);

  GtkWidget *labelOptimal = gtk_label_new(" ");
  gtk_label_set_text(GTK_LABEL(labelOptimal),setText);
  gtk_widget_set_name(labelOptimal,"newLabel");
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_optimalSolution), labelOptimal);

  GtkWidget *labelCost = gtk_label_new(" ");
  gtk_label_set_text(GTK_LABEL(labelCost),setCost);
  gtk_widget_set_name(labelCost,"newLabel");
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_itemCost), labelCost);

  GtkWidget *labelValue = gtk_label_new(" ");
  gtk_label_set_text(GTK_LABEL(labelValue),setValue);
  gtk_widget_set_name(labelValue,"newLabel");
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_itemValue), labelValue);
}

void createFile(char *fileName) {
  file_tableData = fopen(fileName,"w+");
  header = malloc(totalObjects * sizeof(char*));

  for(int row =0; row < totalObjects; row++) 
  {
    for(int column=0; column < 4; column++) 
    {
      if (column == 0){
        header[row] = gtk_entry_get_text(GTK_ENTRY(tableData[row][column]));
      }
      fprintf(file_tableData,"%s;",(gtk_entry_get_text(GTK_ENTRY(tableData[row][column]))));
    }
    fprintf(file_tableData,"\n");
  }
  fclose(file_tableData);
}

void createObjects(){
  objectProblem = malloc (totalObjects-1*sizeof(int));
  Objects data;
  for (int row = 1; row < totalObjects;row ++)
  {
    data.value = atoi(gtk_entry_get_text(GTK_ENTRY(tableData[row][1])));
    data.cost = atoi(gtk_entry_get_text(GTK_ENTRY(tableData[row][2])));
    if (strcmp(gtk_entry_get_text(GTK_ENTRY(tableData[row][3])),"INF") == 0) {
      data.totalObjects = 1000000;
    }
    else {
      data.totalObjects = atoi(gtk_entry_get_text(GTK_ENTRY(tableData[row][3])));
    }
    objectProblem[row-1] = data;
  }
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
	int entry_totalObjects = atoi(gtk_entry_get_text (GTK_ENTRY(g_entry_totalObjects)));
	printf("Entrada Manual\n");
	totalObjects = entry_totalObjects + 1;
	createTableData();
	gtk_widget_hide(windowInitialKnapsack);
  gtk_widget_show_now(windowTableData);
}

void on_btn_getFile_clicked() {
  header = malloc(totalObjects * sizeof(char*));
	totalObjects = countObjectsFiles (gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(g_filechooser_btn)));

	int matrixData[totalObjects-1][3];
	startFill(matrixData,gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(g_filechooser_btn)),header);
  printf("Entrada Archivo\n");
  createTableDataFile(matrixData);
  gtk_widget_hide(windowInitialKnapsack);
  gtk_widget_show_now(windowTableData);
}

void on_btn_getTableData_clicked() {
	int lenName = strlen(gtk_entry_get_text (GTK_ENTRY(g_entry_fileName))) + 21;
  
	char fileName[lenName]; 
  strcpy(fileName,"examples/Mochila/");
	strcat(fileName, gtk_entry_get_text (GTK_ENTRY(g_entry_fileName)));
  strcat(fileName, ".cvs");
  printf("%d\n",fileName);

  entry_knapsackCapacity = atoi(gtk_entry_get_text (GTK_ENTRY(g_entry_knapsackCapacity)));
	createFile(fileName);
  createObjects();
  setTotalObjectsCount(totalObjects-1,entry_knapsackCapacity+1);
  Objects finalMatrix[entry_knapsackCapacity+1][totalObjects-1];
  knapsackAlgorithm(finalMatrix,objectProblem);
  
  int optimalSolution[totalObjects];
  getOptimalSolution(optimalSolution,finalMatrix);
  createOptimalSolutionLabel(optimalSolution);
  //getFinalMatrix(finalMatrix,entry_knapsackCapacity+1);
  
  createFinalTable(finalMatrix,entry_knapsackCapacity+2);
  gtk_widget_hide(windowTableData);
  gtk_widget_show_now(windowFinalTable);
  
}
