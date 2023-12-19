# trycpp23

**Test Task**

Create application for visual representation of measurement results.

Task description:

1. The application should be window-based with elements of graphical user interface (GUI)

1. Application should be able to load measurement data from files of the following format:

// File Header

\# Pendulum Instruments AB, TimeView32 V1.01          // Organization name, name and version of application this file was created by

\# FREQUENCY A                                        // <a name="__ddelink__1_1106358489"></a>Measurement type

\# Thu Nov 27 12:26:21 2003                           // Measurement start time

\# Measuring time: 800 ns 		Single: Off      // Measurement parameters

\# Input A: Auto, 1Mк, AC, X1, Pos  Filter: Off       // Measurement parameters

\# Input B: Auto, 1Mк, DC, X1, Pos  Common: On        // Measurement parameters

\# Ext.arm: Off                     Ref.osc: Internal // Measurement parameters

\# Hold off: Off                    Statistics: Off   // Measurement parameters

// Measurement data: <timestamp> <value>

0\.0000000000000e+000 1.6394073114428e+004            

3\.0724070450092e-003 1.6395409053595e+004

6\.1448140900184e-003 1.6379824408800e+004

............

All header lines begin with #.  Spacing between fields inside the header lines is not fixed and can be filled by spaces or tabs. Measurement data is represented by <timestamp> <value> pairs of numbers. Timestamps are sorted using ascending order, <value> can have any value. Application should be able to handle large files properly (up to 5 million data points).

If input file has wrong format – user should be presented with corresponding error message.

Samples of files are in SamplesFiles folder of the Test Task archive

1. Application should present measurement data as a 2 D-plot in style “dots connected with lines”. X-axis corresponds to <timestamp> and Y-axis corresponds to <value>. The user should be able to see the whole graph at the same time (without the need to scroll)

1. Additional data representation capabilities are not mandatory and are up to the developer.

1. Result of test task implementation should be presented in the form of source files and resulting binary files.


**Requirements for tools and environment:**

1. Application should work under one of the following OS (your choice):
- Windows
- Linux
2. Should be implemented using С++.
3. It is allowed to use third party libraries for building UI (Qt, MFC, etc), except special components for rendering and visualizing graphs/plots.

**Implementation**

Qt 5.12.12 (qt-opensource-windows-x86-5.12.12.exe)
C++23 (xpack-mingw-w64-gcc-13.2.0-1-win32-x64.zip)
