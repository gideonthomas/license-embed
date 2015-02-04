#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include <map>

// Must be defined to instantiate template classes
#define TXMP_STRING_TYPE std::string

// Must be defined to give access to XMPFiles
#define XMP_INCLUDE_XMPFILES 1

// Ensure XMP templates are instantiated
#include "public/include/XMP.incl_cpp"

// Provide access to the API
#include "public/include/XMP.hpp"

#include <iostream>
#include <fstream>

using namespace std;

void displayXMP(SXMPMeta* meta) {
    string value = "";
    XMP_DateTime date;

    cout<<"This resource ";

    meta->GetProperty(kXMP_NS_XMP_Rights, "WebStatement", &value, 0);
    if(value == "") {
        cout<<"does not have any specified license information"<<endl;
    } else {
        cout<<"has a license specified by the contents of the following webpage: "<<value<<endl;
    }

    if(meta->GetProperty_Date(kXMP_NS_XMP, "ModifyDate", &date, 0)) {
        // Convert the date struct into a convenient string and display it
        string dateStr;
        SXMPUtils::ConvertFromDate(date, &dateStr);
        cout<<"\tlast modified on "<<dateStr<<endl<<endl;
    }
}

void displayLicenses(map<string, string> licenses) {
    cout<<"------Licenses------"<<endl;

    int i = 1;

    for(map<string, string>::iterator it = licenses.begin(); it != licenses.end(); ++it, ++i) {
        cout<<i<<". "<<it->first<<endl;
    }

    cout<<endl;
}

string getLicense(int index, map<string, string> licenses) {
    int i = 1;

    for(map<string, string>::iterator it = licenses.begin(); it != licenses.end(); ++it, ++i) {
        if(i == index) {
            return it->first;
        }
    }

    return "Attribution";
}

/**
*   Initializes the toolkit and attempts to open a file for reading metadata.  Initially
* an attempt to open the file is done with a handler, if this fails then the file is opened with
* packet scanning. Once the file is open several properties are read and displayed in the console.
* The XMP object is then dumped to a text file and the resource file is closed.
*/
int main(int argc, const char** argv) {
    if(argc != 2) { // 2 := command and 1 parameter
        cout<<"usage: MyModifyXMP (filename)"<<endl;
        return 0;
    }

    string filename = string(argv[1]);

    if(!SXMPMeta::Initialize()) {
        cout<<"Could not initialize toolkit!";
        return -1;
    }

    XMP_OptionBits options = 0;
    #if UNIX_ENV
        options |= kXMPFiles_ServerMode;
    #endif

    // Must initialize SXMPFiles before we use it
    if (!SXMPFiles::Initialize(options)) {
        cout<<"Could not initialize SXMPFiles.";
        return -1;
    }

    try {
        // Options to open the file with - read only and use a file handler
        XMP_OptionBits opts = kXMPFiles_OpenForUpdate | kXMPFiles_OpenUseSmartHandler;

        bool ok;
        SXMPFiles myFile;
        SXMPMeta meta;
        map<string, string> licenses;
        int licenseChoice;
        std::string status = "";

        licenses["Attribution"] = "https://creativecommons.org/licenses/by/4.0/legalcode";
        licenses["Attribution-ShareAlike"] = "https://creativecommons.org/licenses/by-sa/4.0/legalcode";
        licenses["Attribution-NoDerivs"] = "https://creativecommons.org/licenses/by-nd/4.0/legalcode";
        licenses["Attribution-NonCommercial"] = "https://creativecommons.org/licenses/by-nc/4.0/legalcode";
        licenses["Attribution-NonCommercial-ShareAlike"] = "https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode";
        licenses["Attribution-NonCommercial-NoDerivs"] = "https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode";

        // First we try and open the file
        ok = myFile.OpenFile(filename, kXMP_UnknownFile, opts);
        if(!ok) {
            status += "No smart handler available for " + filename + "\n";
            status += "Trying packet scanning.\n";

            // Now try using packet scanning
            opts = kXMPFiles_OpenForUpdate | kXMPFiles_OpenUsePacketScanning;
            ok = myFile.OpenFile(filename, kXMP_UnknownFile, opts);
        }

        // If the file still cannot be opened
        if(!ok) {
            cout<<"Unable to open "<<filename<<endl;

            // Terminate the toolkit
            SXMPFiles::Terminate();
            SXMPMeta::Terminate();

            return -1;
        }

        // If the file is open then read the metadata
        cout<<status<<endl;
        cout<<filename<<" opened successfully"<<endl;
        // Create the xmp object and get the xmp data
        myFile.GetXMP(&meta);

        cout<<"------Current metadata------"<<endl;

        displayXMP(&meta);

        displayLicenses(licenses);

        cout<<"Choose a license to add:";
        cin>>licenseChoice;
        licenseChoice = licenseChoice > 6 || licenseChoice < 1 ? 1 : licenseChoice;
        string licenseChoiceStr = getLicense(licenseChoice, licenses);
        cout<<"Choosing "<<licenseChoice<<" license"<<endl;

        cout<<"Adding license to metadata"<<endl;

        meta.SetProperty(kXMP_NS_XMP_Rights, "WebStatement", licenses[licenseChoiceStr], 0);

        myFile.PutXMP(meta);

        myFile.CloseFile();

        ok = myFile.OpenFile(filename, kXMP_UnknownFile, opts);

        // If the file still cannot be opened
        if(!ok) {
            cout<<"Unable to open "<<filename<<endl;

            // Terminate the toolkit
            SXMPFiles::Terminate();
            SXMPMeta::Terminate();

            return -1;
        }

        myFile.GetXMP(&meta);

        cout<<"Updated XMP content..."<<endl<<endl;

        cout<<"------Updated metadata------"<<endl;

        displayXMP(&meta);
    }
    catch(XMP_Error & e) {
        cout<<"ERROR: "<<e.GetErrMsg()<<endl;
    }

    // Terminate the toolkit
    SXMPFiles::Terminate();
    SXMPMeta::Terminate();

    return 0;
}
