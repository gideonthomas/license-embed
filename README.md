license-embed
=============

A C++ proof-of-concept library that offers a command-line utility to embed a Creative Commons license(URL) into image XMP metadata. This library uses [Adobe's XMP Toolkit SDK](http://www.adobe.com/devnet/xmp.html) to implement metadata manipulation of an image.

## Usage
Firstly, clone the repository:
```
$ git clone https://github.com/gideonthomas/license-embed.git
```


To use the library, first you have to build Adobe's XMP Toolkit.

### Mac OSX - Build XMP Toolkit
1) First install the [cmake](http://www.cmake.org/download/) utility for Mac OSX. You will need both the application and the command line interface tool.

To install the CLI tool:
```
$ brew update
$ brew install cmake
```

Then, place the CMake.app file into the `XMP-Toolkit/tools/cmake` folder (create it if it does not exist).

2) Run the shell script `XMP-Toolkit/build/GenerateXMPToolkitSDK_mac.sh` and choose the appropriate platform for installation (a Mac-based build). You will most likely want to generate a "static" version of the library.

3) Open the generated project file in XCode. The file will be located in a path similar to `XMP-Toolkit/build/xcode/static/intel/XMPSDKToolkitSDK.xcodeproj`.

4) In the menu in XCode, select <b>Product > Build</b>. Ensure that the build was successful.


Now that you have built the Toolkit, you need to build the library itself.

### Mac OSX - Build license-embed library
1) Run the shell script `XMP-Toolkit/samples/build/GenerateXMPToolkitSDK_mac.sh` and choose the appropriate platform (Mac OSX).

2) Open the generated project file in XCode. The file will be located in a path similar to `XMP-Toolkit/samples/build/intel/macintosh/XMPSDKToolkitSample.xcodeproj`

3) In the menu in XCode, select <b>Product > Build</b>. Ensure that the build was successful.


The library should now be ready to use. It will be located int the `XMP-Toolkit/samples/target` folder.

Simply run the MyModifyXMP file and provide a pathname and follow the instructions.
```
$ ./MyModifyXMP path/to/file/relative/to/MyModifyXMP
```
