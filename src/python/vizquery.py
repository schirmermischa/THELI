#!/usr/bin/env python
"""
   CDS package to query VizieR big catalogues
   G.Landais (27/10/2017)

   VizieRClient.py [-h] [-l] [-i] [-mime=output_format] [-source=..] [asu_params]
     -h: this help
     -l: list large catalogues
     -d: debug
     -i: table information (table given by -source)
     -mime: csv|votable|ascii
     -source: table/catalogue name
     --file  : query with a list

     asu_params : the ASU parameters
        ex: -out.max: max number of records
            -c: center
            -c.rs: radius in arcsec
            "Jmag>10" constraint on column Jmag


     Licensed under a BSD license - see LICENSE.txt for details
"""
import sys
import re
import requests

try:
    import numpy
    from astropy.table import *
    from astropy.io import ascii

    from astropy import log
    log.setLevel('ERROR')

    use_astropy = True
except ImportError:
    use_astropy = False
    sys.stderr.write("(WARNING) astropy is not installed\n")

if int(sys.version[0]) < 3:
    import urllib2 as ulib
else:
    import urllib.request as ulib
import urllib


DEFAULT_RADIUS=2. # arcsec
DEFAULT_LIMIT=100
DEBUG = False
VIZIER_URL = "http://vizier.u-strasbg.fr/viz-bin/"
TAPVIZIER = "http://tapvizier.u-strasbg.fr/TAPVizieR/"


FORMAT_TSV = "tsv"
FORMAT_VOTABLE = "votable"
FORMAT_ASCII = "ascii"


def debug(obj):
    if DEBUG is True:
        sys.stderr.write("(debug)"+str(obj)+"\n")


def error(obj):
    sys.stderr.write("(error)"+str(obj)+"\n")


def std_name(name):
    if isinstance(name, bytes):
        return name.decode('utf8').strip()
    else:
        return name.strip()

class TAP:
    """Execute TAP queries
    """

    def __init__(self, server):
        """Constructor
        :param server: the TAP server root URL
        """
        self.__server = server

    def __tapurl(self, adql, fmt):
        query = "/tap/sync?request=doQuery&phase=RUN&lang=ADQL"
        query += "&format="+fmt
        #query += "&query="+adql.replace(" ", "+").replace("%","%25")
        query += "&query="+adql.replace(" ", "+")
        return query

    def get_table(self, adql, fmt="votable"):
        """Execute Synchronous query and bufferise the result
        :param adql: ADQL query
        :param fmt: output format
        :return: astropy.table.Table
        """
        if use_astropy is False:
            raise Exception("unavailable option - needs to install astropy")

        query = self.__tapurl(adql, fmt)
        debug("TAP(GET): " + self.__server + query)
        return Table.read(self.__server+query)


class METAdata:
    """ Cache METAdata
        serialize the metadata into a local file with catalogues description
        (catalogue_name, table_name, column_name)
    """
    def __init__(self):
        if use_astropy is False:
            raise Exception("unavailable option - needs to install astropy")

        self.__tap = TAP(TAPVIZIER)
        self.__columns = None

    def info_columns(self, name=None):
        """get the column info
        :param name: the table or catalogue name
        :returns: columns
        """
        if name is None:
            adql = "SELECT column_name, table_name, col.unit, datatype, description, indexed, std"
            adql+= " FROM TAP_SCHEMA.columns col"
            adql+= " JOIN METAtab t on (col.table_name = t.name)"
            adql+= " JOIN METAdba d on (t.dbaid=d.dbaid)"
            adql+= " JOIN METAcat c on (c.catid=t.catid)"
            adql+= " WHERE d.login like 'vizls%'"
        else:
            adql = "SELECT column_name, table_name, unit, datatype, description, indexed, std"
            adql+= " FROM TAP_SCHEMA.columns where table_name like '{0}%'".format(name)

        table = self.__tap.get_table(adql, FORMAT_VOTABLE)

        try:
            table.convert_bytestring_to_unicode()
        except:
            sys.stderr("--byte is not converted")
        for col in table.columns:
            table[col] = table[col].astype(str)

        return table


    def info_table(self, name):
        """get the table info
        :param name: search name: data_collection(=catalogue), acronym, table_name
        :return: astropy table
        """
        if name is None: raise Exception("name is null")

        adql = "SELECT t.name as table_name, t.explain as description, t.records"
        adql+= " FROM METAtab t WHERE t.name like '{0}%'".format(name)
        table = self.__tap.get_table(adql, FORMAT_VOTABLE)

        for col in table.columns:
            table[col] = table[col].astype(str)

        return table

    def __std_table(self, table):
        """(Update) standardize an astropy table
        :param table: table in input
        """
        re_v1 = re.compile("^ *!ext/([^ ]+) .*$")
        re_v2 = re.compile(".* *-source[= ]+([^ ]*).*")
        sz = len(table["table_id"])
        columnt = Column([0] * sz, dtype=int, name="type")

        for i in range(sz):
            value = std_name(table["table_id"][i])
            mo = re_v1.match(value)
            if mo:
                table["table_id"][i] = mo.group(1).replace(".exe","")
                columnt[i] = 1
            else:
                mo = re_v2.match(value)
                if mo:
                    table["table_id"][i] = mo.group(1)
                    columnt[i] = 2
                else:
                    columnt[i] = 0

        for col in table.columns:
            table[col] = table[col].astype(str)
        table.add_column(columnt)

    def list_large_table(self, catalogue_name=None):
        """list the large VizieR table
        :param catalogue_name: catalogue name (VizieR id name)
        :return: astropy table with large table description
        """
        adql = "SELECT c.name as data_collection, t.name as table_name, t.explain as description, t.records, t.filename as table_id"
        adql+= " FROM METAtab t join METAdba d on (t.dbaid=d.dbaid)"
        adql+= " join METAcat c on (c.catid=t.catid)"
        adql+= " WHERE d.login like 'vizls%'"
        if catalogue_name is not None:
            adql += " AND t.name like '{0}/%'".format(catalogue_name)
        table = self.__tap.get_table(adql, FORMAT_VOTABLE)

        self.__std_table(table)
        return table

USER_AGENT = "python-vizquery"
class CDSClient:
    """ VizieR client
    """
    def __init__(self, default_format=FORMAT_ASCII, default_radius=DEFAULT_RADIUS, default_limit=DEFAULT_LIMIT):
        """
        Constructor.
        :param default_format: default output format
        :param default_radius: default radius
        :param default_limit: default limit of records returned
        """
        self.format = default_format
        self.radius = default_radius
        self.limit = default_limit

        self.__source = None
        self.__parameters = None
        self.__filename = None

    def __service(self, format):
        if format == FORMAT_TSV:
            return "asu-tsv"
        elif format == FORMAT_VOTABLE:
            return "votable"
        else :
            return "asu-txt"

    def __url(self, format = None):
        if format is None: out = self.format
        else: out = format
        url = "{0}/{1}?".format(VIZIER_URL, self.__service(out))

        if self.__source is None:
            raise Exception("source is required")
        url +="&-source="+self.__source

        if self.__parameters is None:
            return url

        for param in self.__parameters:
            url += "&"+param
        return url


    def query(self, name, params, filename=None):
        """ Prepare the query
        :param name: table designation
        :param params: asu parameters
        :param filename: constraint filename submitted
        :return: url/adql
        """
        self.__source= name
        self.__parameters = params
        if filename is not None: self.__filename = filename
        return self.__url()

    def get(self):
        """get the data in an astropy table
        :return: result in astropy table
        """
        if use_astropy is False:
            raise Exception("unavailable option - needs to install astropy")

        url = self.__url(FORMAT_TSV)
        debug("url:" + url)
        return ascii.read(url, comment='^#', format='fixed_width_no_header', delimiter='\t')

    def post(self):
        """get the data in an astropy table usinf a file list
        :return: result in astropy table
        """
        if self.__filename is None:
            fd = self.__get_http()
        else:
            fd = self.__post_http()

        return ascii.read(fd.text)


    def __get_http(self):
        url = self.__url()
        debug("url:"+url)
        request = ulib.Request(url)
        request.add_header("User-Agent", USER_AGENT)
        return ulib.urlopen(request)

    def __post_http(self):
        self.__parameters.append("-source="+self.__source)
        url = "{0}/{1}?".format(VIZIER_URL, self.__service(self.format))
        debug("url(post):"+url)

        sort_list = None
        params = []
        for parameter in self.__parameters:
            p = parameter.split('=')
            if len(p) != 2: raise Exception("unrecognized parameter {0} in file upload".format(parameter))
            if p[0] == "-sort": sort_list = p[1]
            params.append((p[0],p[1]))

        if sort_list:
            return requests.post(url, headers={"User-Agent": USER_AGENT},
                                 data=params, files={'*-sort': open(self.__filename, 'r')})
        else:
            return requests.post(url,  headers={"User-Agent": USER_AGENT},
                                 data=params, files={'-c': open(self.__filename, 'r')})

    def print_stdout(self):
        """ print result into stdout
        """
        if self.__filename is None:
            fd = self.__get_http()
        else:
            fd = self.__post_http()

        for line in fd:
            sys.stdout.write(line.decode('utf8'))


if __name__ == "__main__":
    __info = False
    __params = []
    __mime = None
    __source = None
    __filename = None

    reg_constraint = re.compile("^(.*)([<>])([^<>]*)$")
    for __o in sys.argv[1:]:
        if __o in ("-h", "--help"):
            help("__main__")
            sys.exit(1)

        elif __o in ("-i", "--info"):
            __info = True

        elif __o in ("-d", "--debug"):
            DEBUG = True

        elif __o in ("-l", "--list"):
            meta = METAdata()
            tables = meta.list_large_table()
            for table in tables:
                if table[4].find(".dat") > 0: continue
                print ("{1} ({3} records) id={4}\n  {2}".format(table[0], table[1], table[2],
                                                                table[3], table[4]))
            sys.exit(0)

        elif __o.find("--file=") == 0:
            __filename = __o[7:]
            continue

        else:
            """asu params"""
            mo = re.match("^-source=(.*)$", __o)
            if mo:
                __source = mo.group(1)
            else:
                mo = re.match("-mime=(.*)$", __o)
                if mo:
                    __mime = mo.group(1)
                    continue

                if __o[0] == '-':
                    __params.append(__o)
                    continue

                if __o.find("=") > 0:
                    __params.append(__o)
                    continue

                m = reg_constraint.match(__o)
                if m is None:
                    raise Exception("unavailable parameter " + __o)

                __params.append("{0}={1}{2}".format(m.group(1), m.group(2), m.group(3)))


    if __source is None:
        help("__main__")
        sys.exit(1)

    if __info is True:
        meta = METAdata()
        tables = meta.info_table(__source)

        for table in tables:
            print("{0} ({2} records)\t{1}".format(table[0], table[1], table[2]))
            columns = meta.info_columns(table[0])
            for column in columns:
                opt = ""
                if (column[4] == '1'): opt += "(i)"
                if (column[5] == '1'): opt += "(p)"
                print("{0:32s} {1:10s} {2:6s} {3}".format(column[0], column[1], opt, column[3]))

            print()

        print ("(i) : indexed column\n(p) default column")
        sys.exit(0)

    client = CDSClient()
    if __mime is not None:
        if __mime in (FORMAT_TSV, FORMAT_VOTABLE, FORMAT_ASCII):
            client.format = __mime

    #print(__params)
    client.query(__source, __params, __filename)
    client.print_stdout()
