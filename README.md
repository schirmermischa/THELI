# THELI v3

THELI is a data processing pipeline for astronomical images. Version 3 is a complete rewrite in C++ / Qt5 (compared to version 2 in Qt3), eliminating a large number of dependencies and overcoming installation issues. Using a hybrid memory/drive data model and full parallelization, v3 offers a vast gain in processing speed. It also fully scales with the amount of available RAM and CPUs on your machine. 

With its own integrated 'iView' FITS browser, image data processing has never been easier. THELI v3 should also run on MacOS.

The following dependencies must be met. They are all common use:

Qt5.9.6   https://www.qt.io/

cfitsio   https://heasarc.gsfc.nasa.gov/fitsio/

fftw3     http://fftw.org/

GSL v2.5  https://www.gnu.org/software/gsl/

swarp v2.38   https://www.astromatic.net/software/swarp

scamp v2.7    https://github.com/astromatic/scamp

sextractor v2.19     https://www.astromatic.net/software/sextractor

vizquery.py  http://cds.u-strasbg.fr/resources/doku.php?id=cdsclient

sesame   http://cdsarc.u-strasbg.fr/ftp/pub/sw/cdsclient.tar.gz

