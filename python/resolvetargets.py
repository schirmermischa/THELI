import sys
import astropy.coordinates
name = sys.argv[1]
coords = astropy.coordinates.get_icrs_coordinates(name)
print (coords.ra.value, coords.dec.value)
