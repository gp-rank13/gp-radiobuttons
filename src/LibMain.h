#pragma once

#include "gigperformer/sdk/GPMidiMessages.h"
#include "gigperformer/sdk/GPUtils.h"
#include "gigperformer/sdk/GigPerformerAPI.h"
#include "gigperformer/sdk/types.h"
#include <string>
#include <sstream>
#include "RadioButton.h"

// Extension name and description
const std::string XMLProductDescription =   
    "<Library>" 
    "<Product Name=\"Radio Buttons\" Version=\"1.4\" BuildDate=\"11/6/2022\"></Product> "
    "<Description>Adds Radio Button panel templates to Gig Performer.</Description>"
    "</Library>"; 

// The widgetList is used to register the widgets for listening (to trigger the callbacks) and for reference/update during callbacks
// This can be manually populated if you are not generating a dynamic list
static std::vector<std::string> widgetList;

class LibMain : public gigperformer::sdk::GigPerformerAPI
{
  protected:
    int GetPanelCount() override;
    std::string GetPanelName(int index) override;
    std::string GetPanelXML(int index) override;

  public:
    // These must be here but no need to do anything unless you want extra behavior
    explicit LibMain(LibraryHandle handle) : GigPerformerAPI(handle)  {}
    ~LibMain() override {}

   void OnWidgetValueChanged(const std::string &widgetName, double newValue) override;
    void OnRackspaceActivated() override;
    void OnEditStateChanged(bool inEditState) override;
    void OnStatusChanged(GPStatusType status) override;
    static std::string getWidgetType(std::string widgetName);
    static std::string extractWidgetName(std::string widgetName);
    static std::vector<std::string> splitWidgetName(std::string widgetName, char delim);
    void addWidgetListener(std::vector<std::string> widgets);
    void checkAndUpdateRadioButtonListeners(bool useGlobalRackspace);

    void Initialization() override {
        // Create the list of widgets to use in the extension
            widgetList = RadioButton::generateRadioButtons(); 

        // Register the callbacks to use
        registerCallback("OnWidgetValueChanged");
        registerCallback("OnRackspaceActivated");
        registerCallback("OnEditStateChanged");
        registerCallback("OnStatusChanged");
        
        // Register the widgets in order to listen for changes
        for (size_t i = 0; i < widgetList.size(); i++) {
                listenForWidget(widgetList[i], true);    
        } 
    }

    // XML description of the extension - defined in the separate GPExtension header
    std::string GetProductDescription() override {
           return XMLProductDescription;
    }
};
