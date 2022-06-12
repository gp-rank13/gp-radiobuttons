// Radio Button extension for Gig Performer by @rank13
#include "LibMain.h"
#include "Panels.h"

//#include <cstdint>

using GPUtils = gigperformer::sdk::GPUtils;

// Return the panel count
int LibMain::GetPanelCount() {
   return panelNames.size();
}

// Return the panel name
 std::string LibMain::GetPanelName(int index) {
   std::string text; 
   size_t panelIndex = (size_t)index;
   if (panelIndex >= 0 && panelIndex < panelNames.size())
      text = panelNames[index];
         
   return text;      
}

// Return the panel layout in XML format - either internal template or custom user panel
 std::string LibMain::GetPanelXML(int index) {
   std::string text;
   size_t panelIndex = (size_t)index;
   if (panelIndex >= 0 && panelIndex < panelNames.size()) {
      // The custom user panels must be in the same folder as the library
      GPUtils::loadTextFile(getPathToMe() + "/" + userPanelNames[index], text );

      // If no custom user panels are found, use the default template panel
      if (text == "") text = panelXML[index];
   }
   return text;
}

// Extract the widget type from the beginning of the combined "Type:Name" format
std::string LibMain::getWidgetType(std::string widgetName) {
   std::string widgetType;
   if (widgetName.find(":"))
      widgetType = widgetName.substr(0, widgetName.find(":"));

   return widgetType;
}

// Extract the widget name from the end of the combined "Type:Name" format
std::string LibMain::extractWidgetName(std::string widgetName) {
   if (widgetName.find(":"))
      widgetName = widgetName.substr(widgetName.find(":") + 1);

   return widgetName;
}

// Split a widget handle into parts based on the supplied delimiter and return as a list
std::vector<std::string> LibMain::splitWidgetName(std::string widgetName, char delim) {
   std::string part;
   std::vector<std::string> parts;
   std::stringstream ss(widgetName);
   while(std::getline(ss, part, delim)) {
      parts.push_back(part);
   }
   return parts;
}

// Add widget listeners to the supplied widget handles
void LibMain::addWidgetListener(std::vector<std::string> widgets) {
   for (size_t i = 0; i < widgets.size(); i++) {
      listenForWidget(widgets[i], true);
      widgetList.push_back(widgets[i]);
   } 
}

// Check whether the widgets on the global or local rackspace are radio buttons and add listeners if they are new
void LibMain::checkAndUpdateRadioButtonListeners(bool useGlobalRackspace) {
   std::vector<std::string> list;
   getWidgetList(list, useGlobalRackspace);
   //consoleLog("Check and Update Radio Buttons");
   for (size_t i = 0; i < list.size(); i++) {
      std::string widget = list[i];
      if (getWidgetType(widget) == "Button") {
         widget = extractWidgetName(widget);
         if(RadioButton::isRadioButtonExtendedSyntax(widget)) {
            if (!RadioButton::inRadioButtonList(widget)) {
               RadioButton::radioButtonList.push_back(widget);
               listenForWidget(widget, true);
            }
         }
      }
   }
}

// Handle widget changes
void LibMain::OnWidgetValueChanged(const std::string &widgetName, double newValue) {
   // Determine if the widget is a radio button
   if (RadioButton::isRadioButton(widgetName)) {
      std::string group = RadioButton::getRadioGroup(widgetName);
      if (newValue == 1.0) {
         // Iterate through the radio buttons and de-select the others in the group
         for (size_t i = 0; i < RadioButton::radioButtonList.size(); i++) {
            if (widgetExists(RadioButton::radioButtonList[i]))
               if (RadioButton::inRadioGroup(RadioButton::radioButtonList[i], group))
                  if (RadioButton::radioButtonList[i] != widgetName)
                        setWidgetValue(RadioButton::radioButtonList[i], 0.0);
         }
         // Set parameter values directly in the plugin
         if (RadioButton::isRadioButtonExtendedSyntax(widgetName)) {
            std::vector<std::string> widgetNameParts = splitWidgetName(widgetName, '_');
            std::string plugin = widgetNameParts[3];
            //consoleLog(plugin + " uses extended syntax");
            int parameter = std::stoi(widgetNameParts[4]);
            float value = std::stof(widgetNameParts[5]) / 100;
            bool useGlobalRackspace = pluginExists(plugin, true);
            setPluginParameter(plugin, parameter, value, useGlobalRackspace);
         }
      }
      else {
         // Ensure there is always one radio button in the group selected (depending on the user setting)
         if (RadioButton::RADIO_ENFORCE_SELECTION) {
            bool hasSelectedButton = false;
            for (size_t i = 0; i < RadioButton::radioButtonList.size(); i++) {
               if (RadioButton::getRadioGroup(RadioButton::radioButtonList[i]) == group && getWidgetValue(RadioButton::radioButtonList[i]) == 1.0)
                  hasSelectedButton = true;
            }
            if (!hasSelectedButton)
                  setWidgetValue(widgetName, 1.0);
         }
      }
   }
} 

// Handle rackspace changes: check for any new radio buttons with extended syntax
void LibMain::OnRackspaceActivated() {
   LibMain::checkAndUpdateRadioButtonListeners(false);
}

// Handle exiting Edit mode: check for any new radio buttons with extended syntax
void LibMain::OnEditStateChanged(bool inEditState) {
   if (!inEditState) {
      LibMain::checkAndUpdateRadioButtonListeners(false);
      LibMain::checkAndUpdateRadioButtonListeners(true);
   }
}

// Update the widget listeners on the global rackspace when the gig has loaded
void LibMain::OnStatusChanged(GPStatusType status) {
   if (status == GPStatus_GigFinishedLoading)
      LibMain::checkAndUpdateRadioButtonListeners(true);
} 

namespace gigperformer {
   namespace sdk {
      GigPerformerAPI *CreateGPExtension(LibraryHandle handle) {
         return new LibMain(handle);
      }
   } // namespace sdk
} // namespace gigperformer
