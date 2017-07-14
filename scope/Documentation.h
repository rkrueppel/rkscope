/** @file Documentation.h This file has all the extra documentation in it. */

/** @mainpage Scope
Last documentation update: 2015-02-15
\section iiintro Scope, a two-photon microscope control software with multi-area capabilities
###Scope's features###
- Frame scanning (sawtooth, bidirectional, plane hopper) with multiple channels in multiple areas
- Resonance scanning
- Analog PMT signal integration (with/without FPGA), photon counting (with FPGA), digital and analog demultiplexing (with FPGA)
- Acquires stacks with a motorized z stage or a fast z device (e.g. an electrically tunable lens or a piezo drive)
- Acquires time series, with trial-to-trial plane hopping, with external triggering, repeats, gated by an external TTL signal, etc.
- Displays overlays of several channels, the current scale, current pixel values, adjustable histograms etc.
- A labbook logging all your recordings and to note down your comments
- The flexibility to use different kinds of hardware configurations, from a cheap one-card-does it all solution to FPGA based multi-area analog demultiplexing.

###Contact###
Roland Krueppel mailto:krueppel@hifo.uzh.ch \n
Jerry Chen mailto:jerry.chen@hifo.uzh.ch

###Copyright###
Copyright: 2012-2015 by Roland Krueppel, Sievi Lombriser, Jerry Chen - The Helmchen lab, University of Zurich, http://www.hifo.uzh.ch/research/helmchen.html

###Download###
Clone via Git from http://sourceforge.net/projects/rkscope (email me for access to the code). There are several branches in the repository:
resonance/master: most current branch, regularly updated by the Helmchen lab team\n
roland: my personal branch, implementing some long-term conceptual changes and adding stuff from the resonance branch (mostly untested)\n
pre_resonance: more or less stable, before resonance code and additional fixes were added\n
Please post feature requests and bugs as a ticket at https://sourceforge.net/p/rkscope/tickets/

###Scope supports###
- National Instruments [%DAQmx](http://www.ni.com/dataacquisition/nidaqmx) (tested: [NI-6259](http://sine.ni.com/nips/cds/view/p/lang/en/nid/14129), [NI-6229](http://sine.ni.com/nips/cds/view/p/lang/en/nid/14137))
- National Instruments [FPGA](http://www.ni.com/fpga-hardware/) (tested: [NI PXIe-7962R](http://sine.ni.com/nips/cds/view/p/lang/en/nid/208166))
- National Instruments [FlexRIO](http://www.ni.com/flexrio/) (tested: [NI 6587](http://sine.ni.com/nips/cds/view/p/lang/en/nid/209270), [NI 5771](http://sine.ni.com/nips/cds/view/p/lang/en/nid/210735), [NI 5751](http://sine.ni.com/nips/cds/view/p/lang/en/nid/209099))
- Galvanometric scanners, both regular (most models work out of the box) and resonance (tested: [Cambridge CRS 8 kHz](http://www.cambridgetechnology.com/index.php?option=com_content&view=article&id=103:resonant-scanners&catid=57:resonant-scanners&Itemid=100))
- [Galil](http://www.galilmc.com) motion controllers (tested: [DMC-21x3](http://www.galilmc.com/products/dmc-21x3.php))
- [Standa](http://www.standa.lt) motion controllers (tested: [8SMC1-USBhF](http://www.standa.lt/products/catalog/motorised_positioners?item=175))
- [Sutter ](http://www.sutter.com) motion controllers (test in progress: [MP-285](http://www.sutter.com/products/product_sheets/MP-285.html))
- 32/64 bit builds (64 bit not thoroughly tested)

###Reminder###
I am by no means a professional programmer. Although I have several years of experience there is no guarantee that the designs and
algorithms used in Scope are really good solutions. So, good luck. Roland

###Please note###
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

/** @page building Build Scope
\tableofcontents

\section needs You need
- Microsoft Visual Studio 2012 (Professional or better, it could work with Express, but getting WTL to work there needs some fiddling).
Get it e.g. from MSDNAA (Link for University Zurich https://www.uzh.ch/cmsssl/id/dl/sw/spezliz/MSDNAA.html).
Get Update 2 from http://www.microsoft.com/en-us/download/details.aspx?id=38188
- Windows Template Library (download WTL 8.1 at http://sourceforge.net/projects/wtl/)
- Boost libraries (1.53.0, download at http://www.boost.org and build yourself)
- National Instruments %DAQmx (make sure you install support for ANSI C)
- For National Instruments FPGA support, LabView FPGA Module 2012, Xilinx Tools, FPGA C API
- For Galil/Feinmess controller support, download STL Library C++ at http://www.galilmc.com/support/software-downloads.php). Also the GalilTools are very useful.
- For Standa controller/stage support, MicroSMC or MicroSMCx64 (download C Development Kit at http://www.standa.lt/products/catalog/motorised_positioners?item=175&prod=microstep_driver_usb_interface
- exiftool.exe, download at http://www.sno.phy.queensu.ca/~phil/exiftool/

\subsection Optional
- Visual Studio Tools for Git (highly recommended for pushing/fetching code to sourceforge.com directly from Visual Studio, download at http://visualstudiogallery.msdn.microsoft.com/abafc7d6-dcaa-40f4-8a5e-d6724bdb980c)
- Direct2D debug layer (integrated in Windows 8, for Windows 7 download at http://archive.msdn.microsoft.com/Direct2DTools)
- Doxygen to build the documentation from the source code comments (download at http://www.stack.nl/~dimitri/doxygen/download.html)
- Microsoft XML notepad to edit xml configuration files (download at http://www.microsoft.com/en-us/download/details.aspx?id=7973)

\section prep Getting ready
\subsection wtl Install WTL
Just copy everything into e.g. C:\\Program Files\\Windows Template Library. 

\subsection vs Prepare Visual Studio
Install Visual Studio 2012, then install Update 2. Install Git Tools. Git should be automatically installed, too.\n
In Visual Studio you have to add WTL to the general include directories: While any solution is open go to View->Property Manager, in Debug|Win32 and Release|Win32
-> Microsoft.Cpp.Win32.user -> VC++ Directories -> Include directories add C:\\Program Files\\Windows Template Library\\include.\n
Clone sourceforge repository `https://git.code.sf.net/p/rkscope/code` to a local directory.\n

\subsection boost Install Boost
Copy everything into e.g. c:\\Temp\\boost. Open a command prompt, go to that directory and
execute 'bootstrap'. Then `b2 --toolset=msvc --build-type=complete stage`. Detailed information in the boost documenation. After
compile is finished (can take very long...) copy everything into e.g. "C:\\Program Files\\Boost\\boost_1_53_0".\n
Again, in Visual Studio add this path to the general include directories: While any solution is open go to View->Property Manager, in Debug|Win32 and Release|Win32
-> Microsoft.Cpp.Win32.user -> VC++ Directories -> Include directories add "C:\\Program Files\\Boost\\boost_1_53_0"

For a 64 bit build of boost invoke `b2 --toolset=msvc address-model=64 --build-type=complete stage`.

\subsection ni Install National Instruments stuff
Just install, now further configuration needed.

\subsection galil Install Galil/Feinmess stuff
Extract the 32bit libraries for Visual Studio 2012 into 'scope/devices/xyz/galil'. For 64bit extract those libraries into 'scope/devices/xyz/galil/x64'. Later copy 
Galil2.dll (from debug&release subdirectories) into the respective Debug&Release directories where your Scope.exe is built in. In that way you can run scope.exe from Visual
 Studio, otherwise it complains that it cannot find the Galil dlls. Be careful not to confuse debug and release dlls as this can lead to very confusing errors!

\subsection standa Install Standa stuff
Install MicroSMC (32bit for 32bit builds of Scope).

\subsection doxygen Configure Doxygen
Add Doxgen under Tools-External Tools... (Name: "Doxygen", Command: "C:\Program Files\doxygen\bin\doxygen.exe", Arguments: "$(SolutionDir)\documentation\default.doxygen", Initial directory:
"$(SolutionDir)", check "Use output window"). Now, with a click on "Doxygen" you can build the html documentation.

\section build Build Scope
- Download Scope and copy everything into e.g. C:\\Scope
- Open the scope.sln file. Check at Project->Properties->C++->General that the 'Additional include directories' match the ones
on your system. Also Check Linker->General->Additional library directories and Linker->Input->Additional Dependencies. Do this
for Debug and Release configuration. You may have to adapt the path to the git.exe in lastgitcommit.bat.

Now you have to create some local files that are not part of the Scope Git repository:
- Copy ScopeDefinesExamples.h in the scope folder to ScopeDefines.h. You need this later in (@ref configuration).
- Create a text file "version.txt" in the scope folder (this keeps track of the Git commits) and write something into.
- Create a text file "version.h" in the scope folder and write something into it. Both files will be filled by lastgitcommit.bat when
you first compile Scope.

\subsection exiftool Copy exiftool
Scope calls the exiftool.exe to fix TIF tags of saved files. Obviously it cannot be included in the sources.
Copy exiftool.exe (rename if necessary) to scope/tools/ (create this folder if necessary).

\subsection galilinst Copy Galil libraries
For linking to the Galil libraries, copy them to devices/xyz/galil/debug/Galil2.lib and Galil2.pdb and devices/xyz/galil/release/Galil2.lib
To run Scope from Visual Studio, Galil2.dll has to be in the same directory as scope.exe. Thus copy release/debug versions to
scope/Release and scope/Debug.

*/

/** @page configuration Configure Scope
\tableofcontents

\section intro Introduction
The hardware you use is selected at compile time via defines in ScopeDefines.h.

Configuration of these hardware components is loaded on program start from an xml file. The same type of file, filled with
the parameters set during imaging acquisition, is saved together with any image you record. I.e. parameters like Framerate_Hz
which are skipped during xml load during startup are then filled with the correct values.\n
I recommend to build Scope for your hardware, run it with whatever config and then choose File->Save default configuration. This is then a default configuration 
file for your hardware which you can adapt to your needs.

All these parameters are inside scope::parameters (have a look there, you find a lot of useful information).
Scope uses an instance of this internally, e.g. if you change the
zoom slider inside Scope the corresponding value inside scope::ScopeController::GuiParameters is changed automatically (by the
scope::ScopeValue magic). This way Scope always has a consistent parameter set (which can easily be saved as an xml).

\section defines ScopeDefines.h
See ScopeDefinesExample.h. Copy this file using Windows Explorer, not Visual Studio, to ScopeDefines.h. The ScopeDefines.h is excluded from Git so you can adapt it
to your needs. In there you basically define (at compile time) which types of hardware you use in your setup.
In the configuration xml file you then configure this hardware (at run time).

As a help against misconfigurations some basic checks are done during compile time in scope::CheckScopeDefines().

\section xmlconfig XML config file
All parameters are derived from scope::parameters::Base. By using Boost's property_tree library parameters can be saved to and loaded
from XML files. Look at scope::parameters for details. On start Scope tries to load a "default.xml" in its directory, if that does not
exists it opens a dialog in which you can choose an xml xonfig file. You can easily edit the xml file with the Microsoft XML notepad
([download](http://www.microsoft.com/en-us/download/details.aspx?id=7973)) or with an editor of your choice.

\section Examples

\subpage daqmxexample

\subpage daqmxexample2

\subpage fpgaphotoncountexample

\subpage fpgaresonance

\subpage fpgadigitaldemultiplexer

\subpage fpgaanalogintegrator

\subpage fpgaanalogdemultiplexer

\section tips Tips and tricks
- If you are using an FPGA for PMT input it should generate a sample clock. Make sure your Outputs are configured for External
sampling clock and the correct line to receive it.
- If you are using multiple areas, make sure both areas use different devices.
- If you are using an FPGA for PMT input make sure you select not oversampling!
*/

/** @page daqmxexample Example config for a one-area DAQmx system using one NI-6259 card
This is the most simple and cheapest configuration. The same card does analog output and analog input. We use the 6259 as a workhorse for this, many other cards
probably also work.

Copy ScopeDefinesExamples.h (which is in the repository) to ScopeDefine.h by using Windows Explorer (not Visual Studio!). Git ignores ScopeDefines.h
so that you can adapt this to your needs without messing up for other people. Now choose the following defines in ScopeDefines.h
- \#define SCOPE_NAREAS				1
- \#define SCOPE_USE_SIMPLEDAQMX_OUTPUTS
- \#define SCOPE_USE_DAQMX_INPUTS
and xyz hardware as you want it e.g. \#define SCOPE_USE_NOXYFPUSTAGE and \#define SCOPE_USE_GALIL_XYZSTAGE. Then compile your scope.exe.

Check http://sine.ni.com/nips/cds/view/p/lang/en/nid/14129 for details of the 6259 card.

Now to the xml file (remember: you cann find all the parameters here in scope::parameters with detailed descriptions!):
~~~~~
<?xml version="1.0" encoding="utf-8"?>
<scope>
	<Date>2013-04-03</Date>
	<Time>10-20-54</Time>
	<Comment></Comment>
	<StartInputsFirst>true</StartInputsFirst>
	<CommonMasterTrigger>/PXI-6259_0/ao/StartTrigger</CommonMasterTrigger>
~~~~~
A fundamental parameters is [StartInputsFirst](@ref scope::parameters::Scope::startinputsfirst), it determines if input tasks are started first (and then outputs) or vice versa. For
a %DAQmx system start inputs first and use an analog output task as mastertrigger for synchronizing all tasks
(see scope::OutputsDAQmx) for more details.\n
The [CommonMasterTrigger](@ref scope::parameters::Scope::commontrigger) determines which signal all tasks wait for to start synchronously. Here we would start the inputs first, then the outputs with output of area 0 last.
Since the StartTrigger of area 0 is the common master trigger, all tasks wait for it and start synchronously when area 0 output task is started. Since
this configuration only has one area this does not really matter.
~~~~~
	<MasterFOVSizeX_um>3000</MasterFOVSizeX_um>
	<MasterFOVSizeY_um>3000</MasterFOVSizeY_um>
~~~~~
This is the [maximum FOV](@ref scope::parameters::Scope::masterfovsizex) you can reach with the current scanner configuration (see below). It is used for a little diagram showing the current FOV (with zoom and offset)
with respect to the maximum FOV. In a multi-area system it also shows the FPU movement of the single areas FOVs. There the MasterFOVSize is limited only by the
objective performance and the FPU stage travel range. For a one-area system just use BaseMicronPerPixel * 256 (see below).
~~~~~
<area0>
    <Area>0</Area>
	<IsSlaveArea>false</IsSlaveArea>
    <Linerate_Hz>1</Linerate_Hz>
    <Framerate_Hz>1</Framerate_Hz>
	<Frametime_s>1</Frametime_s>
    <ScanMode>Sawtooth</ScanMode>
    <BaseMicronPerPixelX>3.1</BaseMicronPerPixelX>
    <BaseMicronPerPixelY>3.1</BaseMicronPerPixelY>
    <MicronPerPixelX>0.1</MicronPerPixelX>
    <MicronPerPixelY>0.1</MicronPerPixelY>
~~~~~
This is area number 0 and it is a master area. For multiarea configurations these parameters become important.\n
Linerate_Hz, Framerate_Hz, ScanMode, and MicronPerPixelX/Y are filled with real values in the xml file that is saved
together with acquired images. Here they are just placeholders.\n
However, important now is the [BaseMicronPerPixel](@ref scope::parameters::Area::basemicronperpixelx) value, this is how many microns is one pixel at zoom 1 and 256x256 pixel resolution. Edit this
values after you took some first calibration images.\n
Now it gets interesting...
~~~~~
    <daq>
      <inputs>
        <Channels>2</Channels>
        <Oversampling>true</Oversampling>
~~~~~
You want to acquire two channels and want to use [oversampling](@ref scope::parameters::Inputs::oversampling), i.e. the acquisition runs at the maximum rate the NI-6259 card allows,
but for every pixel several samples (pixeltime divided by device's minimum sample time) are averaged. Also the output runs at the lower pixel rate.
~~~~~
        <Uint16Range>firsthalf</Uint16Range>
~~~~~
The NI-6259 card has an [input range](@ref scope::parameters::Inputs::rangetype) of -X Volt - +X Volt (see below). Your PMT signals only go in the negative direction. We are
reading samples from the card as unsigned integers (0..65535). Negative signals are mapped to 0..32766 with 0 V beeing 0. Positive
signals are mapped from 32767...65535 with 0V beeing 32767. To avoid display of the unused values choose ['firsthalf'](@ref scope::Uint16RangeHelper) here. Note,
that still full 16 bit images are saved.
~~~~~
        <ChannelsString>PXI-6259_0/ai0:1</ChannelsString>
~~~~~
These are the two input channels, adapt to your device name.
~~~~~
        <Range_V>0.5</Range_V>
~~~~~
Choose the [input range](@ref scope::parameters::InputsDAQmx::range) (changes digitizer settings on the 6259 card), here -0.5V..+0.5V. Adapt if your PMT/preamp output is different.
~~~~~
        <DAQTimingMethod>ReferenceClock</DAQTimingMethod>
        <ReferenceClockSource>PXI_Clk10</ReferenceClockSource>
        <ReferenceClockRate_Hz>10000000</ReferenceClockRate_Hz>
~~~~~
Since the input and output  tasks run with different rates (the input it oversampling at maximum sampling rate, the output runs at
pixel rate) both tasks can not use the same clock. However, both tasks still need to be synchronized. NI recommends to use the
PXI 10MHz backplane clock (from the PXI chassis) as a reference for both tasks. Check scope::InputsDAQmx for more information. Be
careful: If you do not use the [ReferenceClock](@ref scope::DaqTimingHelper) timing method you still have to provide a valid ReferenceClockRate of e.g. the onboard
clock, since this determines the possible sampling rates of your device (which Scope has to calculate). See DAQmx::PredictSampleRate
and the references there for details.
~~~~~
        <ExternalClockSource>/PXI-6259_0/PXI_Trig1</ExternalClockSource>
~~~~~
If we had choosen ['External'](@ref scope::DaqTimingHelper) as timing method, e.g. for FPGA generated sample clock, this parameter is important. Here it is only a dummy.
~~~~~
        <MaxRateAggregate_Hz>1000000</MaxRateAggregate_Hz>
        <Sampling>NonSimultaneousSampling</Sampling>
	</inputs>
~~~~~
These parameters are important to calculate the maximum sampling rate and the allowed pixel rates. ['MaxRateAggregate_Hz'](@ref scope::parameters::InputsDAQmx::maxrateaggregate) is the
maximum sampling rate when acquiring from both (!) channels, it is device specific. See scope::parameters::InputsDAQmx::MinimumPixeltime
and scope::parameters::InputsDAQmx::CoercedPixeltime for details. The 6259 card does [non-simultaneous sampling](@ref scope::DaqSamplingHelper), by the way.
~~~~~
	  <outputs>
		<ChannelsString>PXI-6259_0/ao0:3</ChannelsString>
        <OutputRange_V>5</OutputRange_V>
        
~~~~~
The four output channels (for x-scanner, y-scanner, fast z axis, and Pockels cell). Possible [output ranges](@ref scope::parameters::Outputs::range) for the NI-6259 are
+-5V and +-10V, since we do not need to drive any output with >5V we choose 5 here for better resolution.
~~~~~
        <DAQTimingMethod>ReferenceClock</DAQTimingMethod>
        <MinimumPixelTime_us>0.8</MinimumPixelTime_us>
        <ReferenceClockSource>PXI_Clk10</ReferenceClockSource>
        <ReferenceClockRate_Hz>1e+007</ReferenceClockRate_Hz>
        <ExternalClockSource>/PXI-6259_0/PXI_Trig1</ExternalClockSource>
~~~~~
Same story as for the inputs, make sure these values are exactly the same as in the input section to ensure
proper synchronization of inputs and outputs. Check scope::OutputsDAQmx. The [MinimumPixelTime_us](@ref scope::parameters::OutputsDAQmx::minimumpixeltime) is the minimal output sampling time the
device supports for 4 output channels. For the 6259 it is 1.25 MSamples/second, corresponding to 0.8us.
~~~~~
        <MinOutputScanner_V>-2</MinOutputScanner_V>
        <MaxOutputScanner_V>2</MaxOutputScanner_V>
        <MinOutputPockels_Fraction>0</MinOutputPockels_Fraction>
        <MaxOutputPockels_Fraction>2</MaxOutputPockels_Fraction>
~~~~~
The 'Range_V' before was the output range setting for the digital to analog conversion. These ranges here set the minimum/maximum
voltages we want to generate. Values here depend on your scanners/Pockels cell driver. The ranges and more details for the
fast z axis are configured further down.
~~~~~
        <ExportPixelClockTerminal>/PXI-6259_0/PFI12</ExportPixelClockTerminal>
~~~~~
The pixel clock is exported to an output terminal for your pleasure.
~~~~~
      </outputs>
      <ShutterLine>PXI-6259_0/port0/line0</ShutterLine>
~~~~~
The digital output line connected to the shutter. Shutter should be closed on low state and open on high.
~~~~~
      <Pixeltime_us>6</Pixeltime_us>
      <Scannerdelay_us>0</Scannerdelay_us>
      <Averages>1</Averages>
~~~~~
The initial pixeltime, scannerdelay, and average setting on program start.
~~~~~
      <RequestedFrames>1</RequestedFrames>
    </daq>
~~~~~
Only a dummy now, filled for the xml acompanying saved images.
~~~~~
    <framesawtooth>
      <XResolution_Pixel>256</XResolution_Pixel>
      <YResolution_Pixel>256</YResolution_Pixel>
      <Zoom>1</Zoom>
      <XOffset_Fraction>0</XOffset_Fraction>
      <YOffset_Fraction>0</YOffset_Fraction>
      <FastZ_um>0</FastZ_um>
      <Pockels_Fraction>0</Pockels_Fraction>
      <XRotation_Degree>0</XRotation_Degree>
      <YRotation_Degree>0</YRotation_Degree>
      <presets>
        <preset0>
          <Name>512x512</Name>
          <Pixeltime_us>6</Pixeltime_us>
          <Scannerdelay_us>-100</Scannerdelay_us>
          <Averages>1</Averages>
          <XResolution_Pixel>512</XResolution_Pixel>
          <YResolution_Pixel>512</YResolution_Pixel>
          <XCutoff_Fraction>0.1</XCutoff_Fraction>
          <YCutoff_Fraction>0.1</YCutoff_Fraction>
          <XRetrace_Fraction>0.2</XRetrace_Fraction>
          <YRetrace_Fraction>0.05</YRetrace_Fraction>
        </preset0>
      </presets>
      <XCutoff_Fraction>0.1</XCutoff_Fraction>
      <YCutoff_Fraction>0.1</YCutoff_Fraction>
      <XRetrace_Fraction>0.1</XRetrace_Fraction>
      <YRetrace_Fraction>0.1</YRetrace_Fraction>
    </framesawtooth>
~~~~~
Initial settings for sawtooth framescanning on program start. Important in xml files acompanying a saved image (Note: in
'area0-ScanMode' you can see which type of scan was used to acquire that image).
Have a look at scope::parameters::ScannerVectorFrameBasic and scope::parameters::ScannerVectorFrameSaw for further information.
Check also scope::PixelmapperFrameSaw to see how an image is assembled depending on these parameters. If have put in a
preset as an example. If you save presets in Scope (and save the parameter file afterwards or use one that was saved
with an image) more stuff is put in here. See scope::parameters::ScannerVectorFrameSaw::Preset.
~~~~~
    <framebidirectional>
      <XResolution_Pixel>256</XResolution_Pixel>
      <YResolution_Pixel>256</YResolution_Pixel>
      <Zoom>1</Zoom>
      <XOffset_Fraction>0</XOffset_Fraction>
      <YOffset_Fraction>0</YOffset_Fraction>
      <FastZ_um>0</FastZ_um>
      <Pockels_Fraction>0</Pockels_Fraction>
      <XRotation_Degree>0</XRotation_Degree>
      <YRotation_Degree>0</YRotation_Degree>
      <presets />
      <XTurning_Fraction>0.1</XTurning_Fraction>
      <YTurning_Fraction>0.1</YTurning_Fraction>
    </framebidirectional>
~~~~~
Initial settings for bidirectional framescanning on program start. This is how it looks without presets.
Have a look at scope::parameters::ScannerVectorFrameBiDi for further information. Check also scope::PixelmapperFrameBiDi
to see how an image is assembled depending on these parameters.
~~~~~
    <frameplanehopper>
      <XResolution>256</XResolution>
      <YResolution>256</YResolution>
      <Zoom>1</Zoom>
      <XOffset_Fraction>0</XOffset_Fraction>
      <YOffset_Fraction>0</YOffset_Fraction>
      <FastZ_um>0</FastZ_um>
      <Pockels_Fraction>0</Pockels_Fraction>
      <XRotation_Degree>0</XRotation_Degree>
      <YRotation_Degree>0</YRotation_Degree>
      <presets />
    </frameplanehopper>
~~~~~
Initial settings for plane hopper framescanning on program start. Have a look at scope::parameters::ScannerVectorFramePlaneHopper
for further information. Note that the plane hopping is not fully implemented yet!
~~~~~
    <fpuzstage>
	  <Position_um>0</Position_um>
      <MinOutput_V>0</MinOutput_V>
      <MaxOutput_V>4</MaxOutput_V>
      <Voltage_V>0</Voltage_V>
      <CalibrationFile>None</CalibrationFile>
    </fpuzstage>
~~~~~
This is the configuration for the fast z axis, which is controlled by a voltage, e.g. en ETL (in a multiarea system every
FPU/area would have one). This output is generated at the third channel specified at 'outputs-ChannelsString' above.
Check scope::FastZControl for details on the calibration file etc.
~~~~~
    <fpuxystage>
      <XPosition_um>0</XPosition_um>
      <YPosition_um>0</YPosition_um>
      <PollInterval_ms>1000</PollInterval_ms>
      <StepSizeForButtons_um>10</StepSizeForButtons_um>
    </fpuxystage>
  </area0>
~~~~~
This is the configuration for the xy-stage of the FPU of a multiarea system. Since we have here a single area system this is
basically only a placeholder.
~~~~~
  <storage>
    <Autosave>true</Autosave>
    <SaveLive>false</SaveLive>
    <Basename>test</Basename>
    <Folder>C:\ScopeData\</Folder>
    <SaveParameters>true</SaveParameters>
    <UseTIFFTags>true</UseTIFFTags>
	<CompressTIFF>true</CompressTIFF>
  </storage>
~~~~~
'Autosave' determines if stacks, timeseries, and single images are automatically saved. Note: If you are not autosaving you
have no method of saving what was acquired! Set to false only for debugging.\n
If you set 'SaveLive' to true, also continuous/live scans are saved (like a timeseries). Also only interesting for debug
purposes.\n
Files are saved into 'Folder' plus subfolder with current date. Filenames are composed as e.g. 'test_A0_Ch1_0002.tif', test is
the 'Basename', 'A0' means area 0, 'Ch1' means channel 1, and '0002' is a sequential number (for every saved image).
'SaveParameters' determines if an xml file (basically the same as the one I am describing here, filled with all the parameters
with which the image was acquire) is saved together with each image. 'true' is highly recommended. With 'UseTIFFTags' Scope
writes the image scaling as TIFF tags in the image, in a format that ImageJ recognises. I.e. inside ImageJ the correct scaling 
(given you have set BaseMicronPerPixel above) is automatically displayed. Scope uses 'exiftool.exe', which has to be
present in the 'scope/tools' folder to write TIFF flags.\n
If you want to write compressed tiffs (using the ZIP algorithm) set CompressTIFF to true. This setting has almost no performance overhead, use it to save space.\n
See scope::StorageController for details.
~~~~~
  <stack>
    <startat>
      <ZPosition_um>0</ZPosition_um>
      <Pockels_Fraction>0</Pockels_Fraction>
    </startat>
    <stopat>
      <ZPosition_um>0</ZPosition_um>
      <Pockels_Fraction>0</Pockels_Fraction>
    </stopat>
    <Spacing_um>1</Spacing_um>
    <ZDeviceType>ZStage</ZDeviceType>
	<OverallTime_s>1</OverallTime_s>
  </stack>
~~~~~
This is filled with useful information in the xml accompanying a saved stack.\n
See scope::ScopeController::ScopeControllerImpl::RunStack for details.
~~~~~
  <timeseries>
    <Frames>1</Frames>
    <Totaltime_s>2.35011</Totaltime_s>
    <Triggered>false</Triggered>
    <AllTriggered>false</AllTriggered>
    <Triggerchannel>/PXI-6259_0/PFI0</Triggerchannel>
    <Repeats>1</Repeats>
    <BetweenRepeats_s>2.54986</BetweenRepeats_s>
    <OverallTime_s>2.35011</OverallTime_s>
  </timeseries>
~~~~~
This is filled with useful information in the xml accompanying a saved timeseries. The only parameters you have to fill in
is the 'Triggerchannel' you want to use for external triggering. The others parameters are editable/calculated inside Scope.\n
See scope::ScopeController::ScopeControllerImpl::RunTimeseries for details.
~~~~~
  <stage>
    <XPosition_um>0</XPosition_um>
    <YPosition_um>0</YPosition_um>
    <ZPosition_um>0</ZPosition_um>
    <PollInterval_ms>1000</PollInterval_ms>
    <RS232Connection>COM7 19200</RS232Connection>
    <XEncoderCountsPerMicrometer>2</XEncoderCountsPerMicrometer>
    <YEncoderCountsPerMicrometer>2</YEncoderCountsPerMicrometer>
    <ZEncoderCountsPerMicrometer>60</ZEncoderCountsPerMicrometer>
  </stage>
~~~~~
Parameters for the Galil stage, this looks slightly different for another stage type, e.g. a Sutter stage.
~~~~~
	<stimulation>
		<Channel>PXI-6259_0/port1/line0</Channel>
		<Timingsource>/PXI-6259_0/ao/SampleClock</Timingsource>
~~~~~
Choose a digital output line here. Attention, these lines provide only very little current, depending on your stimulation hardware this may not be enough. Ask the
electronics specialist of your trust for help.\n
Using the sample clock of the analog output task on the same card ensures synchrony between scanning and stimulation. The stimulation task runs thus at the pixel speed.
~~~~~
		<Duration_s>1</Duration_s>
		<Enable>false</Enable>
		<OffTime_s>0.5</OffTime_s>
		<Onset_s>0</Onset_s>
		<OnTime_s>0.5</OnTime_s>
	</stimulation>
~~~~~
This is filled with information from inside Scope.
~~~~~
	<frames />
</scope>
~~~~~
If you click on "Tools->Save window positions" in Scope the current positionts of all its windows are saved. If you then choose "File->Save parameters..." these
window positions are saved in here in the parameters xml file.\n\n
That's it!
*/

/** @page daqmxexample2 Example config for a one-area DAQmx system using two NI-6259 cards
The drawback in the previous configuration is that one card generates the output signal for both x and y scanners. Since on one %DAQmx card you can only have one
output task with one sample clock, you have to generate a whole frame as signals although every line and every column is the same.
Using two cards has two main advantages:
- The size of the internal buffer of %DAQmx is limited, near 3000x3000 pixels is the limit and you get weird allocation errors. By using two cards, one will
generate the x scanner (and Pockels) signals with the pixel clock. The other card generates the y scanner (and fast z) signals with the line clock. For a
3000x3000 you only have to have two buffers with 3000 samples each.
- If you change e.g. the Pockels value during live scanning Scope has to rewrite the whole frame into the %DAQmx buffer. Thus you get the updated Pockels value only
in the frame after the frame you clicked on the slider. Using two cards the output buffers are much smaller and (at least for the x and pockels signals) the update
is visible very quickly.

Copy ScopeDefinesExamples.h (which is in the repository) to ScopeDefine.h by using Windows Explorer (not Visual Studio!). Git ignores ScopeDefines.h
so that you can adapt this to your needs without messing up for other people. Now choose the following defines in ScopeDefines.h
- \#define SCOPE_NAREAS				1
- \#define SCOPE_USE_TWOCARDDAQMX_OUTPUTS
- \#define SCOPE_USE_DAQMX_INPUTS
and xyz hardware as you want it e.g. \#define SCOPE_USE_NOXYFPUSTAGE and \#define SCOPE_USE_GALIL_XYZSTAGE. Then compile your scope.exe.

Now to the xml file (remember: you cann find all the parameters here in scope::parameters with detailed descriptions!):
~~~~~
<outputs>
    <OutputRange_V>5</OutputRange_V>
    <MinOutputScanner_V>-3</MinOutputScanner_V>
    <MaxOutputScanner_V>3</MaxOutputScanner_V>
    <MinOutputPockels_V>0</MinOutputPockels_V>
    <MaxOutputPockels_V>2</MaxOutputPockels_V>
    <XPOutString>PXI-6259_0/ao0:1</XPOutString>
    <YZOutString>PXI-6259_1/ao0:1</YZOutString>
    <PixelTimingMethod>ReferenceClock</PixelTimingMethod>
    <LineTimingMethod>ReferenceClock</LineTimingMethod>
    <MinimumPixelTime_us>0.8</MinimumPixelTime_us>
    <PixelReferenceClockSource>PXI_Clk10</PixelReferenceClockSource>
    <LineReferenceClockSource>PXI_Clk10</LineReferenceClockSource>
    <ReferenceClockRate_Hz>1e+007</ReferenceClockRate_Hz>
    <ReferenceClockRate_Hz>1e+007</ReferenceClockRate_Hz>
    <ExternalPixelClockString>/PXI-6259_0/PXI_Trig1</ExternalPixelClockString>
    <ExternalLineClockString>/PXI-6259_1/PXI_Trig2</ExternalLineClockString>
    </outputs>
~~~~~
This is very similar to @ref daqmxexample but here you have to provide two strings with the channels on the two boards. You also have to configure the timing method
for both cards. Ideally you use two identical cards and put the same values for both cards in here. If you use two different cards, make sure the minimum pixel time is
the one from the slower board.

The rest of the xml file is identical to @ref daqmxexample.
*/

/** @page fpgaphotoncountexample Example config for a one-area FPGA/DAQmx system using one NI-6259 card and a NI-7962R FPGA with NI FlexRIO 6587 adapter module (photon counting).
In this configuration the concept is to output scanner/fast z/Pockels signals from a regular 6259 card. PMT signals are pulse
amplified (by e.g. Alphalas BVG-100 amplifier) and converted to LVDS pulses (by e.g. PulseResearchLab comparator and logic level
converter). The LVDS signals are then read in with a very fast (> 1GHz) LVDS digital input adapter card (NI 6587) on an FPGA (e.g. 7962R).
Depending on your imaging conditions this can give superior signal-to-noise performance compared to regular analog integration. But be aware that the apparent signal
levels are very low! Having a pixel time of 6 µs gives you 480 laser pulses per pixel. Ideally for this configuration you have 0-1 photons (better: photoelectrons) per
laser pulse. Then your maximum signal level would be 480.
Higher photon fluxes still lead to an increase in the apparent signal level, since the photon counter vi on the FPGA counts the "high" samples. For higher photon flux
you have several photons per laser pulse, but the signal that reaches the comparator could be a large pulse. This pulse is over the threshold for more samples than a
single photon pulse, but the relationship is non-linear (in whatever complex way). Etc..

Copy ScopeDefinesExamples.h (which is in the repository) to ScopeDefines.h by using Windows Explorer (not Visual Studio!). Git ignores ScopeDefines.h
so that you can adapt this to your needs without messing up for other people. Now choose the following defines in ScopeDefines.h
- \#define SCOPE_NAREAS				1
- \#define SCOPE_USE_SIMPLEDAQMX_OUTPUTS
- \#define SCOPE_USE_FPGAPHOTONCOUNTER_INPUTS
and xyz hardware as you want it e.g. \#define SCOPE_USE_NOXYFPUSTAGE and \#define SCOPE_USE_GALIL_XYZSTAGE. Then compile your scope.exe.

- Check http://sine.ni.com/nips/cds/view/p/lang/en/nid/14129 for details of the 6259 card.
- Check http://sine.ni.com/nips/cds/view/p/lang/en/nid/208166 for detail of the 7962R FPGA
- Check http://sine.ni.com/nips/cds/view/p/lang/en/nid/209270 for details of the 6587 high-speed digital adapter module

Now to the xml file (remember: you can find all the parameters here in scope::parameters with detailed descriptions!):
~~~~~
<?xml version="1.0" encoding="utf-8"?>
<scope>
	<Date>2013-04-03</Date>
	<Time>10-20-54</Time>
	<Comment></Comment>
	<StartInputsFirst>false</StartInputsFirst>
	<CommonMasterTrigger>/PXI-6259_0/ao/StartTrigger</CommonMasterTrigger>
	<MasterFOVSizeX_um>3000</MasterFOVSizeX_um>
	<MasterFOVSizeY_um>3000</MasterFOVSizeY_um>
~~~~~
Since the FPGA provides a sample clock for the 6259 card (via the PXI_Trig1 PXI chassis line), we have to start the outputs task first. 
It waits then for its sample clock which will arrive when we now start the FPGA inputs task. Since there is only one area we can leave
the ao/StartTrigger of the 6259 as CommonMasterTrigger (it does not matter anyhow).
*/

/** @page fpgaresonance Example config for a one-area resonance scanner FPGA/DAQmx system using one NI-6259 card and a NI-7962R FPGA with NI FlexRIO 5751 adapter module (high-speed digitizer)
For theory of operation and details on the hardware and software implementation have a look at the documentation/resonance folder.

Copy ScopeDefinesExamples.h (which is in the repository) to ScopeDefines.h by using Windows Explorer (not Visual Studio!). Git ignores ScopeDefines.h
so that you can adapt this to your needs without messing up for other people. Now choose the following defines in ScopeDefines.h
- \#define SCOPE_NAREAS				1
- \#define SCOPE_USE_RESONANCESCANNER		true
- \#define SCOPE_USE_SIMPLEDAQMX_RESONANCE_OUTPUTS
- \#define SCOPE_USE_FPGARESONANCESCANNER_INPUTS
and xyz hardware as you want it e.g. \#define SCOPE_USE_NOXYFPUSTAGE and \#define SCOPE_USE_GALIL_XYZSTAGE. Then compile your scope.exe.

Now to the xml file (remember: you can find all the parameters here in scope::parameters with detailed descriptions!). The additional (!) configurations here are:
~~~~~
<daq>
<ResonanceFrequency_Hz>7910</ResonanceFrequency_Hz>
~~~~~
Resonance frequency of the resonance scanner used for calculations in the software
~~~~~
	<inputs>
		<AcquisitionClockRate_Hz>40000000</AcquisitionClockRate_Hz>
	</inputs>
~~~~~
This is the sampling rate of the FPGA adapter module, for the 5751 this is 40 MHz.
~~~~~
	<outputs>
	<ZoomChannelsString>PXI-6259_0/port0/line1:2</ZoomChannelsString>
~~~~~
Two digital signals used for the discrete zoom (1, 2, 3, 4) of the resonance scanner
~~~~~
	<ExternalClockSource>/PXI-6259_0/PFI1</ExternalClockSource>
~~~~~
Input for the resonance scanner synchronization signal (from the 5751)
~~~~~
	<SwitchResonanceLine>PXI-6259_0/port0/line3</SwitchResonanceLine>
~~~~~
Output for switching the resonance scanner on and off (not needed)
~~~~~
<frameresonance>
	<XTurning_Fraction>0.1</XTurning_Fraction>
	<SquarePixels>false</SquarePixels>
	<YAspectRatio></YAspectRatio>
~~~~~
Settings for image dimensions and pixel dimensions, respectively
~~~~~
	<WaitAfterEnqueueStorage_Millisecond></WaitAfterEnqueueStorage_Millisecond>
	<WaitAfterEnqueueDisplay_Millisecond></<WaitAfterEnqueueDisplay_Millisecond>
</frameresonance>
~~~~~
Time to wait, quick fix for a problem. See resonance scanner documentation folder for details.
*/

/** @page fpgadigitaldemultiplexer Example config for a two-area nbeam FPGA/DAQmx system using two NI-6259 cards and a NI-7962R FPGA with NI FlexRIO 6587 adapter module (digital demultiplexing).
In this configuration the concept is to have one scanner but two areas. Conceptually, in Scope the first area is the so called master area, the second one the slave area.
That means for the first area you can change resolution, pixel time, scan mode etc - it "owns" the scanner. For the second area you can only change fast z and Pockels.
Thus the first 6259 card outputs scannerxy/fastz/Pockels signals, the second 6259 card outputs only fastz/Pockels. As in the photon counting one-area example PMT pulses
are amplified, converted to LVDS, and read with the NI 6587 card (Channel 1 on IO_1, channel 2 on IO_2). You also have to connect the amplified laser pulses from the
MaiTai back BNC connector to IO_0. The FPGA does the demultiplexing (see Wiki).

Copy ScopeDefinesExamples.h (which is in the repository) to ScopeDefine.h by using Windows Explorer (not Visual Studio!). Git ignores ScopeDefines.h
so that you can adapt this to your needs without messing up for other people. Now choose the following defines in ScopeDefines.h
- \#define SCOPE_NAREAS				2
- \#define SCOPE_USE_SIMPLEDAQMX_OUTPUTS
- \#define SCOPE_USE_FPGADIGITALDEMULTIPLEXER_INPUTS
and xyz hardware as you want it e.g. \#define SCOPE_USE_NOXYFPUSTAGE and \#define SCOPE_USE_GALIL_XYZSTAGE. Then compile your scope.exe.

*/

/** @page fpgaanalogdemultiplexer Example config for a two-area nbeam FPGA/DAQmx system using two NI-6259 cards and a NI-7962R FPGA with NI FlexRIO 5771 adapter module (analog/digital demultiplexing).
In this configuration the concept is to have one scanner but two areas. Conceptually, in Scope the first area is the so called master area, the second one the slave area.
That means for the first area you can change resolution, pixel time, scan mode etc - it "owns" the scanner. For the second area you can only change fast z and Pockels.
Thus the first 6259 card outputs scannerxy/fastz/Pockels signals, the second 6259 card outputs only fastz/Pockels. PMT pulses are amplified and directly connected to
the AI 0 and AI 1 connectors of the 5771. You also have to connect the amplified laser pulses from the
MaiTai back BNC connector to the comparator that generates TTL output pulses and from there to the TRIG input of the 5771. The FPGA does the demultiplexing (see Wiki).

Copy ScopeDefinesExamples.h (which is in the repository) to ScopeDefine.h by using Windows Explorer (not Visual Studio!). Git ignores ScopeDefines.h
so that you can adapt this to your needs without messing up for other people. Now choose the following defines in ScopeDefines.h
- \#define SCOPE_NAREAS				2
- \#define SCOPE_USE_SIMPLEDAQMX_OUTPUTS
- \#define SCOPE_USE_FPGAANALOGDEMULTIPLEXER_INPUTS
and xyz hardware as you want it e.g. \#define SCOPE_USE_NOXYFPUSTAGE and \#define SCOPE_USE_GALIL_XYZSTAGE. Then compile your scope.exe.

*/

/** @page fpgaanalogintegrator Example config for a one area FPGA/DAQmx system using one NI-6259 card and a NI-7962R FPGA with NI FlexRio 5771 adapter module.
In this configuration the adapter module does 1.5GHz sampling and integration of the PMT signals.
Connect PMTs to high gain pulse amplifiers and then to AI0 and AI1 of the adapter.

Copy ScopeDefinesExamples.h (which is in the repository) to ScopeDefine.h by using Windows Explorer (not Visual Studio!). Git ignores ScopeDefines.h
so that you can adapt this to your needs without messing up for other people. Now choose the following defines in ScopeDefines.h
- \#define SCOPE_NAREAS				1
- \#define SCOPE_USE_SIMPLEDAQMX_OUTPUTS
- \#define SCOPE_USE_FPGAANALOGINTEGRATOR_INPUTS
and xyz hardware as you want it e.g. \#define SCOPE_USE_NOXYFPUSTAGE and \#define SCOPE_USE_GALIL_XYZSTAGE. Then compile your scope.exe.

*/

/** @page running Running Scope
\tableofcontents
\section install Installation
After you have compiled a release build scope.exe you can copy it (together with e.g. libgalil2.dll) to a directory of your choosing.

\section Daily usage
After you start Scope you are prompted to choose parameter xml file. Make sure you had tested that file not to **** up your system by running it in
Visual Studio as a debug build.

If you add presets for framescans, make sure to save the parameter file afterwards to store these presets for the next start of Scope.

Using the FPGA the first time you start a scan it may take a couple of seconds to initialize the FPGA. 

\section crash Scope crashes

If Scope crashes it could be that the laser shutter is still open - make sure it is closed before you check out the microscope!

- Have a look at the log file, some error related information may have been saved there. 
- Try to reproduce the error while running Scope under Visual Studio as a debug build.
- Do a dry run, switch off the galvos and have a look at the Galvo command signals with an oscilloscope.
- Open the Windows task manager and have a look at Scope's memory and CPU consumption.
- Open a ticket at https://sourceforge.net/p/rkscope/tickets/ 


*/

/** @page developescope Developing Scope
\tableofcontents

_Remember: Always document your code_

\section git Using git
Git uses a local and a remote repository. The first step is to clone the remote repository to a local one. With the Visual Studio Git plugin, go to Team Explorer (if not
visible: View->Team Explorer) and then to Connect. Choose 'Local Git Repositories-Clone' add 'git.code.sf.net/p/rkscope/code' as remote URL and a local directory (e.g.
'C:\\Scope'). After that 'Scope' should be listed under the local repositories. Right-click and choose open. Please note that you still have to open the Visual Studio
 solution file 'scope.sln' for the project to be loaded into Visual Studio!

After you edited code go to the Team Explorer 'Home' page for Scope. A click on 'Changes' displays the changed files. Enter a commit message and 'Commit' the
file changes to your local repository. A click on 'Commits' shows your recent commit under 'Outgoing Commits'. Click on 'Push' to push the commits from the local to
the remote repository.

To preview what commits have been made in the local repository click 'Fetch'. To load these commits into your local repository click 'Pull'.

For in-depth information about git have a look at e.g. http://git-scm.com/book or http://www.vogella.com/articles/Git/article.html 

\section fixfpga Fixing something in an FPGA VI

If you want to change something small in one of the FPGA VIs, e.g. the AnalogDemultiplexer
- Open that Labview project and do the fix
- Recompile using the build specification. The bitfile will be saved into "C:\\Scope\\fpga_labview\\FPGA bitfiles" (this is hardwired into the build spec, Labview does not
allow relative paths)
- Using the NI FPGA Interface C API Generator to generate a C header file. Copy the .h file and the .lvbitx file into "scope\\devices\\fpga"
- Recompile Scope
- Test and push a commit to Sourceforge

\section parameter Implementing a new parameter

If you want to implement a new parameter, e.g. a boolean for using Pockels cell blanking during flyback:
- Add a scope::ScopeNumber<bool> to scope::parameters::ScannerVectorFrameSaw (if you want to leave this untouched, derive a class and only add the ScopeNumber) as a
 public member. Add it to the constructor, load, save, and SetReadOnlyWhileScanning. If you like you can also add it to scope::parameters::ScannerVectorFrameSaw::Preset, also
 as a public member, to the constructor, load, and save. See the other members how it is done.
- Add a checkbox control to the IDD_FRAMESCAN_SAW_PROPPAGE dialog.
- In scope::gui::CFrameScanSawPage add an scope::gui::CScopeCheckBoxCtrl as member. In the constructor connect it to the appropriate member of
 scope::ScopeController::GuiParameters. In scope::gui::CFrameScanSawPage::OnInitDialog attach it to the dialogs edit control resource.
- Now changes in the dialog show up in scope::ScopeController::GuiParameters and vice versa, also setting the ScopeNumber<bool> to readonly  via SetReadOnlyWhileScanning
 affects the edit control in the dialog.
- adapt the algorithm in scope::ScannerVectorFrameSaw::FillP to use Pockels cell blanking depending on the value of the parameter.

\section scanmode Implementing a new frame scan mode

If you want to add something that changes scope::ScannerVectorFrameSaw a lot, implement your own frame scan mode by:
- deriving your own classes from scope::ScannerVectorFrameSaw (or scope::ScannerVectorFrameBasic) and scope::parameters::ScannerVectorFrameSaw (or Basic).
- Look at the other frame scan modes how this is done.
- Add to scope::ScannerVectorType, the scope::ScannerVectorFrameBasic::Factory function, to scope::ScopeController::ScanModeButtons,
and to parameters::ScannerVectorFrameBasic::Factory
- Connect the scan mode button in scope::ScopeController::ScopeControllerImpl::ScopeControllerImpl
- In scope::ScopeController::ScopeControllerImpl::SetGuiCtrlState add the scan mode button (to switch it off during scanning)
- You then need to derive a class from scope::gui::CFrameScanSawPage (or directly from scope::gui::CFrameScanPage)
- and add its dialog resource. Then adapt scope::gui::CScanModesSettingsPage to show your scan mode and allow switching to it.
- Add your page to scope::gui::CScanSettingsSheet::ChangeScanmode.
- There are diverse places where you will need to include your classes header file, you will easily find it out during compile.

\section device Implementing a new device

If you want to implement support for a new device, eg support for a different type of xyz stage:
- Derive a class from e.g. scope::XYZControl and overwrite the methods you need
- Have a look at scope::XYZControlGalil for how it is done there
- Be sure to implement correct error/exception handling
- Implement or derive a class with the exact same name but in namespace scope::parameters
- In ScopeDefines.h you can then set XYZCONTROL_T to you class name, parameters class will be set accordingly.
- There are diverse places where you will need to include your classes header file, you will easily find it out during compile.

\section newfpga Implementing a new FPGA mode

If you want to implement support for a new mode using an FPGA, e.g. with a new FlexRIO adapter card:
- Implement the FPGA code in Labview. Compile it, create a C API with NI's FPGA C API Generator.
- Add a  new class under devices/fpga handling communication to your FPGA VI. If you want, you can have the stuff regarding only the
 adapter card in an extra class (have a look at scope::FPGAIO6587 and scope::FPGAIO5771). Have a look at e.g. scope::FPGAAnalogDemultiplexer to 
 see how its done.
- Add a new enumerator in scope::InputsTypeHelper in ScopeDatatypes.h.
- Add a new parameter class under scope::parameters in IO.h.
- Add your parameters class at scope::parameters::Inputs::Factory.

\section documentationupload Uploading updated documentation

Although you can always build the newest code documentation using doxygen on your local computer updating Scope's website http://rkscope.sourceforge.net/index.html 
is a little more work. Doing it like this (with the zipped documentation) speeds up the upload tremendously.
- Build the local documentation. Zip the documentation/html folder.
- Using Putty (http://www.chiark.greenend.org.uk/~sgtatham/putty/) log in to sourceforge's remote shell. Settings are: Hostname 'shell.sourceforge.net',
Connections-Data-Auto-login username 'yoursourceforgeusername,rkscope' (yes with the comma, e.g. 'rkrueppel,rkscope'), Connection-SSH-Remote command 'create'
- Once the shell is created (can take a while) do 'cd /home/project-web/rkscope/htdocs' then delete the old documentation stuff in there (rm -R *)
- Then use e.g. WinSCP (http://www.winscp.net) to upload the documentation zip to sourceforge. Settings are: Protocol 'SFTP', Hostname 'web.sourceforge.net', 
User 'yoursourceforgeusername', Environment-Directories-Remote directory '/home/project-web/rkscope/htdocs'
- Again in the remote shell 'unzip html.zip' and then 'rm html.zip'.
- Voila! Point your browser to http://rkscope.sourceforge.net and refresh the page.
*/

/** @page concepts Scope concepts
\tableofcontents

###Scope's features are realized by###
- programming in [C++](http://www.isocpp.org/) using [Microsoft Visual Studio 2012](http://www.microsoft.com/visualstudio/eng)
- using modern C++11 features, [STL](http://www.cplusplus.com/reference/stl/), and [Boost libraries](http://www.boost.org)
- using the lightweight [Windows Template Library](https://sourceforge.net/projects/wtl/) (see scope::gui)
- relying on [Direct2D](http://msdn.microsoft.com/en-us/library/windows/desktop/dd370990%28v=vs.85%29.aspx) for displaying (see d2d)
- using Windows Imaging Components for TIFF saving (see scope::ScopeMultiImageEncoder) and [exiftools.exe](http://www.sno.phy.queensu.ca/~phil/exiftool/) for writing [ImageJ](http://rsbweb.nih.gov/ij/) compatible TIFF tags (see scope::StorageController::StorageControllerImpl::FixTIFFTags).
- A pipeline of 'controllers' for data acquisition (scope::DaqController), assembling images (scope::PipelineController), displaying images and histograms (scope::DisplayController), and storing to disk (scope::StorageController)
- classes for different hardware for sampling PMT input (scope::InputsDAQmx and scope::InputsFPGA), and FPGA classes (scope::FPGADemultiplexer, scope::FPGAPhotonCounterV2)
- classes for different scan modes, until now frame scanning in sawtooth (scope::ScannerVectorFrameSaw) or bidirectional (scope::ScannerVectorFrameBiDi) mode and ETL plane hopping (scope::ScannerVectorFramePlaneHopper)
- the ability to run all these controller for every area in a separate thread
- implementing a custom set of thread-safe values (scope::ScopeValue, scope::ScopeNumber, scope::ScopeString) that can be connected to functions via Boost::signals2
- implementing a custom set of thread-safe controls (scope::gui::CScopeEditCtrl, scope::gui::CScopeSliderCtrl, ...) to use underlying scope::ScopeValue.
- extensive code documentation using [Doxygen](http://www.stack.nl/~dimitri/doxygen/)
- version control through [git](http://git-scm.com/) and hosting at [sourceforge.com](http://www.sourceforge.com)

\section scopevalues ScopeValues
A core component of Scope is the scope::ScopeValue class and its derivatives scope::ScopeNumber and scope::ScopeString. They provide thread-safe access
to a data member. More importantly you can connect functions to these objects that are called when the data member is changed or
when the read/read-write property of the object is changed. ScopeNumbers also provide range checking.

\section scopecontrols ScopeControls
Several classes derived from WTL classes encapsulating dialog controls. They can easily be connected to ScopeValues. Then the scope::ScopeValue
is displayed in the control, edits to the control lead to changes of the scope::ScopeValue and vice versa. Also setting the read/read-write
mode of the scope::ScopeValue disables/enables the control.

\section guiparameters GuiParameters
The controller ScopeController has a static public data member scope::ScopeController::GuiParameters that contains a lot of scope::ScopeValues that describe all
parameters of the current Scope configuration. By connecting ScopeControls to ScopeValues in scope::ScopeController::GuiParameters, this contains the
values that are displayed/can be edited in the GUI.

\section controllers Controllers
A set of classes derived from scope::BaseController, which holds a thread object and several member functions to start/stop
this thread (using an scope::AbortCondition member). All these classes use the pimpl idiom to hide the implementation and shorten compile times.
Each object has its own parameters data member. Derived classes like scope::DaqController mostly overwrite the Run method (which is run
in the objects thread upon invoking the Start method).\n
SynchronizedQueue's provide data flow between the controllers (from scope::DaqController to scope::PipelineController to scope::StorageController and scope::DisplayController).
They send ScopeMessage that contain different data types as cargo member (scope::DaqChunkPtr or scope::ScopeMultiImagePtr after the scope::PipelineController). 

\section io ScopeInputs/ScopeOutputs
These classes encapsulate the communication with the data acquisition hardware. scope::InputsDAQmx and scope::OutputsDAQmx utilize
the National Instruments %DAQmx C-API (through a thin C++ wrapper in the DAQmx namespace). scope::InputsFPGA uses the National
Instuments FPGA C-API.
*/