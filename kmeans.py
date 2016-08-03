import ctypes
from numpy import zeros
from numpy.ctypeslib import ndpointer

_kmeans = ctypes.CDLL('libkmeans.so')
_kmeans.kmeans.argtypes = (ndpointer(ctypes.c_double), ctypes.c_int, ctypes.c_int, ndpointer(ctypes.c_int),
     ctypes.c_int, ctypes.c_int, ctypes.c_double, ctypes.c_int)

def kmeans(X,K=2,init='random',tol=1.0e-06,nruns=1):
    global _kmeans
    N = X.shape[0]
    p = X.shape[1]
    if init is 'random':
        cinit = 0
    clusterids = zeros(N,dtype=ctypes.c_int)
    _kmeans.kmeans(X.reshape(N*p,),ctypes.c_int(N),ctypes.c_int(p),clusterids,ctypes.c_int(K),ctypes.c_int(cinit),
        ctypes.c_double(tol),ctypes.c_int(nruns))
    return clusterids
