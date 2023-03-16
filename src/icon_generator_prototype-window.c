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
#include <sys/stat.h>  //for creating temp folder

//functional for search_entry and grid_search_view
GtkGrid   *grid_search_view_temp;
               //maybe possible use dynamic string of chars i mean store paths not like a array but as a long string
               //but in below... app is using last_element and not works with empty elements
gchar     *elements[1000];
gchar     *elements_temp[1000];
gint       elements_last_index = 0;
GtkWidget *elements_widgets[1000];
GtkWidget *image;
gchar     *path_library;
gint       grid_search_view_cols = 10; //also will be good calculate window size and set it dynamically
gchar     *temp;
gint       temp_size;
GFile     *temp_file;
gchar     *temp_work_elements[9];   //choosen for work elements

GtkWidget *temp_work_elements_widgets[10];
gint       temp_working_element = 0;
GtkImage  *draw_image;
GdkPixbuf *draw_gdkpixbuf;
GtkBox    *control_box;

GtkSpinButton       *spin_button_x;
GtkSpinButton       *spin_button_y;

static void
change_spin_button_x (GtkWidget *widget)
{
  g_print("change_spin_button_x!\n");
  gdouble new_x = gtk_spin_button_get_value (spin_button_x);
  gdouble old_y = gtk_spin_button_get_value (spin_button_y);
                                //!temporary
  if(temp_working_element!=0 && new_x != 0)
  {
    //search in temp id "layer[temp_working_element]"
    int i = 0;
    char layer[2];
    sprintf(layer,"%d",temp_working_element);
    while ( !((temp[i] =='l') && (temp[i+1] == 'a') && (temp[i+2] =='y') && (temp[i+3] == 'e') && (temp[i+4] == 'r') && (temp[i+5] == layer[0]))  )
    {
      i++;
    }
    i+=7;
    g_print("working layer founded.\n");
    //search transform after i ~200 symbols
    int is_transform_exist = 0; //if we will not find then not exist
    int k=0;
    for(;k<200;k++)
    {
        if((temp[i+k] =='t') && (temp[i+k+1] == 'r') && (temp[i+k+2] =='a') && (temp[i+k+3] == 'n') && (temp[i+k+4] == 's') && (temp[i+k+5] == 'f') && (temp[i+k+6] == 'o') && (temp[i+k+7] == 'r') && (temp[i+k+8] == 'm'))
        {
          is_transform_exist = 1;
          i += k + 8;
          break;
        }
    }
    g_print("transform exist: %d.\n",is_transform_exist);
    //analyze and transform
    if(is_transform_exist)
    {
      //if transform exist
      //try to find "translate"
      int is_translate_exist = 0;
      for(k = 0; k < 100; k++)
      {
        if((temp[i+k] =='t') && (temp[i+k+1] == 'r') && (temp[i+k+2] =='a') && (temp[i+k+3] == 'n') && (temp[i+k+4] == 's') && (temp[i+k+5] == 'l') && (temp[i+k+6] == 'a') && (temp[i+k+7] == 't') && (temp[i+k+8] == 'e'))
        {
          is_translate_exist = 1;
          i += k + 10;
          break;
        }
      }
      g_print("translate exist: %d.\n",is_translate_exist);
      //change
      if(is_translate_exist)
      {
         //if translate exist change
         g_print("translate is exist, started working...\n");
         //move to buffer_start from start to i
         gchar buffer_start[i+1];
         memmove(buffer_start,temp,i*sizeof(temp[0]));
         buffer_start[i] = '\0';
         //organise buffer_change
         gchar new_x_char[10];
         sprintf(new_x_char,"%f",new_x);
         gchar new_y_char[10];
         sprintf(new_y_char,"%f",old_y);
         gchar *buffer_change = g_strconcat(new_x_char," ",new_y_char,NULL);
         //move to buffer_end from i to end
         for(k=0;temp[i+k]!=')';k++){};
         gchar buffer_end[temp_size-k+1];
         memmove(buffer_end,temp+i+k*sizeof(temp[0]),temp_size-k+1);
         buffer_end[temp_size-k] = '\0';
         //concat them in temp
         gchar *temp_buffer = g_strconcat(buffer_start,buffer_change,buffer_end,NULL);
         temp_size = g_utf8_strlen (temp_buffer,-1);
         g_strlcpy (temp,temp_buffer,temp_size+sizeof(temp[0]));
         g_print("finished working.\n");
      }
      else
      {
         //if translate not exist add
         //!temporary empty need other changes for testing
      }
    }
    else
    {
      //if transform not-exist -> create
      g_print("transform not exist, started working...\n");
      //move to buffer_start from start to i
      gchar buffer_start[i+1];
      memmove(buffer_start,temp,i*sizeof(temp[0]));
      buffer_start[i] = '\0';
      //organise buffer_change
      gchar new_x_char[10];
      sprintf(new_x_char,"%f",new_x);
      gchar new_y_char[10];
      sprintf(new_y_char,"%f",old_y);
      gchar *buffer_change = g_strconcat("\ntransform=\"translate(",new_x_char," ",new_y_char,")\"",NULL);
      //move to buffer_end from i to end
      int j = temp_size -i;
      gchar buffer_end[j+1];
      memmove(buffer_end,temp+i*sizeof(temp[0]),j);
      buffer_end[j] = '\0';
      //concat them in temp
      gchar *temp_buffer = g_strconcat(buffer_start,buffer_change,buffer_end,NULL);
      temp_size = g_utf8_strlen (temp_buffer,-1);
      g_strlcpy (temp,temp_buffer,temp_size+sizeof(temp[0]));
      g_print("finished working.\n");
    }
    temp_size+=sizeof(temp[0]);
    //rewrite & redraw
    GFileIOStream *gfiostream = g_file_open_readwrite(temp_file,NULL,NULL);
    GOutputStream *gostream = g_io_stream_get_output_stream (gfiostream);
    g_output_stream_write (gostream, temp, temp_size, NULL, NULL);
    gtk_image_set_from_file (draw_image, g_strconcat(path_library,"/output/temp.svg",NULL));
  }
}

static void
change_spin_button_y (GtkWidget *widget)
{
  g_print("change_spin_button_y!\n");
  gdouble new_y = gtk_spin_button_get_value (spin_button_y);
  gdouble old_x = gtk_spin_button_get_value (spin_button_x);
                              //!temporary
  if(temp_working_element!=0 && new_y != 0)
  {
    //search in temp id "layer[temp_working_element]"
    int i = 0;
    char layer[2];
    sprintf(layer,"%d",temp_working_element);
    while ( !((temp[i] =='l') && (temp[i+1] == 'a') && (temp[i+2] =='y') && (temp[i+3] == 'e') && (temp[i+4] == 'r') && (temp[i+5] == layer[0]))  )
    {
      i++;
    }
    i+=7;
    g_print("working layer founded.\n");
    //search transform after i ~200 symbols
    int is_transform_exist = 0; //if we will not find then not exist
    int k=0;
    for(;k<200;k++)
    {
        if((temp[i+k] =='t') && (temp[i+k+1] == 'r') && (temp[i+k+2] =='a') && (temp[i+k+3] == 'n') && (temp[i+k+4] == 's') && (temp[i+k+5] == 'f') && (temp[i+k+6] == 'o') && (temp[i+k+7] == 'r') && (temp[i+k+8] == 'm'))
        {
          is_transform_exist = 1;
          i += k + 8;
          break;
        }
    }
    g_print("transform exist: %d.\n",is_transform_exist);
    //analyze and transform
    if(is_transform_exist)
    {
      //if transform exist
      //try to find "translate"
      int is_translate_exist = 0;
      for(k = 0; k < 100; k++)
      {
        if((temp[i+k] =='t') && (temp[i+k+1] == 'r') && (temp[i+k+2] =='a') && (temp[i+k+3] == 'n') && (temp[i+k+4] == 's') && (temp[i+k+5] == 'l') && (temp[i+k+6] == 'a') && (temp[i+k+7] == 't') && (temp[i+k+8] == 'e'))
        {
          is_translate_exist = 1;
          i += k + 10;
          break;
        }
      }
      g_print("translate exist: %d.\n",is_translate_exist);
      //change
      if(is_translate_exist)
      {
         //if translate exist change
         g_print("translate is exist, started working...\n");
         //move to buffer_start from start to i
         gchar buffer_start[i+1];
         memmove(buffer_start,temp,i*sizeof(temp[0]));
         buffer_start[i] = '\0';
         //organise buffer_change
         gchar new_x_char[10];
         sprintf(new_x_char,"%f",old_x);
         gchar new_y_char[10];
         sprintf(new_y_char,"%f",new_y);
         gchar *buffer_change = g_strconcat(new_x_char," ",new_y_char,NULL);
         //move to buffer_end from i to end
         for(k=0;temp[i+k]!=')';k++){};
         gchar buffer_end[temp_size-k+1];
         memmove(buffer_end,temp+i+k*sizeof(temp[0]),temp_size-k+1);
         buffer_end[temp_size-k] = '\0';
         //concat them in temp
         gchar *temp_buffer = g_strconcat(buffer_start,buffer_change,buffer_end,NULL);
         temp_size = g_utf8_strlen (temp_buffer,-1);
         g_strlcpy (temp,temp_buffer,temp_size+sizeof(temp[0]));
         g_print("finished working.\n");
      }
      else
      {
         //if translate not exist add
         //!temporary empty need other changes for testing
      }
    }
    else
    {
      //if transform not-exist -> create
      g_print("transform not exist, started working...\n");
      //move to buffer_start from start to i
      gchar buffer_start[i+1];
      memmove(buffer_start,temp,i*sizeof(temp[0]));
      buffer_start[i] = '\0';
      //organise buffer_change
      gchar new_x_char[10];
      sprintf(new_x_char,"%f",old_x);
      gchar new_y_char[10];
      sprintf(new_y_char,"%f",new_y);
      gchar *buffer_change = g_strconcat("\ntransform=\"translate(",new_x_char," ",new_y_char,")\"",NULL);
      //move to buffer_end from i to end
      int j = temp_size -i;
      gchar buffer_end[j+1];
      memmove(buffer_end,temp+i*sizeof(temp[0]),j);
      buffer_end[j] = '\0';
      //concat them in temp
      gchar *temp_buffer = g_strconcat(buffer_start,buffer_change,buffer_end,NULL);
      temp_size = g_utf8_strlen (temp_buffer,-1);
      g_strlcpy (temp,temp_buffer,temp_size+sizeof(temp[0]));
      g_print("finished working.\n");
    }
    temp_size+=sizeof(temp[0]);
    //rewrite & redraw
    GFileIOStream *gfiostream = g_file_open_readwrite(temp_file,NULL,NULL);
    GOutputStream *gostream = g_io_stream_get_output_stream (gfiostream);
    g_output_stream_write (gostream, temp, temp_size, NULL, NULL);
    gtk_image_set_from_file (draw_image, g_strconcat(path_library,"/output/temp.svg",NULL));
  }

}
static void
change_scale_rotate (GtkWidget *widget, GtkAdjustment *data)
{
  gint t = gtk_adjustment_get_value(data);
  g_print("%d\n",t);
}
static void
change_scale_scale (GtkWidget *widget, GtkAdjustment *data)
{
  gint t = gtk_adjustment_get_value(data);
  g_print("%d\n",t);
}
static void
click_button_grid (GtkWidget *widget)
{
  g_print("#\n");
}

static void
work_element_clicked (GtkWidget *widget, gint data)
{
  temp_working_element = data;
  //!temporary
  //reset control
  gtk_spin_button_set_value(spin_button_x,0);
  gtk_spin_button_set_value(spin_button_y,0);
  //!
  //analyze and set current values:
}

static void
element_clicked (GtkWidget *widget, gchar *data)
{
  widget = widget;     //temporary

  int i = 0;
  while(g_strcmp0 (temp_work_elements[i],"0"))
  {
    i++;
  }
  //if in temp_work_elements enough space
  if(i<(sizeof(temp_work_elements)/sizeof(temp_work_elements[i])-1))
  {
    //add to temp_work_element
    temp_work_elements[i] = data;
    //add to work_element to control_box
    temp_work_elements_widgets[i] = gtk_button_new ();
    gtk_button_set_label (GTK_BUTTON(temp_work_elements_widgets[i]),data);
                                                                                                //real id of layer(temp_work_element) in workint temp_file
                                                                                                //see below
    g_signal_connect(temp_work_elements_widgets[i], "clicked", G_CALLBACK (work_element_clicked), i+2);
    //add to control_box
    gtk_box_append (control_box, temp_work_elements_widgets[i]);
    //add to draw
    //
    //1)copy to temp_buffer search "<g" in temp_buffer and "</g>"
    GFile *temp_buffer_file = g_file_new_for_path (data);
    GFileInputStream *temp_buffer_istream = g_file_read (temp_buffer_file, NULL, NULL);
    gchar temp_buffer[10000];
    g_input_stream_read (temp_buffer_istream, temp_buffer, 10000, NULL, NULL);
    //g_input_stream_close (temp_buffer_istream,NULL,NULL);

    //ye it hardcoded, but for inkscape must work for simple elements, if app continue growth
    //it must be changet to something like analyze text, analyze tool,layers,etc
    //
    //temp - when i > 0 must be something like a find <g and repeat find <g i loops
    int j = 0;
           //search "<g"
    while ( !((temp_buffer[j] =='<') && (temp_buffer[j+1] == 'g')) )
    {
      j++;
    }
    int k = j;
           //search "</g>"
    while ( !((temp_buffer[k] =='<') && (temp_buffer[k+1] == '/') && (temp_buffer[k+2] =='g') && (temp_buffer[k+3] == '>') ) )
    {
      k++;
    }
    k+=3;//move to ">" last symbol
    //reduce temp_buffer
    int l;
    for( l = 0; l <= k - j; l++)
    {
      temp_buffer[l] = temp_buffer[j+l];
    }
    temp_buffer[l] = '\0';
    //2)find "Layer n" in temp_buffer and change to "Layer (position in temp_work_elements+1)"
    k = 0;
    while ( !((temp_buffer[k] =='L') && (temp_buffer[k+1] == 'a') && (temp_buffer[k+2] =='y') && (temp_buffer[k+3] == 'e') && (temp_buffer[k+4] == 'r') ) )
    {
      k++;
    }
    char layer[2];
    sprintf(layer,"%d",i+2);
    temp_buffer[k+6] = layer[0];
    k = 0;
    while ( !((temp_buffer[k] =='i') && (temp_buffer[k+1] == 'd') && (temp_buffer[k+2] =='=') ) )
    {
      k++;
    }
    temp_buffer[k+9] = layer[0];

    //3)search last "</g>" in temp and paste to temp + add </svg>
    //read temp again
    k = 0;
    for(int l = i; l >= 0; l--)
    {
      //search "</g>"
      while ( !((temp[k] =='<') && (temp[k+1] == '/') && (temp[k+2] =='g') && (temp[k+3] == '>') ) )
      {
        k++;
      }
      k++;
    }
    k+=3;//move to ">" last symbol
    temp[k+1]='\0';
    temp = g_strconcat(temp,"\n   ",temp_buffer,"\n</svg>",NULL);

     //call rewrite and redraw
     temp_size = g_utf8_strlen (temp,-1);

     GFileIOStream *gfiostream = g_file_open_readwrite(temp_file,NULL,NULL);
     GOutputStream *gostream = g_io_stream_get_output_stream (gfiostream);
     g_output_stream_write (gostream, temp, temp_size, NULL, NULL);
     gtk_image_set_from_file (draw_image, g_strconcat(path_library,"/output/temp.svg",NULL));
  }
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
  GtkImage            *draw_image;
  GtkBox              *control_box;
  GtkSpinButton       *spin_button_x;
  GtkSpinButton       *spin_button_y;
  GtkAdjustment       *spinx;
  GtkAdjustment       *spiny;
  GtkScale            *scale_rotate;
  GtkScale            *scale_scale;
  GtkAdjustment       *scalerotate;
  GtkAdjustment       *scalescale;
  GtkButton           *button_grid;
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
  gtk_widget_class_bind_template_child    (widget_class, IconGeneratorPrototypeWindow, draw_image);
  gtk_widget_class_bind_template_child    (widget_class, IconGeneratorPrototypeWindow, control_box);
  gtk_widget_class_bind_template_child    (widget_class, IconGeneratorPrototypeWindow, spin_button_x);
  gtk_widget_class_bind_template_child    (widget_class, IconGeneratorPrototypeWindow, spin_button_y);
  gtk_widget_class_bind_template_child    (widget_class, IconGeneratorPrototypeWindow, spinx);
  gtk_widget_class_bind_template_child    (widget_class, IconGeneratorPrototypeWindow, spiny);
  gtk_widget_class_bind_template_child    (widget_class, IconGeneratorPrototypeWindow, scale_scale);
  gtk_widget_class_bind_template_child    (widget_class, IconGeneratorPrototypeWindow, scale_rotate);
  gtk_widget_class_bind_template_child    (widget_class, IconGeneratorPrototypeWindow, scalescale);
  gtk_widget_class_bind_template_child    (widget_class, IconGeneratorPrototypeWindow, scalerotate);
  gtk_widget_class_bind_template_child    (widget_class, IconGeneratorPrototypeWindow, button_grid);
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
  GdkPixbuf* pixbuf_temp;
  //place 0 element(for set direction and start placement for others)
  image = gtk_image_new_from_resource ("/icon/generator/prototype/0.svg");
  elements_widgets[0] = gtk_button_new ();
  gtk_button_set_child (GTK_BUTTON (elements_widgets[0]), image);
  g_signal_connect(elements_widgets[0], "clicked", G_CALLBACK (element_clicked), "0");
  gtk_widget_set_size_request (elements_widgets[0],100,100);
  gtk_grid_attach ( self->grid_search_view,elements_widgets[0],0,1,100,100);
  //place others
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
        gtk_grid_attach_next_to (self->grid_search_view,elements_widgets[i],elements_widgets[i-grid_search_view_cols],GTK_POS_BOTTOM,100,100);
      } else
      {
        gtk_grid_attach_next_to (self->grid_search_view,elements_widgets[i],elements_widgets[i-1],GTK_POS_RIGHT,100,100);
      }
    gtk_widget_set_size_request (elements_widgets[i],100,100);
    gtk_widget_set_visible (elements_widgets[i], TRUE);
  }

  //store temp element in /tmp(for working with svg content like a with text file)
  //create
  temp = g_strconcat("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n",
"<!-- Created with Inkscape (http://www.inkscape.org/) -->\n",
"<svg\n",
"   width=\"128\"\n",
"   height=\"128\"\n",
"   viewBox=\"0 0 33.866666 33.866666\"\n",
"   version=\"1.1\"\n",
"   id=\"svg5\"\n",
"   inkscape:version=\"1.2.2 (732a01da63, 2022-12-09, custom)\"\n",
"   sodipodi:docname=\"0.svg\"\n",
"   xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\"\n",
"   xmlns:sodipodi=\"http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd\"\n",
"   xmlns=\"http://www.w3.org/2000/svg\"\n",
"   xmlns:svg=\"http://www.w3.org/2000/svg\">\n",
"  <sodipodi:namedview\n",
"     id=\"namedview7\"\n",
"     pagecolor=\"#ffffff\"\n",
"     bordercolor=\"#000000\"\n",
"     borderopacity=\"0.25\"\n",
"     inkscape:showpageshadow=\"2\"\n",
"     inkscape:pageopacity=\"0.0\"\n",
"     inkscape:pagecheckerboard=\"0\"\n",
"     inkscape:deskcolor=\"#d1d1d1\"\n",
"     inkscape:document-units=\"mm\"\n",
"     showgrid=\"false\"\n",
"     inkscape:zoom=\"0.70199214\"\n",
"     inkscape:cx=\"397.44035\"\n",
"     inkscape:cy=\"561.9721\"\n",
"     inkscape:window-width=\"1920\"\n",
"     inkscape:window-height=\"1016\"\n",
"     inkscape:window-x=\"0\"\n",
"     inkscape:window-y=\"0\"\n",
"     inkscape:window-maximized=\"1\"\n",
"     inkscape:current-layer=\"layer1\" />\n",
"  <defs\n",
"     id=\"defs2\" />\n",
"  <g\n",
"     inkscape:label=\"Layer 1\"\n",
"     inkscape:groupmode=\"layer\"\n",
"     id=\"layer1\">\n",
"    <text\n",
"       xml:space=\"preserve\"\n",
"       transform=\"matrix(0.26458333,0,0,0.26458333,5.5401104,4.8437908)\"\n",
"       id=\"text234\"\n",
"       style=\"font-size:64px;white-space:pre;shape-inside:url(#rect236);display:inline;fill:#000000;stroke-width:3.77953\"><tspan\n",
"         x=\"32.619141\"\n",
"         y=\"82.716797\"\n",
"         id=\"tspan367\">1</tspan></text>\n",
"   </g> \n",
"</svg>\n",NULL);

  temp_size = g_utf8_strlen (temp,-1);

  draw_image = self->draw_image;
  //store
  mkdir(g_strconcat(path_library,"/output",NULL), 0777);
  temp_file = g_file_new_for_path (g_strconcat(path_library,"/output/temp.svg",NULL));

  GFileOutputStream *gfostream = g_file_create (temp_file, G_FILE_CREATE_NONE, NULL, NULL);
  //GFileInputStream  *gfistream = g_file_read (temp_file, NULL, NULL);
  g_output_stream_write (gfostream, temp, temp_size, NULL, NULL);
  gtk_image_set_from_file (self->draw_image, g_strconcat(path_library,"/output/temp.svg",NULL));
  //g_input_stream_read (gfistream, temp, temp_size, NULL, NULL);

  //fill temp_work_elements with "0"
  for(int i = 0; i < 10; i++)
  {
    temp_work_elements[i] = "0";
  }

  control_box = self->control_box;


  //set handlers for control
  g_signal_connect (self->spin_button_x, "value-changed", G_CALLBACK (change_spin_button_x), NULL);
  g_signal_connect (self->spin_button_y, "value-changed", G_CALLBACK (change_spin_button_y), NULL);
  g_signal_connect (self->scale_rotate , "value-changed", G_CALLBACK (change_scale_rotate) , self->scalerotate);
  g_signal_connect (self->scale_scale  , "value-changed", G_CALLBACK (change_scale_scale)  , self->scalescale);
  g_signal_connect (self->button_grid  , "clicked", G_CALLBACK (click_button_grid), NULL);

  spin_button_x = self->spin_button_x;
  spin_button_y = self->spin_button_y;
}
