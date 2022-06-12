// Radio Button extension for Gig Performer by @rank13
#include <string>
#include <vector>

class RadioButton
{  
public:
    static const int RADIO_GROUP_COUNT;
    static const int RADIO_BUTTON_COUNT;
    static const bool RADIO_ENFORCE_SELECTION;
    static const std::string RADIO_NAME_PREFIX;

    static std::vector<std::string> radioButtonList; 
    static bool isRadioButton(std::string handle);
    static bool isRadioButtonExtendedSyntax(std::string handle);
    static bool inRadioGroup(std::string handle, std::string group);
    static bool inRadioButtonList(std::string handle);
    static std::string getRadioGroup(std::string handle);
    static std::string getPluginNameFromExtendedSyntax(std::string handle);
    static std::vector<std::string> generateRadioButtons();
};