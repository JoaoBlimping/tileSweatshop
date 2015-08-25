//this namespace makes hotkeys activate the correct functions

#ifndef HOTKEY_MANAGER_H
#define HOTKEY_MANAGER_H


#include <gtk/gtk.h>

#include <map>
#include <vector>


namespace HotkeyManager
{
  //maps keys to their pressed value
  static std::map<guint,bool> keyMap;

  //maps key combinations to functions
  static std::map<std::vector<guint> *,void(*)()> functionMap;


  //connects the key events for a widget to this thing's functions
  void connect(GtkWidget * widget);

  //adds a function to the hotkey manager
  void addFunction(std::vector<guint> * keys,void(*function)());

  void addFunction(guint * keys,int nKeys,void(*function)());

  //occurs when a key has been pressed
  gboolean keyPressEvent(GtkWidget * widget,GdkEvent * event,gpointer data);

  //occurs when a key has been released
  gboolean keyReleaseEvent(GtkWidget * widget,GdkEvent * event,gpointer data);
}


#endif
