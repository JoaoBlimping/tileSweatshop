#include <gtk/gtk.h>
#include <cstdlib>

#include "Tile.h"
#include "Tool.h"
#include "Tools.h"
#include "Context.h"
#include "PaintingContext.h"
#include "PaletteContext.h"


//how much it steps when you zoom in or out
const float ZOOM_STEP = 0.5f;
const float OUTLINE_WIDTH = 1;


//the tile contexts
static Context const * const paintingContext = new PaintingContext(new Tile(32,32));
static Context const * const paletteContext = new PaletteContext();

//pan context drawing stuff
static Tool const * currentTool = Tools::pen;
static int drawColour = 0;
static int alpha = 1;

static float alphaRed = 0.2f;
static float alphaGreen = 0.2f;
static float alphaBlue = 0.2f;


//redraws everything
static void redrawSurface(cairo_t * cr,Context * context)
{
  //clear background
  cairo_set_source_rgb(cr,alphaRed,alphaGreen,alphaBlue);
  cairo_paint(cr);

  //put a border around the tile to show where it ends
  cairo_set_source_rgb (cr,0,0,0);
  cairo_rectangle(cr,context->tileX,context->tileY,context->getTile()->width * context->scale,context->getTile()->height * context->scale);
  cairo_set_line_width(cr,OUTLINE_WIDTH);
  cairo_stroke(cr);

  //display the tile
  context->getTile()->render(context->tileX,context->tileY,context->scale,cr);
}


//draw on the tile at the given position
static void fillPixel(GtkWidget * widget,gdouble x,gdouble y,Context * context)
{
  if (currentTool == NULL)
  {
    return;
  }

  int drawX = (int)((x - context->tileX) / context->scale);
  int drawY = (int)((y - context->tileY) / context->scale);

  //make sure they aren't going out of the tile
  if (drawX < 0 || drawY < 0 || drawX > context->getTile()->width || drawY > context->getTile()->height)
  {
    return;
  }

  currentTool->apply(drawX,drawY,drawColour | (alpha << 24),context->getTile());

  //Now invalidate the affected region of the drawing area
  gtk_widget_queue_draw(widget);
}


//get the colour from a pixel and set it as the current drawing colour
static void pickPixel(GtkWidget * widget,gdouble x,gdouble y,Context * context)
{
  int pickX = (int)((x - context->tileX) / context->scale);
  int pickY = (int)((y - context->tileY) / context->scale);

  //make sure they aren't going out of the tile
  if (pickX < 0 || pickY < 0 || pickX > context->getTile()->width || pickY > context->getTile()->height)
  {
    return;
  }

  drawColour = context->getTile()->getPixel(pickX,pickY) & 0xFFFFFF;

  //Now invalidate the affected region of the drawing area
  gtk_widget_queue_draw(widget);
}


//called when part of the image needs to be redrawn
//cr is a drawing thing which is already clipped to the offending area
static gboolean drawEvent(GtkWidget * widget,cairo_t * cr,gpointer data)
{
  Context * context = (Context *)data;

  redrawSurface(cr,context);

  //let it continue on it's merry way <3
  return FALSE;
}


//event called when the mouse is clicked
static gboolean buttonPressEvent(GtkWidget * widget,GdkEventButton * event,
                                 gpointer data)
{
  Context * context = (Context *)data;

  if (event->button == GDK_BUTTON_PRIMARY)
  {
    fillPixel(widget, event->x, event->y,context);
  }
  else if (event->button == GDK_BUTTON_MIDDLE)
  {
    context->dragOriginX = event->x - context->tileX;
    context->dragOriginY = event->y - context->tileY;
  }
  else if (event->button == GDK_BUTTON_SECONDARY)
  {
    pickPixel(widget, event->x, event->y,context);
  }

  //dealt with
  return TRUE;
}


//when the mouse is moved
static gboolean motionNotifyEvent (GtkWidget      *widget,
                        GdkEventMotion *event,
                        gpointer        data)
{
  Context * context = (Context *)data;

  if (event->state & GDK_BUTTON1_MASK)
  {
    fillPixel(widget, event->x, event->y,context);
  }
  else if (event->state & GDK_BUTTON2_MASK)
  {
    context->tileX = event->x - context->dragOriginX;
    context->tileY = event->y - context->dragOriginY;
    gtk_widget_queue_draw(widget);
  }

  //dealt with
  return TRUE;
}


static gboolean scrollEvent(GtkWidget * widget,GdkEventScroll * event,
                            gpointer data)
{
  Context * context = (Context *)data;

  //do the scroll
  if (event->direction == GDK_SCROLL_UP)
  {
    context->scale += ZOOM_STEP;
  }

  if (event->direction == GDK_SCROLL_DOWN)
  {
    context->scale -= ZOOM_STEP;
  }

  //make sure it doesn't go too low
  //if it does, it like flips inside out which is mad
  if (context->scale <= 0)
  {
    context->scale = ZOOM_STEP;
  }

  //redraw the screen
  gtk_widget_queue_draw(widget);

  //dealt with
  return TRUE;
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


//when a tool is selected
static void toolSelectedEvent(GtkToggleButton * button,gpointer data)
{
    currentTool = (Tool *)data;
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
  GObject * palette;
  GObject * drawColourEntry;
  GObject * alphaButton;
  GObject * toolbox;


  gtk_init(&argc,&argv);

  //create a builder and load our description
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder,"assets/builder.ui",NULL);

  //connect signal handlers to the constructed widgets
  window = gtk_builder_get_object(builder,"window1");
  g_signal_connect(window,"destroy",G_CALLBACK(close_window),NULL);

  //connect the drawing area
  drawingArea = gtk_builder_get_object(builder,"drawingarea");
  g_signal_connect(drawingArea,"draw",G_CALLBACK(drawEvent),(gpointer)paintingContext);
  g_signal_connect(drawingArea,"motion-notify-event",G_CALLBACK(motionNotifyEvent),(gpointer)paintingContext);
  g_signal_connect(drawingArea,"button-press-event",G_CALLBACK(buttonPressEvent),(gpointer)paintingContext);
  g_signal_connect(drawingArea,"scroll-event",G_CALLBACK(scrollEvent),(gpointer)paintingContext);

  //connect the palette
  palette = gtk_builder_get_object(builder,"palette");
  g_signal_connect(palette,"draw",G_CALLBACK(drawEvent),(gpointer)paletteContext);
  g_signal_connect(palette,"motion-notify-event",G_CALLBACK(motionNotifyEvent),(gpointer)paletteContext);
  g_signal_connect(palette,"button-press-event",G_CALLBACK(buttonPressEvent),(gpointer)paletteContext);
  g_signal_connect(palette,"scroll-event",G_CALLBACK(scrollEvent),(gpointer)paletteContext);

  drawColourEntry = gtk_builder_get_object(builder,"drawColourEntry");
  g_signal_connect(drawColourEntry,"activate",G_CALLBACK(enterColourEvent),NULL);

  alphaButton = gtk_builder_get_object(builder,"alphaButton");
  g_signal_connect(alphaButton,"toggled",G_CALLBACK(alphaToggleEvent),NULL);

  //add all the tools to the toolbox
  toolbox = gtk_builder_get_object(builder,"toolbox");

  GtkWidget * penButton = gtk_radio_button_new_with_label(NULL,"pen");
  g_signal_connect(penButton,"toggled",G_CALLBACK(toolSelectedEvent),(gpointer)Tools::pen);
  gtk_box_pack_start(GTK_BOX(toolbox),penButton,FALSE,FALSE,0);

  GtkWidget * fillButton = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(penButton),"fill");
  g_signal_connect(fillButton,"toggled",G_CALLBACK(toolSelectedEvent),(gpointer)Tools::fill);
  gtk_box_pack_start(GTK_BOX(toolbox),fillButton,FALSE,FALSE,0);

  GtkWidget * replaceButton = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(penButton),"replace");
  g_signal_connect(replaceButton,"toggled",G_CALLBACK(toolSelectedEvent),(gpointer)Tools::replace);
  gtk_box_pack_start(GTK_BOX(toolbox),replaceButton,FALSE,FALSE,0);


  //add some events that the drawing area doesn't naturally get
  gtk_widget_set_events (GTK_WIDGET(drawingArea),
                         gtk_widget_get_events (GTK_WIDGET(drawingArea)) |
                         GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK |
                         GDK_SCROLL_MASK);

  gtk_widget_set_events (GTK_WIDGET(palette),
                         gtk_widget_get_events (GTK_WIDGET(palette)) |
                         GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK |
                         GDK_SCROLL_MASK);

  gtk_widget_show_all (GTK_WIDGET(window));

  gtk_main();

  return 0;
}
