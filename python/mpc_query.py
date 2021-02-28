import sys
from astroquery.mpc import MPC
name=sys.argv[1]
dateobs=sys.argv[2]
eph = MPC.get_ephemeris(name, start=dateobs, number=1)
print(eph['RA'][0], eph['Dec'][0])
