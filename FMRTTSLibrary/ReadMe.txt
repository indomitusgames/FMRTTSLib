========================================================================
    STATIC LIBRARY : FMRTTSLibrary Project Overview
========================================================================

AppWizard has created this FMRTTSLibrary library project for you.

No source files were created as part of your project.


FMRTTSLibrary.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

FMRTTSLibrary.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////

Voices in SAPI are selected via ISpObjectToken. Typically one uses the helper function SpFindBestToken to find the best token, and then use ISpVoice::SetVoice() to select that token:

ISpObjectToken* cpToken(NULL);
SpFindBestToken(SPCAT_VOICES, szRequiredAttribs, L"", &cpToken);
pVoice->SetVoice(cpToken);
cpToken->Release();
Now, the only issue is what you need to pass for szRequiredAttribs. This needs to be a semicolon-delimited list of attributes. The best documentation for SAPI voice attributes is in the Object Tokens and Registry Settings whitepaper, specifically, the Voices section. There, it says that all voices must support the following attributes:

Vendor (TTS engine Vendor name)
Language (The LCID in hex of language this engine speaks)
Gender (Value should be "Male" if Male voice, "Female" if female)
VendorPreferred (If this is the Default voice for the vendor named in vendor)
Name (String representing language independent name)
This is not an exclusive listing of attributes; other attributes may be defined by the vendor.

So, if you wanted the preferred voice from Microsoft, you would set szRequiredAttribs = L"vendor=microsoft;vendorpreferred". If you wanted a male voice (from any vendor), set szRequiredAttribs = L"gender=male".