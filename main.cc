#include <gtk/gtk.h>

#include <cstdlib>
#include <vector>

#include "Tile.h"
#include "Tool.h"
#include "Tools.h"
#include "Context.h"
#include "PaintingContext.h"
#include "PaletteContext.h"


//how much it steps when you zoom in or out
const float ZOOM_STEP = 0.5f;
const float OUTLINE_WIDTH = 1;
const int PALETTE_SAMPLE_SIZE = 10;


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

//all the tiles and the tile selection
static int selectedTile = 0;
static std::vector<Tile> tiles;

//widgets that have to get changed by random shit
GtkToggleButton * alphaButton;
GtkEntry * drawColourEntry;
GtkDrawingArea * palette;


//multiplies all the colours a colour thing by a value!!!
static int multiplyColour(int colour,float multiplier)
{
  int red = (int)((float)((colour >> 16) & 0xFF) * multiplier);
  int green = (int)((float)((colour >> 8) & 0xFF) * multiplier);
  int blue = (int)((float)(colour & 0xFF) * multiplier);

  if (red > 0xFF)
    red = 0xFF;

  if (green > 0xFF)
    green = 0xFF;

  if (blue > 0xFF)
    blue = 0xFF;

  return (red << 16) | (green << 8) | (blue);
}


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
  alpha = (context->getTile()->getPixel(pickX,pickY) >> 24) & 1;

  //Now invalidate the affected region of the drawing area
  gtk_widget_queue_draw(widget);
}


//create colour thingos
static void setupPalette()
{
  Tile * paletteTile = paletteContext->getTile();

  //make a darker one in the top left
  int darkColour = (1 << 24) | multiplyColour(drawColour,0.5f);

  for (int x = 0;x < PALETTE_SAMPLE_SIZE;x++)
  {
    for (int y = 0;y < PALETTE_SAMPLE_SIZE;y++)
    {
      paletteTile->setPixel(x,y,darkColour);
    }
  }

  //make a lighter on after
  int lightColour = (1 << 24) | multiplyColour(drawColour,2);

  for (int x = paletteTile->width - PALETTE_SAMPLE_SIZE;x < paletteTile->width;x++)
  {
    for (int y = 0;y < PALETTE_SAMPLE_SIZE;y++)
    {
      paletteTile->setPixel(x,y,lightColour);
    }
  }

  //redraw the palette
  gtk_widget_queue_draw(GTK_WIDGET(palette));
}


//configures the drawingareas
static gboolean configureEvent(GtkWidget * widget,GdkEventConfigure * event,
                               gpointer data)
{
  Context * context = (Context *)data;

  int windowWidth = gtk_widget_get_allocated_width(widget);
  int windowHeight = gtk_widget_get_allocated_height(widget);
  int tileWidth = context->getTile()->width * context->scale;
  int tileHeight = context->getTile()->height * context->scale;

  context->tileX = windowWidth / 2 - tileWidth / 2;
  context->tileY = windowHeight / 2 - tileHeight / 2;

  return TRUE;
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
    gtk_toggle_button_set_active(alphaButton,alpha == 0);
    setupPalette();
  }

  //dealt with
  return TRUE;
}


//when the mouse is moved
static gboolean motionNotifyEvent(GtkWidget * widget,GdkEventMotion * event,
                                   gpointer data)
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

  //dealt with 8)
  return TRUE;
}


//when someone has entered a new colour
static void enterColourEvent(GtkEntry * entry,gpointer data)
{
  drawColour = (int)(strtol(gtk_entry_get_text(entry),NULL,0) & 0xFFFFFF);
  gtk_toggle_button_set_active(alphaButton,FALSE);
  setupPalette();
}


//when someone toggles alpha drawing
static void alphaToggleEvent(GtkToggleButton * button,gpointer data)
{
  if (gtk_toggle_button_get_active(button))
  {
    alpha = 0;
  }
  else
  {
    alpha = 1;
  }
}


//when a tool is selected
static void toolSelectedEvent(GtkToggleButton * button,gpointer data)
{
    currentTool = (Tool *)data;
}


//when the tile select area needs to draw
static gboolean tileSelectDrawEvent(GtkWidget * widget,cairo_t * cr,
                                    gpointer data)
{

}


//when the mouse is clicked in the tile select area
static gboolean tileSelectClickEvent(GtkWidget * widget,GdkEventButton * event,
                                 gpointer data)
{

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
  GObject * tileSelectArea;
  GObject * drawColourEntry;
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
  g_signal_connect(drawingArea,"configure-event",G_CALLBACK(configureEvent),(gpointer)paintingContext);
  g_signal_connect(drawingArea,"motion-notify-event",G_CALLBACK(motionNotifyEvent),(gpointer)paintingContext);
  g_signal_connect(drawingArea,"button-press-event",G_CALLBACK(buttonPressEvent),(gpointer)paintingContext);
  g_signal_connect(drawingArea,"scroll-event",G_CALLBACK(scrollEvent),(gpointer)paintingContext);

  //connect the palette
  palette = GTK_DRAWING_AREA(gtk_builder_get_object(builder,"palette"));
  g_signal_connect(palette,"draw",G_CALLBACK(drawEvent),(gpointer)paletteContext);
  g_signal_connect(palette,"configure-event",G_CALLBACK(configureEvent),(gpointer)paletteContext);
  g_signal_connect(palette,"motion-notify-event",G_CALLBACK(motionNotifyEvent),(gpointer)paletteContext);
  g_signal_connect(palette,"button-press-event",G_CALLBACK(buttonPressEvent),(gpointer)paletteContext);
  g_signal_connect(palette,"scroll-event",G_CALLBACK(scrollEvent),(gpointer)paletteContext);

  //connect the tile select area
  tileSelectArea = gtk_builder_get_object(builder,"tileSelectArea");
  g_signal_connect(tileSelectArea,"draw",G_CALLBACK(drawEvent),(gpointer)paintingContext);

  drawColourEntry = gtk_builder_get_object(builder,"drawColourEntry");
  g_signal_connect(drawColourEntry,"activate",G_CALLBACK(enterColourEvent),NULL);

  alphaButton = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"alphaButton"));
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
