#include <gtk/gtk.h>

#include "Tile.h"


//Surface to store current scribbles
static cairo_surface_t * surface = NULL;


//a tile that I intend to display
static Tile * gTile = new Tile(32,32);

//user's selected colours
static float drawRed = 0;
static float drawGreen = 0;
static float drawBlue = 0;
static bool drawAlpha = FALSE;

static float alphaRed = 0.5;
static float alphaGreen = 0.5;
static float alphaBlue = 0.5;

//other things that are related
static float scale = 1;


//draws that which is meant to be on the screen
static void clearSurface ()
{
  cairo_t * cr;

  cr = cairo_create(surface);

  cairo_set_source_rgb (cr,0.5,0.5,0.5);
  cairo_paint (cr);

  gTile->render(0,0,1,cr);

  cairo_destroy (cr);
}


//creates the surface that is drawn to
static gboolean configure_event_cb(GtkWidget * widget,
                                    GdkEventConfigure * event,gpointer data)
{
  if (surface)
  {
    cairo_surface_destroy(surface);
  }

  surface = gdk_window_create_similar_surface(gtk_widget_get_window(widget),
                                              CAIRO_CONTENT_COLOR,
                                              gtk_widget_get_allocated_width(widget),
                                              gtk_widget_get_allocated_height(widget));

  // Initialize the surface
  clearSurface ();

  //dealt with
  return TRUE;
}

//called when part of the image needs to be redrawn
//cr is a drawing thing which is already clipped to the offending area
static gboolean draw_cb(GtkWidget * widget,cairo_t * cr,gpointer data)
{
  clearSurface();
  gTile->render(0,0,1,cr);

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



  gTile->setPixel(drawX,drawY,0x00FFFF);//TODO: use a real colour

  /* Now invalidate the affected region of the drawing area. */
  gtk_widget_queue_draw_area (widget,drawX,drawY,drawX + scale,drawY + scale);
}

/* Handle button press events by either drawing a rectangle
 * or clearing the surface, depending on which button was pressed.
 * The ::button-press signal handler receives a GdkEventButton
 * struct which contains this information.
 */
static gboolean button_press_event_cb (GtkWidget      *widget,
                       GdkEventButton *event,
                       gpointer        data)
{
  /* paranoia check, in case we haven't gotten a configure event */
  if (surface == NULL)
    return FALSE;

  if (event->button == GDK_BUTTON_PRIMARY)
    {
      fillPixel (widget, event->x, event->y);
    }
  else if (event->button == GDK_BUTTON_SECONDARY)
    {
      clearSurface ();
      gtk_widget_queue_draw (widget);
    }

  /* We've handled the event, stop processing */
  return TRUE;
}

/* Handle motion events by continuing to draw if button 1 is
 * still held down. The ::motion-notify signal handler receives
 * a GdkEventMotion struct which contains this information.
 */
static gboolean motion_notify_event_cb (GtkWidget      *widget,
                        GdkEventMotion *event,
                        gpointer        data)
{
  /* paranoia check, in case we haven't gotten a configure event */
  if (surface == NULL)
    return FALSE;

  if (event->state & GDK_BUTTON1_MASK)
    fillPixel(widget, event->x, event->y);

  /* We've handled it, stop processing */
  return TRUE;
}


static void close_window (void)
{
  if (surface)
    cairo_surface_destroy (surface);

  gtk_main_quit ();
}


void printHello(GtkWidget * widget,gpointer data)
{
  g_print("ayy\n");
}


int main (int argc,char * argv[])
{
  GtkBuilder * builder;
  GObject * window;
  GObject * button;
  GObject * drawingArea;

  gtk_init(&argc,&argv);

  //create a builder and load our description
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder,"assets/builder.ui",NULL);

  //connect signal handlers to the constructed widgets
  window = gtk_builder_get_object(builder,"window1");
  g_signal_connect(window,"destroy",G_CALLBACK(close_window),NULL);

  button = gtk_builder_get_object(builder,"button1");
  g_signal_connect(button,"clicked",G_CALLBACK(printHello),NULL);


  //add the rawing area
  drawingArea = gtk_builder_get_object(builder,"drawingarea1");

  /* Signals used to handle the backing surface */
  g_signal_connect (drawingArea, "draw",
                    G_CALLBACK (draw_cb), NULL);
  g_signal_connect (drawingArea,"configure-event",
                    G_CALLBACK (configure_event_cb), NULL);

  /* Event signals */
  g_signal_connect (drawingArea, "motion-notify-event",
                    G_CALLBACK (motion_notify_event_cb), NULL);
  g_signal_connect (drawingArea, "button-press-event",
                    G_CALLBACK (button_press_event_cb), NULL);

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
