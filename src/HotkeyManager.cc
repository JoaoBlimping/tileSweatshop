#include "HotkeyManager.h"

#include <gtk/gtk.h>

#include <vector>
#include <iterator>


void HotkeyManager::connect(GtkWidget * widget)
{
  g_signal_connect(widget,"key-press-event",G_CALLBACK(keyPressEvent),NULL);
  g_signal_connect(widget,"key-release-event",G_CALLBACK(keyReleaseEvent),NULL);
};


void HotkeyManager::addFunction(std::vector<guint> * keys,void(*function)())
{
  functionMap[keys] = function;
};


void HotkeyManager::addFunction(guint * keys,int nKeys,void(*function)())
{
  std::vector<guint> * vectorKeys = new std::vector<guint>();
  vectorKeys->resize(nKeys);

  for (int i = 0;i < nKeys;i++)
  {
    (*vectorKeys)[i] = keys[i];
  }

  functionMap[vectorKeys] = function;
};


gboolean HotkeyManager::keyPressEvent(GtkWidget * widget,GdkEventKey * event,
                                      gpointer data)
{
  try
  {
    keyMap.at(event->keyval) = TRUE;
  }
  catch (...){}

  //check if any sets of keys are now activated
  for (std::map<std::vector<guint> *,void(*)()>::iterator functionIterator = functionMap.begin();
       functionIterator != functionMap.end();++functionIterator)
  {
    bool good = true;
    for (std::vector<guint>::iterator keyIterator = functionIterator->first->begin();
         keyIterator != functionIterator->first->end();++keyIterator)
    {
      if (!keyMap[*keyIterator])
      {
        good = false;
      }
    }

    if (good)
    {
      functionIterator->second();
    }
  }

  //let other things have this event if tthey want
  return FALSE;
};


gboolean HotkeyManager::keyReleaseEvent(GtkWidget * widget,GdkEventKey * event,
                                        gpointer data)
{
  try
  {
    keyMap.at(event->keyval) = FALSE;
  }
  catch (...){}

  //let other things habe this event if they want
  return FALSE;
};
