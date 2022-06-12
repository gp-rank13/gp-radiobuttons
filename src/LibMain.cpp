#include "LibMain.h"

#include <cstdint>

using GPUtils = gigperformer::sdk::GPUtils;

/// Ignore a given value
/// \details this is a dummy function to suppress compiler warnings about unused parameters
template <typename T> void Ignore(T const &) noexcept
{
}

// define an XML string describing your product
const std::string XMLProductDescription =
    // Replace with your information
    "<Library>"
    "  <Product"
    "    Name=\"Hello GP - CPP\""
    "    Version=\"1.0\""
    "    BuildDate=\"06/10/2022\">"
    "  </Product> "
    "  <Description>Briefly describe your product</Description>"
    "  <ImagePath>/Path/To/ImageFile/foo.jpg</ImagePath>"
    "</Library>";

// List of panels
std::vector<std::string> panelNames = {
    "Red",
    "Yellow",
    "Blue",
};
std::vector<std::string> relativePanelLocations = {
    "Red.gppanel",
    "Yellow.gppanel",
    "Blue.gppanel",
};

std::string pathToMe; // This needs to be initialized from the initialization
                      // section of the LibMain class so it can be used in the
                      // standalone functions directly below

int LibMain::GetPanelCount()
{
    return static_cast<int>(panelNames.size());
}

std::string LibMain::GetPanelName(int index)
{
    std::string text;
    if (index >= 0 && static_cast<std::size_t>(index) < panelNames.size())
    {
        text = panelNames[index];
    }

    return text;
}

// Return panel layout in XML format
std::string LibMain::GetPanelXML(int index)
{
    std::string text;
    if (index >= 0 && static_cast<std::size_t>(index) < panelNames.size())
    {
        // We assume the panels are in the same folder as the library
        GPUtils::loadTextFile(getPathToMe() + "/" + relativePanelLocations[index], text);
    }
    return text;
}

// List of menu items
std::vector<std::string> menuNames = {
    "Show Front Panels",
    "Show Back Wiring",
    "Show Setlists",
};

int LibMain::GetMenuCount()
{
    return static_cast<int>(menuNames.size());
}

std::string LibMain::GetMenuName(int index)
{
    std::string text;
    if (index >= 0 && static_cast<std::size_t>(index) < menuNames.size())
    {
        text = menuNames[index];
    }

    return text;
}

void LibMain::InvokeMenu(int index)
{
    if (index >= 0 && static_cast<std::size_t>(index) < menuNames.size())
    {
        switch (index)
        {
        case 0:
            switchToPanelView();
            break;
        case 1:
            switchToWiringView();
            break;
        case 2:
            switchToSetlistView();
            break;

        default:
            break;
        }
    }
}
void LibMain::OnModeChanged(int mode)
{
    consoleLog(std::string("Switching to mode: ") + ((mode == GP_SetlistMode) ? "Setlist" : "FrontBack"));
}

void LibMain::OnSwitchToPanelView()
{

    consoleLog("Switching to panel view");
    setPlayheadState(true);

    int count = getPluginParameterCount("Legend", false);
    consoleLog(std::to_string(count));

    std::string p1 = getPluginParameterName("Legend", 1, false);
    consoleLog(p1);
}

void LibMain::OnSwitchToWiringView()
{
    consoleLog("Switching to wiring view");
    setPlayheadState(false);

    setBPM(99);
}

void LibMain::OnGlobalPlayStateChanged(bool playing)
{
    consoleLog(std::string("playing = ") + (playing ? " true" : "false"));

    // switchToSetlistView();
    // switchToWiringView();

    int color = RGBAToColor(1, 0, 0, 0.5);
    setWidgetFillColor("MyShape", color);

    double red, green, blue, alpha;
    ColorToRGBA(color, red, green, blue, alpha);
    consoleLog(std::to_string(alpha));
}

void LibMain::OnWidgetValueChanged(const std::string &widgetName, double newValue)
{
    Ignore(widgetName);
    Ignore(newValue);

    // int noteNumber = (int)GPUtils::Scale(newValue, 0.0, 1.0, 36, 96);
    // GPMidiMessage n = GPMidiMessage::makeNoteOnMessage(noteNumber, 64, 0);
    // const std::string device = "MIDI Monitor (Untitled)";
    // sendMidiMessageToMidiOutDevice(device, n);

    std::string caption = getWidgetCaption("abc");
    consoleLog("caption: " + caption);
    std::string value = getWidgetTextValue("abc");
    consoleLog("value: " + value);

    // consoleLog(newValue > 0.5 ? "Yes" : "No");
    // if (newValue > 0.5)
    // {
    //     if (widgetName == "abc")
    //     {
    //         next();
    //     }
    //     else
    //     {
    //         previous();
    //     }
    // }
}

void LibMain::OnMidiDeviceListChanged(std::vector<std::string> &inputs, std::vector<std::string> &outputs)
{
    for (std::size_t i = 0; i < inputs.size(); i++)
    {
        std::string &name = inputs[i];
        consoleLog("Input found: " + name);
    }

    Ignore(outputs);
}

void LibMain::OnWidgetCaptionChanged(const std::string &widgetName, const std::string &newCaption)
{
    Ignore(widgetName);
    consoleLog("Widget caption changed: " + newCaption);
}

void LibMain::OnSongChanged(int oldIndex, int newIndex)
{
    Ignore(oldIndex);
    consoleLog("Song changed from C++ example");
    std::string name = getSongName(newIndex);
    consoleLog("New song is called " + name);
}

void LibMain::OnWidgetStateChanged(const std::string &widgetName, int newState)
{
    consoleLog("Widget " + widgetName + ": " + ((newState == 0) ? "created" : "removed"));
}

void LibMain::Initialization()
{
    // Do any initialization that you need

    // .... your code here

    // Finally, register all the methods that you are going to actually use,
    // i.e, the ones you declared above as override
    registerCallback("OnSongChanged");
    registerCallback("OnStatusChanged");
    registerCallback("OnMidiDeviceListChanged");
    registerCallback("OnWidgetValueChanged");
    registerCallback("OnWidgetStateChanged");
    registerCallback("OnWidgetCaptionChanged");
    registerCallback("OnGlobalPlayStateChanged");
    registerCallback("OnModeChanged");
    registerCallback("OnSwitchToPanelView");
    registerCallback("OnSwitchToWiringView");
    registerCallback("OnTempoChanged");
    registerCallback("OnSetlistChanged");
    registerCallback("OnRackspaceActivated");
    listenForWidget("abc", true);
    listenForWidget("def", true);

    consoleLog("path to library " + getPathToMe());
}

std::string LibMain::GetProductDescription()
{
    // Generally don't touch this - simply define the constant
    // 'XMLProductDescription' at the top of this file with an XML description of
    // your product
    return XMLProductDescription;
}

extern "C" void addtwo(GPRuntimeEngine *vm)
{
    int b = GP_VM_PopInteger(vm);
    int a = GP_VM_PopInteger(vm);
    GP_VM_PushInteger(vm, a + b);
}

extern "C" void subtracttwo(GPRuntimeEngine *vm)
{
    int b = GP_VM_PopInteger(vm);
    int a = GP_VM_PopInteger(vm);
    GP_VM_PushInteger(vm, a - b);
}

extern "C" void Increment(GPRuntimeEngine *vm)
{
    int a = GP_VM_PopInteger(vm);
    GP_VM_PushInteger(vm, a + 1);
}

extern "C" void DupString(GPRuntimeEngine *vm)
{
    char buffer[100];
    GP_VM_PopString(vm, buffer, 100);
    std::string s = buffer;
    s = s + s;

    GP_VM_PushString(vm, s.c_str());
}

ExternalAPI_GPScriptFunctionDefinition functionList[] = {
    {"AddTwo", "a : integer, b :integer", "Returns Integer", "Add the integers", addtwo},
    {"SubtractTwo", "a : integer,b :integer", "Returns Integer", "Subtract the integers", subtracttwo},
    {"Increment", "a : integer", "Returns Integer", "Increment incoming value", Increment},
    {"DupString", "a : String", "Returns String", "Return with the string twice", DupString},
};

int LibMain::RequestGPScriptFunctionSignatureList(GPScript_AllowedLocations location,
                                                  ExternalAPI_GPScriptFunctionDefinition **list)
{
    Ignore(location);

    // Allow these in ANY script so no need to check the location field
    *list = functionList;
    int count = sizeof(functionList) / sizeof(ExternalAPI_GPScriptFunctionDefinition);
    return count;
}

namespace gigperformer
{
namespace sdk
{

GigPerformerAPI *CreateGPExtension(LibraryHandle handle)
{
    return new LibMain(handle);
}

} // namespace sdk
} // namespace gigperformer
