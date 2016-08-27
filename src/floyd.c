#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "algorithm_floyd.h"


FILE * matriz;

GtkWidget       *windowFloyd;
GtkWidget       *windowInitialFloyd;
GtkWidget       *g_spin_totalNodes;
GtkWidget       *g_frame_manualEntry;
GtkWidget       *g_frame_fileEntry;
GtkWidget       *g_filechooser_btn;
GtkWidget       ***entrada;
GtkWidget       ***entradaP;
GtkWidget       **rowHeader;
GtkWidget       *scrollWindow;
GtkWidget       *scrollWindowP;
GtkWidget       *g_tableDzero;
GtkWidget       *g_tableP;
GtkWidget       *dialogFloyd;
GtkWidget       *g_lbl_tableDnumber;
GtkWidget       *g_cmbFor;
GtkWidget       *g_cmbTo;
GtkWidget       *g_fixed_cmbFor_container;
GtkWidget       *g_fixed_cmbTo_container;
GtkWidget       *g_lbl_betterPath;
char **header;
char *result;

int countNodes= 0;
int totalRounds = 0;
const char *alphabet[27]={"A","B","C","D","E","F","G","H","I","J",
"K","L","M","N","Ñ","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

int loadFile=0;

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

    windowInitialFloyd = GTK_WIDGET(gtk_builder_get_object(builder, "window_initial_floyd"));
    gtk_builder_connect_signals(builder, NULL);

    dialogFloyd = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_floyd"));

    scrollWindow = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_table"));
    scrollWindowP = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_tableP"));

    g_frame_fileEntry = GTK_WIDGET(gtk_builder_get_object(builder, "frame_fileEntry"));
    gtk_widget_hide(g_frame_fileEntry);
    g_frame_manualEntry = GTK_WIDGET(gtk_builder_get_object(builder, "frame_manualEntry"));
    gtk_widget_hide(g_frame_manualEntry);

    g_lbl_betterPath = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_betterPath"));

    g_spin_totalNodes = GTK_WIDGET(gtk_builder_get_object(builder, "spn_totalNodes"));
    gtk_spin_button_set_range (GTK_SPIN_BUTTON(g_spin_totalNodes),1,27);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(g_spin_totalNodes),1,3);

    g_filechooser_btn = GTK_WIDGET(gtk_builder_get_object(builder, "filechooser_btn"));

    g_lbl_tableDnumber = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_tableDnumber"));

    g_cmbTo = GTK_WIDGET(gtk_builder_get_object(builder, "cmb_to"));
    g_cmbFor = GTK_WIDGET(gtk_builder_get_object(builder, "cmb_for"));

    g_fixed_cmbFor_container = GTK_WIDGET(gtk_builder_get_object(builder,"fixed_cmbFor_container"));
    g_fixed_cmbTo_container = GTK_WIDGET(gtk_builder_get_object(builder,"fixed_cmbTo_container"));

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
 
    gtk_widget_show(windowInitialFloyd);                
    gtk_main();
 
    return 0;
}
 
// called when window is closed
void on_window_floyd_destroy() {
  free(header);
  free(entrada);
  free(entradaP);
  gtk_main_quit();
}

void on_window_initial_floyd_destroy() {
  free(header);
  free(entrada);
  free(entradaP);
  gtk_main_quit();
}

void createTableP() {
  entradaP = calloc(countNodes,sizeof(GtkWidget**));

  g_tableP = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (scrollWindowP), g_tableP);

  for(int j = 0; j < countNodes; j++) {
    entradaP[j] = calloc(countNodes,sizeof(GtkWidget*));
  }

  for(int row =0; row < countNodes; row++) 
  {
    for(int column=0; column < countNodes; column++) 
    {
      entradaP[row][column] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY(entradaP[row][column]),8);
      gtk_widget_set_sensitive(entradaP[row][column],FALSE);
      gtk_grid_attach (GTK_GRID (g_tableP),entradaP[row][column] , column, row, 1, 1);

      if(row != 0 && column != 0){
        gtk_entry_set_text (GTK_ENTRY(entradaP[row][column]),"0");
      }
      if (row == 0 && column != 0){
        if (loadFile == 1){
          gtk_entry_set_text (GTK_ENTRY(entradaP[row][column]),header[column]);
        }
        else{
        gtk_entry_set_text (GTK_ENTRY(entradaP[row][column]),alphabet[column-1]);
      }
      }
      if (column ==0 && row!=0){
        if (loadFile ==1){
          gtk_entry_set_text (GTK_ENTRY(entradaP[row][column]),header[row]);
        }
        else{
          gtk_entry_set_text (GTK_ENTRY(entradaP[row][column]),alphabet[row-1]);  
        }
        
      }
    }
  }

}

void createTableZero(int nodes) {
  entrada = calloc(nodes,sizeof(GtkWidget**));
  countNodes = nodes;
  createTableP();

  g_tableDzero = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (scrollWindow), g_tableDzero);

  for(int j = 0; j < nodes; j++) {
    entrada[j] = calloc(nodes,sizeof(GtkWidget*));
  }

  for(int row =0; row < nodes; row++) 
  {
    for(int column=0; column < nodes; column++) 
    {
      entrada[row][column] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY(entrada[row][column]),8);
      gtk_grid_attach (GTK_GRID (g_tableDzero),entrada[row][column] , column, row, 1, 1);

      if(row==column && row != 0 && column != 0){
        gtk_entry_set_text (GTK_ENTRY(entrada[row][column]),"0");
        gtk_widget_set_sensitive(entrada[row][column],FALSE);
      }
      if (row == 0 && column != 0){
        gtk_entry_set_text (GTK_ENTRY(entrada[row][column]),alphabet[column-1]);
      }
      if (column ==0 && row!=0){
        gtk_entry_set_text (GTK_ENTRY(entrada[row][column]),alphabet[row-1]);
      }
      if (column !=0 && row!=0 && row!=column){
        gtk_entry_set_text (GTK_ENTRY(entrada[row][column]),"10000000");
      }
    }
  }

  gtk_widget_show_all(windowFloyd); 
}

void setTableFile(int Matriz[][countNodes-1]){
  int nodes=countNodes;
  entrada = calloc(nodes,sizeof(GtkWidget**));

  
  g_tableDzero = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (scrollWindow), g_tableDzero);

  for(int j = 0; j < nodes; j++) {
    entrada[j] = calloc(nodes,sizeof(GtkWidget*));
  }

  for(int row =0; row < nodes; row++) 
  {
    for(int column=0; column < nodes; column++) 
    {
      char str[10];
      entrada[row][column] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY(entrada[row][column]),8);
      gtk_grid_attach (GTK_GRID (g_tableDzero),entrada[row][column] , column, row, 1, 1);
      
      if(row==column && row != 0 && column != 0){
        gtk_entry_set_text (GTK_ENTRY(entrada[row][column]),"0");
        gtk_widget_set_sensitive(entrada[row][column],FALSE);
      }
      if (row == 0 && column != 0){
        gtk_entry_set_text (GTK_ENTRY(entrada[row][column]),header[column]);
      }
      if (column ==0 && row!=0){
        gtk_entry_set_text (GTK_ENTRY(entrada[row][column]),header[row]);
      }
      if (column !=0 && row!=0 && row!=column) {
        sprintf(str, "%d", Matriz[row-1][column-1]);
        gtk_entry_set_text (GTK_ENTRY(entrada[row][column]),str);
      }
    }
  }

  gtk_widget_show_all(windowFloyd);
}

void setTableDistance(int MatrizD[][countNodes-1]){
  int nodes = countNodes;

  char tableDInstruction[100] = "Datos obtenidos para la Tabla D(";
  char tableNumber[4];
  sprintf(tableNumber, "%d", totalRounds+1);
  strcat(tableDInstruction, tableNumber);
  strcat(tableDInstruction, ")");

  gtk_label_set_text(GTK_LABEL(g_lbl_tableDnumber), tableDInstruction);

  for(int row =0; row < nodes; row++)
  { 
    for(int column=0; column < nodes; column++) 
    {
      char str[10];
      gtk_widget_set_sensitive(entrada[row][column],FALSE);

      if (row == 0 && column != 0){
        gtk_entry_set_text (GTK_ENTRY(entrada[row][column]),header[column]);
      }
      if (column ==0 && row!=0){
        gtk_entry_set_text (GTK_ENTRY(entrada[row][column]),header[row]);
      }
      if (column !=0 && row!=0){
        sprintf(str, "%d", MatrizD[row-1][column-1]);
        
        if (strcmp(str,gtk_entry_get_text(GTK_ENTRY(entrada[row][column])))==0) {
          gtk_widget_set_name (entrada[row][column],"oldValue");
        } else {
          gtk_widget_set_name (entrada[row][column],"newValue");
        }
        gtk_entry_set_text (GTK_ENTRY(entrada[row][column]),str);
      }
    }
  }

  gtk_widget_show_all(scrollWindow); 
}

void setTableP(int matrixP[][countNodes-1]){
  int nodes = countNodes;

  for(int row =0; row < nodes; row++) 
  {
    for(int column=0; column < nodes; column++) 
    {
      char str[10];

      if (row == 0 && column != 0){
        gtk_entry_set_text (GTK_ENTRY(entradaP[row][column]),header[column]);
      }
      if (column ==0 && row!=0){
        gtk_entry_set_text (GTK_ENTRY(entradaP[row][column]),header[row]);
      }
      if (column !=0 && row!=0){
        sprintf(str, "%d", matrixP[row-1][column-1]);
        if (strcmp(str,gtk_entry_get_text(GTK_ENTRY(entradaP[row][column])))==0) {
          gtk_widget_set_name (entradaP[row][column],"oldValue");
        } else {
          gtk_widget_set_name (entradaP[row][column],"newValueP");
        }
        gtk_entry_set_text (GTK_ENTRY(entradaP[row][column]),str);
      }
    }
  }
  gtk_widget_show_all(scrollWindowP); 
}

void on_btn_spnTotalNodes_clicked() {
    int nodes = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(g_spin_totalNodes)) +1;
    printf("Numero obtenido del spin = %d \n", nodes );
    createTableZero(nodes);
    gtk_widget_hide(windowInitialFloyd);
    gtk_widget_show_now(windowFloyd);
}

void on_btn_manualEntry_clicked() {
    gtk_widget_hide(g_frame_fileEntry);
    gtk_widget_show(g_frame_manualEntry);
}

void on_btn_fileEntry_clicked() {
    gtk_widget_hide(g_frame_manualEntry);
    gtk_widget_show(g_frame_fileEntry);
}

void on_btn_filechooserBtn_clicked() {
  countNodes = countNodesFiles(gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(g_filechooser_btn)));
  loadFile = 1;
  int matrizF[countNodes-1][countNodes-1];
  
  header = malloc(countNodes * sizeof(char*));
  startFill(matrizF,gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(g_filechooser_btn)),header);
  createTableP();
  setTableFile(matrizF);
  gtk_widget_hide(windowInitialFloyd);
  gtk_widget_show_now(windowFloyd);
}

void createFile() {
  matriz = fopen("examples/programaFloyd.cvs","w+");
  header = malloc(countNodes * sizeof(char*));

  for(int row =0; row < countNodes; row++) 
  {
    for(int column=0; column < countNodes; column++) 
    {
      if (row == 0){
          header[column] = gtk_entry_get_text(GTK_ENTRY(entrada[row][column]));
       }
      fprintf(matriz,"%s;",(gtk_entry_get_text(GTK_ENTRY(entrada[row][column]))));
    }
     fprintf(matriz,"\n");
  }
  fclose(matriz);
}

void applyFloyd() {
  int MatrizD[countNodes-1][countNodes-1];
  int matrixP[countNodes-1][countNodes-1];

  for(int row =0; row < countNodes; row++) 
  {
    for(int column=0; column < countNodes; column++) 
    {
      if(row!=0 && column!=0){
        MatrizD[row-1][column-1] = atoi(gtk_entry_get_text(GTK_ENTRY(entrada[row][column])));
        matrixP[row-1][column-1] = atoi(gtk_entry_get_text(GTK_ENTRY(entradaP[row][column])));
      }
    }
  }
  floyd(MatrizD,matrixP,totalRounds);
  setTableDistance(MatrizD);
  setTableP(matrixP);
  totalRounds++;
}

void getOptimalPath(int inicio,int fin){
  char flecha[7] = " --> ";
  int medium = atoi(gtk_entry_get_text(GTK_ENTRY(entradaP[inicio][fin])));

  if (medium==0){
    printf("Ruta directa de %d a %d \n",inicio,fin);
    strcat(result,flecha);
    strcat(result, header[fin]);
  }
  else{
    strcat(result,flecha);
    strcat(result, header[medium]);
    printf("Tome %d y pase por %d \n",inicio,medium);
    getOptimalPath(medium,fin);
  }
  gtk_label_set_text (GTK_LABEL(g_lbl_betterPath),result);
}

void fillCombobox() {
  g_cmbFor = gtk_combo_box_text_new ();
  g_cmbTo = gtk_combo_box_text_new();

  for (int i = 0; i < countNodes; ++i)
  {
    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(g_cmbFor),NULL,header[i]);
    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(g_cmbTo),NULL,header[i]);
  }
  
  gtk_container_add (GTK_CONTAINER (g_fixed_cmbFor_container), g_cmbFor);
  gtk_container_add (GTK_CONTAINER (g_fixed_cmbTo_container), g_cmbTo);
}

void on_btn_getPath_clicked() {
  if (totalRounds == 0) {
    setCantidadNodos(countNodes-1);
    createFile();
    applyFloyd();
  } else {
    if (totalRounds <countNodes-1) {
      applyFloyd();
    }
    if (totalRounds == countNodes-1) {
      fillCombobox();
      gtk_widget_show_all(dialogFloyd);
    }
  }
}


void on_getOptimalPath_clicked() {
  int inicio,fin;
  free(result);
  result = malloc(sizeof(char) * 100);
  for (int i = 0; i < countNodes; ++i)
  {
    if(strcmp(header[i],gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(g_cmbFor))) == 0) {
      inicio = i;
    }
    if(strcmp(header[i],gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(g_cmbTo))) == 0) {
      fin = i;
    }
  }
  result = header[inicio];
  printf("Inicio: %d, Salida: %d\n",inicio,fin);
  getOptimalPath(inicio,fin);
}

