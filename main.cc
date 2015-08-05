#include <gtk/gtk.h>
#include <cstdlib>

#include "Tile.h"
#include "Tool.h"
#include "tools.h"


//how much it steps when you zoom in or out
const float ZOOM_STEP = 0.5f;
const float OUTLINE_WIDTH = 1;


//a tile that I intend to display
static Tile * gTile = new Tile(32,32);

//user's selected colours
static int drawColour = 0;

static float alphaRed = 0.25;
static float alphaGreen = 0.25;
static float alphaBlue = 0.25;

//other things that are related
static float scale = 1;
static int alpha = 1;

static float tileX = 0;
static float tileY = 0;
static float dragOriginX = 0;
static float dragOriginY = 0;


//redraws everything
static void redrawSurface(cairo_t * cr)
{
  //clear background
  cairo_set_source_rgb(cr,alphaRed,alphaGreen,alphaBlue);
  cairo_paint(cr);

  //put a border around the tile to show where it ends
  cairo_set_source_rgb (cr,0,0,0);
  cairo_rectangle(cr,tileX,tileY,gTile->width * scale,gTile->height * scale);
  cairo_set_line_width(cr,OUTLINE_WIDTH);
  cairo_stroke(cr);

  //display the tile
  gTile->render(tileX,tileY,scale,cr);
}


//called when part of the image needs to be redrawn
//cr is a drawing thing which is already clipped to the offending area
static gboolean drawEvent(GtkWidget * widget,cairo_t * cr,gpointer data)
{
  redrawSurface(cr);

  //let it continue on it's merry way <3
  return FALSE;
}


//draw on the tile at the given position
static void fillPixel(GtkWidget * widget,gdouble x,gdouble y)
{
  int drawX = (int)((x - tileX) / scale);
  int drawY = (int)((y - tileY) / scale);

  //make sure they aren't going out of the tile
  if (drawX < 0 || drawY < 0 || drawX > gTile->width || drawY > gTile->height)
  {
    return;
  }

  //TODO: call a tool  | (alpha << 24)

  //Now invalidate the affected region of the drawing area
  gtk_widget_queue_draw(widget);
}


//event called when the mouse is clicked
static gboolean buttonPressEvent(GtkWidget * widget,GdkEventButton * event,
                                 gpointer data)
{
  if (event->button == GDK_BUTTON_PRIMARY)
  {
      fillPixel (widget, event->x, event->y);
  }
  else if (event->button == GDK_BUTTON_MIDDLE)
  {
      dragOriginX = event->x - tileX;
      dragOriginY = event->y - tileY;
  }

  //dealt with
  return TRUE;
}


//when the mouse is moved
static gboolean motionNotifyEvent (GtkWidget      *widget,
                        GdkEventMotion *event,
                        gpointer        data)
{
  if (event->state & GDK_BUTTON1_MASK)
  {
    fillPixel(widget, event->x, event->y);
  }
  else if (event->state & GDK_BUTTON2_MASK)
  {
    tileX = event->x - dragOriginX;
    tileY = event->y - dragOriginY;
    gtk_widget_queue_draw(widget);
  }

  //dealt with
  return TRUE;
}


static gboolean scrollEvent(GtkWidget * widget,GdkEventScroll * event,
                            gpointer data)
{
  //do the scroll
  if (event->direction == GDK_SCROLL_UP)
  {
    scale += ZOOM_STEP;
  }

  if (event->direction == GDK_SCROLL_DOWN)
  {
    scale -= ZOOM_STEP;
  }

  //make sure it doesn't go too low
  //if it does, it like flips inside out which is mad
  if (scale <= 0)
  {
    scale = ZOOM_STEP;
  }

  //redraw the screen
  gtk_widget_queue_draw(widget);

  //dealt with
  return TRUE;
}


//when the user presses the zoom in button
static void zoomInEvent(GtkWidget * widget,gpointer data)
{
  scale += ZOOM_STEP;

  gtk_widget_queue_draw(GTK_WIDGET(data));
}


//when the user presses the zoom in button
static void zoomOutEvent(GtkWidget * widget,gpointer data)
{
  scale -= ZOOM_STEP;

  if (scale <= 0)
  {
    scale = ZOOM_STEP;
  }

  gtk_widget_queue_draw(GTK_WIDGET(data));
}


//when someone has entered a new colour
static void enterColourEvent(GtkEntry * entry,gpointer data)
{
  drawColour = (int)(strtol(gtk_entry_get_text(entry),NULL,0) & 0xFFFFFF);
}


//when someone toggles alpha drawing
static void alphaToggleEvent(GtkToggleButton * button,gpointer data)
{
  alpha = 1 - alpha;
}


//when the window is closed
static void close_window (void)
{
  gtk_main_quit ();
}


//when the start of the program turns it's head
int main (int argc,char * argv[])
{
  GtkBuilder * builder;

  GObject * window;
  GObject * drawingArea;
  GObject * drawColourEntry;
  GObject * zoomInButton;
  GObject * zoomOutButton;
  GObject * alphaButton;
  GObject * toolbox;


  gtk_init(&argc,&argv);

  //create a builder and load our description
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder,"assets/builder.ui",NULL);

  //connect signal handlers to the constructed widgets
  window = gtk_builder_get_object(builder,"window1");
  g_signal_connect(window,"destroy",G_CALLBACK(close_window),NULL);

  drawingArea = gtk_builder_get_object(builder,"drawingarea");
  g_signal_connect(drawingArea,"draw",G_CALLBACK(drawEvent),NULL);
  g_signal_connect(drawingArea,"motion-notify-event",G_CALLBACK(motionNotifyEvent),NULL);
  g_signal_connect(drawingArea,"button-press-event",G_CALLBACK(buttonPressEvent),NULL);
  g_signal_connect(drawingArea,"scroll-event",G_CALLBACK(scrollEvent),NULL);

  drawColourEntry = gtk_builder_get_object(builder,"drawColourEntry");
  g_signal_connect(drawColourEntry,"activate",G_CALLBACK(enterColourEvent),NULL);

  zoomInButton = gtk_builder_get_object(builder,"zoomInButton");
  g_signal_connect(zoomInButton,"clicked",G_CALLBACK(zoomInEvent),drawingArea);

  zoomOutButton = gtk_builder_get_object(builder,"zoomOutButton");
  g_signal_connect(zoomOutButton,"clicked",G_CALLBACK(zoomOutEvent),drawingArea);

  alphaButton = gtk_builder_get_object(builder,"alphaButton");
  g_signal_connect(alphaButton,"toggled",G_CALLBACK(alphaToggleEvent),NULL);

  //add all the tools to the toolbox
  toolbox = gtk_builder_get_object(builder,"toolbox");

  gtkButton

  gtk_box_pack_start(GTK_BOX(toolbox),);


  //add some events that the drawing area doesn't naturally get
  gtk_widget_set_events (GTK_WIDGET(drawingArea),
                         gtk_widget_get_events (GTK_WIDGET(drawingArea)) |
                         GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK |
                         GDK_SCROLL_MASK);

  gtk_widget_show_all (GTK_WIDGET(window));

  gtk_main();

  return 0;
}
