#include <gtk/gtk.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>



static void action_textBuffer_changed(GtkTextBuffer* buffer, gpointer data)
{
    GtkTextIter* start;
    GtkTextIter* end;
    gtk_text_buffer_get_start_iter (buffer, start);
    gtk_text_buffer_get_end_iter (buffer, end);
    std::string modified_text ( gtk_text_buffer_get_text (buffer,
                                                          start,
                                                          end,
                                                          false)
                                );
    std::cout << modified_text << std::endl;
}




std::string getText(const char* filename)
{
    std::ifstream ifs;
    ifs.open(filename);
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    ifs.close();
    return buffer.str();
};

void putText(const char* filename, std::string text)
{
    std::ofstream ofs;
    ofs.open(filename);
    ofs << text;
    ofs.close();
};



static void action_editWindowButton_cancel_clicked(GtkWidget* widget, 
                                                   gpointer pw)
{
    //destroy parent window
    GtkWidget* parent_window = GTK_WIDGET (pw);
    gtk_widget_destroy(parent_window);
};

static void action_editWindowButton_save_clicked(GtkWidget* widget, 
                                                 gpointer pw)
{
    //destroy parent window
    GtkWidget* parent_window = GTK_WIDGET (pw);
    gtk_widget_destroy(parent_window);    
};


void open_file(char* filename)
{
  //This function opens the file for editing
  
  GtkWidget* editWindow;
  GtkWidget* button_box;
  GtkWidget* button_cancel;
  GtkWidget* button_save;
  GtkWidget* grid;
  GtkWidget* text_box;
  GtkTextBuffer* text_buffer;

  
  editWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (editWindow), 400, 400);
  gtk_window_set_title (GTK_WINDOW (editWindow), filename);
  gtk_window_set_modal (GTK_WINDOW (editWindow), true);
  
  grid = gtk_grid_new();

  text_buffer = gtk_text_buffer_new (NULL);
  //put text in text buffer
  std::string text = getText(filename);
  gtk_text_buffer_set_text (text_buffer, text.c_str(), text.length());
  g_signal_connect(text_buffer, "changed",
                   G_CALLBACK (action_textBuffer_changed),
                   NULL);

  text_box = gtk_text_view_new();
  gtk_text_view_set_buffer (GTK_TEXT_VIEW (text_box), text_buffer);

  gtk_grid_attach (GTK_GRID (grid), text_box, 0, 0, 1, 1);
  
  
  button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
  
  button_cancel = gtk_button_new_with_label("Cancel");
  gtk_container_add(GTK_CONTAINER (button_box), button_cancel);
  g_signal_connect (button_cancel, "clicked", 
                    G_CALLBACK (action_editWindowButton_cancel_clicked), 
                    (gpointer*) editWindow);
  
  button_save = gtk_button_new_with_label("Save");
  gtk_container_add(GTK_CONTAINER (button_box), button_save);
  

  g_signal_connect(button_save, "clicked",
                   G_CALLBACK (action_editWindowButton_save_clicked), 
                   (gpointer*) editWindow);
  
  
  gtk_grid_attach (GTK_GRID (grid), button_box, 0, 1, 1, 1);
  
  gtk_container_add(GTK_CONTAINER (editWindow), grid);
  
  gtk_widget_show_all (editWindow);
  
};



static void action_button_open_clicked(GtkWidget* widget, 
                                       gpointer pw)
{
    //Open Button Clicked Action
    
    GtkWindow* parent_window = GTK_WINDOW (pw);
    char* open_file_name;

    GtkWidget* dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    const gchar *first_button_text = "Cancel";
    const gchar *second_button_text = "Select";
    
    gint res;

    dialog = gtk_file_chooser_dialog_new("Open File",
                                         parent_window,
                                         action,
                                         first_button_text,
                                         GTK_RESPONSE_CANCEL,
                                         second_button_text,
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if(res == GTK_RESPONSE_ACCEPT)
        {
            GtkFileChooser* chooser = GTK_FILE_CHOOSER(dialog);
            open_file_name = gtk_file_chooser_get_filename(chooser);
            open_file(open_file_name);
        }

    gtk_widget_destroy(dialog);
};

static void action_button_create_clicked(GtkWidget* widget, 
                                         gpointer data)
{
    //Create Button Clicked Action
};


static void activate (GtkApplication* app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *button_box;
  GtkWidget *button_open;
  GtkWidget *button_create;
  GtkWidget *button_exit;
  

  window = gtk_application_window_new (app);
  gtk_window_set_title ( (GtkWindow*) window, "License Manager");
  gtk_window_set_default_size (GTK_WINDOW (window), 400, 400);
  
  button_box = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
  gtk_container_add(GTK_CONTAINER (window), button_box);
  
  button_open = gtk_button_new_with_label("Open License File");
  gtk_container_add(GTK_CONTAINER (button_box), button_open);
  g_signal_connect (button_open, "clicked", 
                    G_CALLBACK (action_button_open_clicked), (gpointer*)window);
  
  button_create = gtk_button_new_with_label("Create New License");
  gtk_container_add(GTK_CONTAINER (button_box), button_create);
  g_signal_connect(button_create, "clicked",
                   G_CALLBACK (action_button_create_clicked), NULL);
                    
  button_exit = gtk_button_new_with_label("Exit");
  gtk_container_add(GTK_CONTAINER (button_box), button_exit);
  g_signal_connect_swapped(button_exit, "clicked",
                          G_CALLBACK (gtk_widget_destroy), window);
                    
  gtk_widget_show_all (window);
};


int main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("com.mdlab-software.LicenseManager", 
                             G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}