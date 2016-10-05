#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "algoritmo_series.h"

GtkWidget       *windowInitial;
GtkWidget       *windowSecond;
GtkWidget       *windowFinal;
GtkWidget       **inputFormatSerie;
GtkWidget       ***tableData;
GtkWidget       *g_tableData;
GtkWidget       *g_frame_manualEntry;
GtkWidget       *g_frame_fileEntry;
GtkWidget       *g_filechooser_btn;
GtkWidget       *g_entry_fileName;
GtkWidget       *g_entry_probabilidadA;
GtkWidget       *g_entry_probabilidadB;
GtkWidget       *g_spinbutton_games;
GtkWidget       *g_spinbutton_ph;
GtkWidget       *g_spinbutton_pr;
GtkWidget       *g_scrolledwindow_serieFormat;
GtkWidget       *g_scrolledwindow_finalTable;
FILE            *file_tableData;

int matrixSize;

int main(int argc, char *argv[]) {
    GtkBuilder      *builder; 
    /*--- CSS -----------------*/
    GtkCssProvider  *provider;
    GdkDisplay      *display;
    GdkScreen       *screen;
    /*-------------------------*/
 
    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_series.glade", NULL);
 
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

    g_scrolledwindow_serieFormat = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_serieFormat"));
    g_scrolledwindow_finalTable = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_finalTable"));

    g_filechooser_btn = GTK_WIDGET(gtk_builder_get_object(builder, "filechooser_btn"));
    GtkFileFilter *filter = gtk_file_filter_new ();
    gtk_file_filter_add_pattern (filter, "*.cvs");
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER(g_filechooser_btn),filter);

    g_spinbutton_games = GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton_games"));
    gtk_spin_button_set_range (GTK_SPIN_BUTTON(g_spinbutton_games),1,100);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(g_spinbutton_games),1,3);

    g_spinbutton_ph = GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton_ph"));
    gtk_spin_button_set_range (GTK_SPIN_BUTTON(g_spinbutton_ph),0.00,1.00);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(g_spinbutton_ph),0.01,3);

    g_spinbutton_pr = GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton_pr"));
    gtk_spin_button_set_range (GTK_SPIN_BUTTON(g_spinbutton_pr),0.00,1.00);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(g_spinbutton_pr),0.01,3);

    g_entry_fileName = GTK_WIDGET(gtk_builder_get_object(builder, "entry_fileName"));
    g_entry_probabilidadA = GTK_WIDGET(gtk_builder_get_object(builder, "entry_probabilidadA"));
    gtk_widget_set_sensitive(g_entry_probabilidadA,FALSE);

    g_entry_probabilidadB = GTK_WIDGET(gtk_builder_get_object(builder, "entry_probabilidadB"));
    gtk_widget_set_sensitive(g_entry_probabilidadB,FALSE);
    /*---------------- CSS ------------------------------------------------*/
    provider = gtk_css_provider_new ();
    display = gdk_display_get_default ();
    screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gsize bytes_written, bytes_read;

    const gchar* home = "style/series.css";  
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
void createButtons(){
	inputFormatSerie = calloc(inputNumberGames,sizeof(GtkWidget*));

	g_tableData = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_serieFormat), g_tableData);

  char text[14];
  char number[4];
  for(int column =0; column < inputNumberGames; column++) 
  {
  	strcpy(text,"Partido  ");
  	sprintf(number, "%d", column + 1);
  	strcat(text,number);
    inputFormatSerie[column] = gtk_toggle_button_new_with_label (text);
    gtk_grid_attach (GTK_GRID (g_tableData),inputFormatSerie[column] , column, 0, 1, 1);
    memset(text,'\0',strlen(text));
  }
  gtk_widget_show_all(windowSecond);
}

void createFile(char *fileName) {
  file_tableData = fopen(fileName,"w+");
  fprintf(file_tableData,"#\n");
  fprintf(file_tableData,"%d\n",inputNumberGames);
  fprintf(file_tableData,"%%\n");
  fprintf(file_tableData,"%f\n",ph);
  fprintf(file_tableData,"%%\n");
  fprintf(file_tableData,"%f\n",pr);

  for(int column =0; column < inputNumberGames; column++) 
  {
    fprintf(file_tableData,"%d^",formatSerie[column]);
  }
  
  fclose(file_tableData);
}

void createFinalTable(double table[matrixSize+1][matrixSize+1]) {
	free(inputFormatSerie);
	int n = inputNumberGames - 1;

	tableData = calloc(n,sizeof(GtkWidget**));

  g_tableData = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_finalTable), g_tableData);

  char number[8];

  for(int j = 0; j < n; j++) {
    tableData[j] = calloc(n,sizeof(GtkWidget*));
  }

  for(int row = 0; row < n; row++) 
  {
    for(int column=0; column < n; column++) 
    {
    	tableData[row][column] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY(tableData[row][column]),10);
      gtk_widget_set_sensitive(tableData[row][column],FALSE);
      gtk_grid_attach (GTK_GRID (g_tableData),tableData[row][column] , column, row, 1, 1);

    	if (row == 0 && column == 0) {
    		gtk_entry_set_text (GTK_ENTRY(tableData[row][column])," ");
        gtk_widget_set_name(tableData[row][column],"header");
    	}
    	if (row != 0 && column == 0) {
    		sprintf(number, "%d", row - 1);
    		gtk_entry_set_text (GTK_ENTRY(tableData[row][column]), number);
        gtk_widget_set_name(tableData[row][column],"header");
    	}
    	if (row == 0 && column != 0) {
    		sprintf(number, "%d", column - 1);
    		gtk_entry_set_text (GTK_ENTRY(tableData[row][column]), number);
        gtk_widget_set_name(tableData[row][column],"header");
    	}
    	if (row != 0 && column != 0) {
        sprintf(number, "%f", table[row-1][column-1]);
    		gtk_entry_set_text (GTK_ENTRY(tableData[row][column]), number);
    	}
    }
  }
  gtk_entry_set_text (GTK_ENTRY(g_entry_probabilidadA), "0.5780");
  gtk_entry_set_text (GTK_ENTRY(g_entry_probabilidadB), "0.422");
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

void on_btn_getNumberGames_clicked() {
	inputNumberGames = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(g_spinbutton_games));
	createButtons();
  gtk_widget_hide(windowInitial);
  gtk_widget_show_now(windowSecond);
}

void on_btn_getFile_clicked(){
	setData( gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(g_filechooser_btn)) );
	createButtons();

	gtk_spin_button_set_value (GTK_SPIN_BUTTON(g_spinbutton_ph),ph);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON(g_spinbutton_pr),pr);

	for(int row =0; row < inputNumberGames; row++)
	{
		if (formatSerie[row] == 1){
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(inputFormatSerie[row]) , TRUE);
		}
		else {
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(inputFormatSerie[row]) , FALSE);
		}
	}

	gtk_widget_hide(windowInitial);
  gtk_widget_show_now(windowSecond);
}

/***************SEGUNDA PANTALLA**********************/
void on_window_secondWindow_destroy() {
	gtk_main_quit();
}

void on_btn_getData_clicked() {
	formatSerie = calloc(inputNumberGames, sizeof(int));
	ph = gtk_spin_button_get_value (GTK_SPIN_BUTTON(g_spinbutton_ph));
	pr = gtk_spin_button_get_value (GTK_SPIN_BUTTON(g_spinbutton_pr));
	qr = 1 - ph;
	qh = 1 - pr;

	for(int row =0; row < inputNumberGames; row++)
	{
		if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(inputFormatSerie[row]))){
			formatSerie[row] = 1;
		}
		else {
			formatSerie[row] = 0;
		}
	}

	int lenName = strlen(gtk_entry_get_text (GTK_ENTRY(g_entry_fileName))) + 23;
  char fileName[lenName]; 
  strcpy(fileName,"examples/Series/");
  strcat(fileName, gtk_entry_get_text (GTK_ENTRY(g_entry_fileName)));
  strcat(fileName, ".cvs");

  matrixSize = calculateMatrixSize(inputNumberGames);
  setBValues();
  double table[matrixSize+1][matrixSize+1];
  algorithm(table);
    for (int row = 0; row<=gamesPlayer;row++){
    for (int col = 0; col <=gamesPlayer;col ++){
      printf("%f   ",table[row][col]);
    }
    printf("\n");
  }

  createFile(fileName);
  createFinalTable(table);

	gtk_widget_hide(windowSecond);
  gtk_widget_show_now(windowFinal);
  
}
/***************TERCERA PANTALLA**********************/
void on_window_final_destroy() {
	gtk_main_quit();
}
