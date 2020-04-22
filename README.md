# THELI v3

THELI is a data processing pipeline for optical, near-infrared and mid-infrared astronomical images. Version 3 is a complete rewrite in C++ / Qt5 (compared to version 2 in Qt3), eliminating a large number of dependencies and overcoming installation issues. Using a hybrid memory/drive data model and full parallelization, v3 offers a vast gain in processing speed. Depending on the hardware and data set, processing speed may increase by factors 2 to 20. THELI v3 also fully scales with the amount of available RAM and CPUs on your machine, and there is still potential for further improvement.

With its own integrated 'iView' FITS browser, advanced processing of astronomical imaging data has never been easier. 

THELI v3 also runs on MacOS.

Even though the current release includes all instruments that were supported in THELI v2, not all of them have been tested. Please contact the author if you encounter data that do not run through the first processing step correctly.

Mid-infrared cameras are currently not supported, but will be implemented on particular request and as soon as THELI v3 has been running stable for a while. If you need mid-IR support, please contact the author with a test data set.

THELI v3 is not yet documented; a set of video tutorials will be made available. Test users familiar with v2 have easily found their way around v3, aided by the much improved user interface.
