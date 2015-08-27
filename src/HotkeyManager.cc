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
  std::vector<guint> * vectorKeys = new std::vector();
  vectorKeys.resize(nKeys);

  for (int i = 0;i < nKeys;i++)
  {
    vectorKeys->push_back(keys[i]);
  }

  functionMap[vectorKeys] = function;
};


gboolean HotkeyManager::keyPressEvent(GtkWidget * widget,GdkEvent * event,
                                      gpointer data)
{
  try
  {
    keyMap.at(GDK_EVENT_KEY(event)->keyval) = TRUE;
  }
  catch (...){}

  //check if any sets of keys are now activated
  for (std::map<std::vector<guint> *,void(*)()>::iterator functionIterator = functionMap.begin();
       functionIterator != functionMap.end();++functionIterator)
  {
    bool good = true;
    for (std::vector<int>::iterator keyIterator = myvector.begin();
         keyIterator != myvector.end();++keyIterator)
    {
      if (!keyMap.at(*keyIterator))
      {
        good = false;
      }
    }

    if (good)
    {
      functionIterator->second();
    }
  }
};


gboolean HotkeyManager::keyReleaseEvent(GtkWidget * widget,GdkEvent * event,
                                        gpointer data)
{
  try
  {
    keys.at(GDK_EVENT_KEY(event)->keyval) = FALSE;
  }
  catch (...){}
};
