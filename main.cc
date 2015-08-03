#include <gtk/gtk.h>
#include <cstdlib>

#include "Tile.h"


//how much it steps when you zoom in or out
const float ZOOM_STEP = 0.5f;


//a tile that I intend to display
static Tile * gTile = new Tile(32,32);

//user's selected colours
static int drawColour = 0;

static float alphaRed = 0.25;
static float alphaGreen = 0.25;
static float alphaBlue = 0.25;

//other things that are related
static float scale = 1;


//redraws everything
static void redrawSurface(cairo_t * cr)
{
  cairo_set_source_rgb (cr,alphaRed,alphaGreen,alphaBlue);
  cairo_paint (cr);
  gTile->render(0,0,scale,cr);
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
  int drawX = (int)(x / scale);
  int drawY = (int)(y / scale);

  //make sure they aren't going out of the tile
  if (drawX < 0 || drawY < 0 || drawX > gTile->width || drawY > gTile->height)
  {
    return;
  }

  gTile->setPixel(drawX,drawY,drawColour);

  //Now invalidate the affected region of the drawing area
  gtk_widget_queue_draw(widget);
}


//when the mouse is clicked
static gboolean buttonPressEvent (GtkWidget      *widget,
                       GdkEventButton *event,
                       gpointer        data)
{
  if (event->button == GDK_BUTTON_PRIMARY)
    {
      fillPixel (widget, event->x, event->y);
    }
  else if (event->button == GDK_BUTTON_SECONDARY)
    {
      gtk_widget_queue_draw (widget);
    }

  /* We've handled the event, stop processing */
  return TRUE;
}


//when the mouse is moved
static gboolean motionNotifyEvent (GtkWidget      *widget,
                        GdkEventMotion *event,
                        gpointer        data)
{
  if (event->state & GDK_BUTTON1_MASK)
    fillPixel(widget, event->x, event->y);

  /* We've handled it, stop processing */
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
  g_print("colour is %X\n",drawColour);
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

  drawColourEntry = gtk_builder_get_object(builder,"drawColourEntry");
  g_signal_connect(drawColourEntry,"activate",G_CALLBACK(enterColourEvent),NULL);

  zoomInButton = gtk_builder_get_object(builder,"zoomInButton");
  g_signal_connect(zoomInButton,"clicked",G_CALLBACK(zoomInEvent),drawingArea);

  zoomOutButton = gtk_builder_get_object(builder,"zoomOutButton");
  g_signal_connect(zoomOutButton,"clicked",G_CALLBACK(zoomOutEvent),drawingArea);

  //add all the tools to the toolbox
  toolbox = gtk_builder_get_object(builder,"toolbox");


  /* Ask to receive events the drawing area doesn't normally
   * subscribe to. In particular, we need to ask for the
   * button press and motion notify events that want to handle.
   */
  gtk_widget_set_events (GTK_WIDGET(drawingArea), gtk_widget_get_events (GTK_WIDGET(drawingArea))
                                     | GDK_BUTTON_PRESS_MASK
                                     | GDK_POINTER_MOTION_MASK);

  gtk_widget_show_all (GTK_WIDGET(window));

  gtk_main();

  return 0;
}
