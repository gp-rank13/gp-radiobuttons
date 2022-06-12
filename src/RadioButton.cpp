// Radio Button extension for Gig Performer by @rank13
#include "RadioButton.h"
#include <regex>

const int RadioButton::RADIO_GROUP_COUNT = 10; // Set the maximum number of radio groups to support
const int RadioButton::RADIO_BUTTON_COUNT = 16; // Set the maximum number of radio buttons within each group
const bool RadioButton::RADIO_ENFORCE_SELECTION = true; // Set whether there should always be one radio button selected
const std::string RadioButton::RADIO_NAME_PREFIX = std::string("RADIO"); // Set the widget handle prefix
std::vector<std::string> RadioButton::radioButtonList; 

// Check whether the supplied handle adheres to the radio button naming format (PREFIX_GROUPNUM_BUTTONNUM e.g. RADIO_1_1) or the extended version that includes a plugin handle and parameter/value.
bool RadioButton::isRadioButton(std::string handle) {
    if (std::regex_match(handle, std::regex(RADIO_NAME_PREFIX + "_[0-9]+_[0-9]+$")) || std::regex_match(handle, std::regex(RADIO_NAME_PREFIX + "_[0-9]+_[0-9]+_[A-Z]+_[0-9]+_[0-9]+$")))
        return true;
    else
        return false;
}

// Check whether the supplied handle matches the extended syntax version that includes a plugin handle and parameter/value (PREFIX_GROUPNUM_BUTTONNUM_PLUGIN_PARAMETERNUM_VALUE e.g. RADIO_1_1_NATIVE_36_0)
bool RadioButton::isRadioButtonExtendedSyntax(std::string handle) {
    if (std::regex_match(handle, std::regex(RADIO_NAME_PREFIX + "_[0-9]+_[0-9]+_[A-Z]+_[0-9]+_[0-9]+$")))
        return true;
    else
        return false;
}

// Check whether the supplied handle is a radio button that's in the same group as the supplied group
bool RadioButton::inRadioGroup(std::string handle, std::string group) {
    // Extract the number following the first underscore in the handle
    if (handle.substr(RADIO_NAME_PREFIX.length() + 1, handle.find("_", RADIO_NAME_PREFIX.length() + 1) - (RADIO_NAME_PREFIX.length() + 1)) == group)
        return true;
    else
        return false;
}

// Check whether the supplied handle exists in the radio button list
bool RadioButton::inRadioButtonList(std::string handle) {
    for (size_t i = 1; i <= radioButtonList.size(); i++) {
        if (radioButtonList[i] == handle)
            return true;
    }
    return false;
}

// Extract the radio group from the supplied handle
std::string RadioButton::getRadioGroup(std::string handle) {
    std::string group;
    if (handle.length() > RADIO_NAME_PREFIX.length() + 3)
        // Extract the number following the first underscore in the handle
        group = handle.substr(RADIO_NAME_PREFIX.length() + 1, handle.find("_", RADIO_NAME_PREFIX.length() + 1) - (RADIO_NAME_PREFIX.length() + 1));
    
    return group;
}

// Extract the plugin name from the radio button handle extended syntax
std::string RadioButton::getPluginNameFromExtendedSyntax(std::string handle) {
    std::string plugin;
    size_t pos = 0;
    int cnt = 0;
    int nth = 3;
    // Find each successive underscore: the third marks the start of the plugin name
    while( cnt != nth ) {
        pos += 1;
        pos = handle.find("_", pos);
        if ( pos == std::string::npos )
            return plugin;
        cnt++;
    }
    plugin = handle.substr(pos + 1, handle.find("_", pos + 1) - (pos + 1));
    return plugin;
}

// Generate the list of radio button handles, following the format PREFIX_GROUPNUM_BUTTONNUM e.g. RADIO_1_1
std::vector<std::string> RadioButton::generateRadioButtons() {
    std::string button;
    for (int i = 1; i <= RADIO_GROUP_COUNT; i++) {
        for (int j = 1; j <= RADIO_BUTTON_COUNT; j++) {
            button = RADIO_NAME_PREFIX + "_" + std::to_string(i) + "_" + std::to_string(j);
            radioButtonList.push_back(button);
        }
    }
    return radioButtonList;
}

