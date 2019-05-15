
#
# class Simulation example using maboss communication layer: to be merged with pyMaBoSS/maboss/simul.py
#

class Simulation:

    def __init__(self, bndfile, cfgfile = None, cfgfiles = None):
        self._network = file_get_contents(bndfile)
        if cfgfile:
            self._config = file_get_contents(cfgfile)
        elif cfgfiles:
            self._config = ""
            for cfgfile in cfgfiles:
                self._config += file_get_contents(cfgfile)
        else:
            raise Exception("Simulation: cfgfile or cfgfiles must be set")

    def getNetwork(self):
        return self._network

    def getConfig(self):
        return self._config

#
# utility function
#

import os

def file_get_contents(filename):
    if not os.path.isfile(filename):
        raise Exception(filename + " is not a valid file")
    fd = os.open(filename, os.O_RDONLY)
    if fd >= 0:
        stat = os.fstat(fd)
        contents = os.read(fd, stat.st_size)
        os.close(fd)
        return contents.decode()
    raise Exception(file + " is not readable")
