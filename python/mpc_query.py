import sys
from astroquery.mpc import MPC
name=sys.argv[1]
dateobs=sys.argv[2]
geolon=sys.argv[3]
geolat=sys.argv[4]
eph = MPC.get_ephemeris(name, start=dateobs, location=(geolon, geolat, '1000m'), number=1)
print(eph['RA'][0], eph['Dec'][0])
