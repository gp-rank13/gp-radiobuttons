# Radio Button Extension for Gig Performer

## Overview
An extension for [Gig Performer](https://gigperformer.com) that implements radio button widgets. _Radio buttons_ are mutually exclusive, meaning you can only activate one widget at a time from a defined group of widgets.

The extension adds 4 panel templates for 2, 4, 8 or 16 radio buttons. These can be acccessed from the _New panel_ menu while in _Edit_ mode in Gig Performer.

![gp-radiobuttons](https://user-images.githubusercontent.com/107261652/173212168-8d1d10c9-0a50-45f5-a2c8-b1b6c1183666.gif)

Normal usage will be for the radio buttons to select different plugin parameters e.g. mapping the buttons to the _Solo_ buttons on a 4-channel mixer to implement _exclusive solo_ for each channel. Advanced usage includes the ability to use the radio buttons to select different discrete values for a single plugin parameter e.g. create _Slow_, _Medium_ and _Fast_ buttons for a single _Speed_ plugin parameter.

Radio button functionality can also be easily added to existing button widgets in previously created panels.

The extension supports up to 10 different radio button groups, with up to 16 radio buttons in each group.

> _Disclaimer: This is a user-created extension and is not supported by Deskew Techologies. Extensions can cause unexpected behaviour in Gig Performer, and in extreme situations could cause it to crash. Please test it thoroughly before using it in any live performances!_

## Installation

Download the extension (library) files for either MacOS or Windows.  

Mac users will need to move the _libRadioButtons.dylib_ file into this folder and then restart Gig Performer:
```
/Users/Shared/Gig Performer/Extensions
```
Windows users will need to move the _RadioButtons.dll_ file into this folder and then restart Gig Performer:
```
C:\Users\Public\Documents\Gig Performer\Extensions
```
When opening Gig Perfomer, a prompt will appear asking to confirm whether to install the extension.

## Customizing the Template Panels

The panels are stored within the compiled extension, but these can be overridden with custom versions. Custom panels must be created in Gig Performer (e.g. using the in-built templates as a starting point) and then exported as individual panel files.  

Panel files must be moved to the extension folder and names must follow the format:
```
RadioButtons2.gppanel
RadioButtons4.gppanel
RadioButtons8.gppanel
RadioButtons16.gppanel
```
Only panel files that are customized are required e.g. if only the 4-button panel is customized, then only the _RadioButtons4.gppanel_ file needs to be added to the extension folder.

## Usage

The extension relies on a special naming format for the widgets, added in the _Advanced_ tab of the _Widget Properties_. The panel templates have already applied this naming to the widgets. However, additional buttons can be added to a radio button group by following this naming convention.

<img width="553" alt="gp-radiobuttons-widget-properties" src="https://user-images.githubusercontent.com/107261652/173212382-7e43735e-c3f1-4c4a-b2f0-9108003ed884.png">

<br />

### Syntax

> RADIO\_[GroupNumber]\_[ButtonNumber]

__Example__  
3 radio buttons in group 1:
```
RADIO_1_1, RADIO_1_2, RADIO_1_3
```
4 radio buttons in group 2:
```
RADIO_2_1, RADIO_2_2, RADIO_2_3, RADIO_2_4
```
### Advanced Syntax

You can also use the radio buttons on a single plugin parameter, where the buttons will select specific values e.g. creating buttons for _Slow_ , _Medium_ and _Fast_ settings on a _Speed_ parameter.

> RADIO\_[GroupNumber]\_[ButtonNumber]\_[PluginHandle]\_[ParameterNumber]\_[ParameterValue]

- The _PluginHandle_ is set in Gig Perfomer's _Wiring View_ by right-clicking the plugin block and using the menu item _OSC/GPScript Handle_.   
- The _ParameterNumber_ is the plugin's parameter number. These can be seen at the top of the plugin window when you change the parameter in the plugin UI. 
- The _Parameter Value_ is an integer between 0 and 100.  
 

__When using the advanced systax, the widgets *cannot* be mapped to plugn parameters. Instead, the widget names will reference the plugin handle and the extension will set the parameter value directly in the plugin.__

__Example 1__  
Add a plugin handle to the _Helix Native_ plugin (named _NATIVE_), and then the following widget names will set the first 4 Snapshots (parameter 36):
```
RADIO_1_1_NATIVE_36_0
RADIO_1_2_NATIVE_36_14
RADIO_1_3_NATIVE_36_29
RADIO_1_4_NATIVE_36_43
```

__Example 2__  
Add a plugin handle to _PSP L'otary2_ plugin (named _ROTARY_), and then the following widget names will allow 3 discrete settings for the speed _Level_ (parameter 2) - Slow (0), Medium (50) and Fast (100): 
```
RADIO_1_1_ROTARY_2_0
RADIO_1_2_ROTARY_2_50
RADIO_1_3_ROTARY_2_100
```

## Building the Extension

To customize/build the extension, refer to the [Gig Performer SDK](https://github.com/gigperformer/gp-sdk) GitHub page.

