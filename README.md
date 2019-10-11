# THELI v3

THELI is a data processing pipeline for optical, near-infrared and mid-infrared astronomical images. Version 3 is a complete rewrite in C++ / Qt5 (compared to version 2 in Qt3), eliminating a large number of dependencies and overcoming installation issues. Using a hybrid memory/drive data model and full parallelization, v3 offers a vast gain in processing speed. It also fully scales with the amount of available RAM and CPUs on your machine. 

With its own integrated 'iView' FITS browser, advanced processing of astronomical imaging data has never been easier. 

THELI v3 should also run on MacOS, but the current beta-release has not been tested on it.

Even though the current beta-release includes all instruments that were supported in THELI v2, this is not yet the case for v3. In fact, only a few instruments have been tested. In principle, all instruments with a single detector should work out of the box. Instruments with multiple detectors should work if their data is stored in MEF FITS format, and if THELI can construct a unique DATE-OBS key for all chips.

Instruments with detectors where the overscan region from multiple readout channels is dividing up the image area are not yet supported (such as the Hamamatsu GMOS CCDs, or SuprimeCam).

Mid-infrared cameras are currently not supported, but support is foreseen as soon as THELI v3 emerges from its beta-testing phase.

