import ctypes,os
from numpy import zeros
from numpy.ctypeslib import ndpointer

libpath = os.path.abspath(__file__)
libpath = os.path.realpath(libpath)
libpath = os.path.dirname(libpath)
libpath = os.path.join(libpath,"libkmeans.so")

_kmeans = ctypes.CDLL(libpath)
_kmeans.kmeans.argtypes = (ndpointer(ctypes.c_double), ctypes.c_int, ctypes.c_int, ctypes.c_int, ndpointer(ctypes.c_int),
     ctypes.c_int, ctypes.c_int, ctypes.c_double, ctypes.c_int)

def kmeans(X,K=2,metric='euclidean',init='random',tol=1.0e-06,nruns=1):
    global _kmeans
    N = X.shape[0]
    p = X.shape[1]
    # initialization method
    cmet = 0
    if init is 'random':
        cinit = 0
    elif init is 'kpp':
        cinit = 1
    else:
        print 'ERROR: Unsupported initialization method.  Defaulting to random.'
        cinit = 0
    clusterids = zeros(N,dtype=ctypes.c_int)
    # distance method
    cmet = 0
    if metric is 'euclidean':
        cmet = 0
    elif metric is 'cosine':
        cmet = 1
    elif metric is 'correlation':
        cmet = 2
    elif metric is 'manhattan':
        cmet = 3
    else:
        print 'ERROR: Unsupported distance metric. Defaulting to euclidean.'
        cmet = 0
    # library call
    _kmeans.kmeans(X.reshape(N*p,),ctypes.c_int(N),ctypes.c_int(p),ctypes.c_int(cmet),clusterids,ctypes.c_int(K),
        ctypes.c_int(cinit),ctypes.c_double(tol),ctypes.c_int(nruns))
    return clusterids
