
class Location:


    def __init__(self, id, name, ltype, xmin, xmax, ymin, ymax):
        self._ID = id
        self._xmin = xmin
        self._xmax = xmax
        self._ymin = ymin
        self._ymax = ymax
        self._Name = name
        self._Type = ltype

class Person:

    def __init__(self, id, name, age, gender, homeid, maxi):
        self._ID = id
        self._Name = name
        self._Age = age
        self._Gender = gender
        self._HomeID = homeid
        self._MaxI = maxi
        self._Connections = []

    def setTime(self, t):
        self._Time = t
    def setAge(self, age):
        self._Age = age
    def setX(self, x):
        self._X = x
    def setY(self, y):
        self._Y = y
    def setLocID(self, lid):
        self._LocID = lid
    def setState(self, state):
        self._State = state
    def setConnections(self, connections):
        conns = [[x.strip() for x in strcon[1:].split(',')] for strcon in connections]
        for c in conns:
            if len(c) > 1:
                self._Connections.append([(self._ID,int(x)) for x in c])
            else:
                self._Connections.append([])
        print len(self._Connections)
    def setHBS(self, hbs):
        self._HBS = hbs
    def setSC(self, sc):
        self._SC = sc
    def setIC(self, ic):
        self._IC = ic
    def setVC(self, vc):
        self._VC = vc
    def setVL(self, vl):
        self._VL = vl
