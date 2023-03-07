/* icon_generator_prototype-window.c
 *
 * Copyright 2023 nik0
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "icon_generator_prototype-config.h"
#include "icon_generator_prototype-window.h"

//functional for search_entry and grid_search_view
GtkGrid             *grid_search_view_temp;
               //maybe possible use dynamic string of chars i mean store paths not like a array but as a long string
               //but in below... app is using last_element and not works with empty elements
char *elements[1000];
char *elements_temp[1000];
gint  elements_last_index = 0;
GtkWidget *elements_widgets[1000];
GtkWidget *image;
gchar *path_library;
gint grid_search_view_cols = 10; //also will be good calculate window size and set it dynamically

static void
element_clicked (GtkWidget *widget, gchar *data)
{
  g_print("%s\n",data);
  widget = widget;     //temporary
}

static void
search_text_changed (GtkSearchEntry *entry)
{
  GtkGrid *grid_search_view = grid_search_view_temp;

  GRegex *regex;
  GMatchInfo *match_info;
  regex = g_regex_new (gtk_editable_get_text(GTK_EDITABLE(entry)), G_REGEX_DEFAULT, G_REGEX_MATCH_DEFAULT, NULL);
  GdkPixbuf* pixbuf_temp;
  //!!!bug when searching - strange positions of some elements
  int t = 1;
  for (int i = 1; i < elements_last_index; i++)
  {
      //delete all widgets in element_widgets and in grid_search_view(exclude 0)
      if (GTK_IS_WIDGET(elements_widgets[i]))
      {
        gtk_widget_unrealize (elements_widgets[i]);
        gtk_grid_remove (grid_search_view,elements_widgets[i]);
      }
      elements_widgets[i] = NULL;
      //filter elements and add to grid_search_view
      g_regex_match (regex, elements[i], 0, &match_info);
      if(g_match_info_matches (match_info))
      {
          image = gtk_image_new();
          pixbuf_temp = gdk_pixbuf_new_from_file_at_scale ( elements[i], 100, 100, TRUE, NULL );
          image = gtk_image_new_from_pixbuf ( pixbuf_temp );
          elements_widgets[t] = gtk_button_new ();
          gtk_button_set_child (GTK_BUTTON (elements_widgets[t]), image);
          g_signal_connect(elements_widgets[t], "clicked", G_CALLBACK (element_clicked), elements[t]);
          gtk_widget_set_size_request (elements_widgets[t],100,100);
          gtk_widget_set_visible (elements_widgets[t], TRUE);

          if( (t % grid_search_view_cols) == 0)
          {
            gtk_grid_attach_next_to (grid_search_view,elements_widgets[t],elements_widgets[t-grid_search_view_cols],GTK_POS_BOTTOM,100,100);
          } else
          {
            gtk_grid_attach_next_to (grid_search_view,elements_widgets[t],elements_widgets[t-1],GTK_POS_RIGHT,100,100);
          }
          t++;
      }
  }
}


struct _IconGeneratorPrototypeWindow
{
  GtkApplicationWindow  parent_instance;

  /* Template widgets */
  GtkSearchEntry      *search_entry;
  GtkGrid             *grid_search_view;

};

G_DEFINE_TYPE (IconGeneratorPrototypeWindow, icon_generator_prototype_window, GTK_TYPE_APPLICATION_WINDOW)

static void
icon_generator_prototype_window_class_init (IconGeneratorPrototypeWindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/icon/generator/prototype/icon_generator_prototype-window.ui");
  gtk_widget_class_bind_template_child    (widget_class, IconGeneratorPrototypeWindow, search_entry);
  gtk_widget_class_bind_template_callback (widget_class, search_text_changed);
  gtk_widget_class_bind_template_child    (widget_class, IconGeneratorPrototypeWindow, grid_search_view);
}

static void
icon_generator_prototype_window_init (IconGeneratorPrototypeWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));



  //first load grid_search_view
  grid_search_view_temp = self->grid_search_view;

  //!first show all elements
  //build path
  const gchar *username = g_get_user_name();
  gchar *path_home = "home";
                        //must will be localized
  gchar *path_pictures = "Pictures";
  gchar *path_symbol = "/";     // /home/username/Pictures/
         path_library = g_strconcat(path_symbol,path_home,path_symbol,username,path_symbol,path_pictures,path_symbol, NULL);

  //build dir
  GDir *dir;
  dir = g_dir_open(path_library,0,NULL);
  //read dir
  const gchar *filename;
  GRegex *regex;
  GMatchInfo *match_info;
  regex = g_regex_new ("svg", G_REGEX_DEFAULT, G_REGEX_MATCH_DEFAULT, NULL);
  while ((filename = g_dir_read_name(dir)))
  {
   //check type of file
   g_regex_match (regex, filename, 0, &match_info);
   if(g_match_info_matches (match_info))
   {
     //store in elements
     elements[elements_last_index] = g_strconcat(path_library,filename,NULL);
     elements_last_index++;
   }
  }

  //show in grid_search_view
  gtk_grid_insert_row (self->grid_search_view,1);
  int direction = 1;
  GdkPixbuf* pixbuf_temp;
  //place 0 element(for set direction and start placement for others)
  image = gtk_image_new_from_resource ("/icon/generator/prototype/0.svg");
  elements_widgets[0] = gtk_button_new ();
  gtk_button_set_child (GTK_BUTTON (elements_widgets[0]), image);
  g_signal_connect(elements_widgets[0], "clicked", G_CALLBACK (element_clicked), "0");
  gtk_widget_set_size_request (elements_widgets[0],100,100);
  gtk_grid_attach ( self->grid_search_view,elements_widgets[0],0,1,100,100);
  //place others
  int j = 0;
  for(int i = 1; i < elements_last_index; i++)
  {
      image = gtk_image_new();
      pixbuf_temp = gdk_pixbuf_new_from_file_at_scale ( elements[i], 100, 100, TRUE, NULL );
      image = gtk_image_new_from_pixbuf ( pixbuf_temp );
      elements_widgets[i] = gtk_button_new ();
      gtk_button_set_child (GTK_BUTTON (elements_widgets[i]), image);
      g_signal_connect(elements_widgets[i], "clicked", G_CALLBACK (element_clicked), elements[i]);
      if(i % grid_search_view_cols==0)
      {
        j = i - grid_search_view_cols;
        gtk_grid_attach_next_to (self->grid_search_view,elements_widgets[i],elements_widgets[j],GTK_POS_BOTTOM,100,100);
      } else
      {
        gtk_grid_attach_next_to (self->grid_search_view,elements_widgets[i],elements_widgets[i-1],GTK_POS_RIGHT,100,100);
      }

    gtk_widget_set_size_request (elements_widgets[i],100,100);
    gtk_widget_set_visible (elements_widgets[i], TRUE);
  }
}
