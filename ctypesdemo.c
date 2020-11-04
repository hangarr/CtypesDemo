#if !defined(CPART)

# python3 wrapper around libsoundpipe.so shared object

import os
from ctypes import *

def wrap_function(lib, funcname, restype, argtypes):
    func = lib.__getattr__(funcname)
    func.restype = restype
    func.argtypes = argtypes
    return func

libctypesdemo = CDLL("libctypesdemo.so")

#====================================================================
# PART 1
#--------------------------------------------------------------------
part1 = wrap_function(libctypesdemo, "part1", c_int, 
                      [c_int, c_float, POINTER(c_int), POINTER(c_float)])

i1 = c_int(10)                    # ctype object that includes a C int
f1 = c_float(20.0)
i2 = c_int(30)
f2 = c_float(40.0)
i2p = pointer(i2)                 # ctype object that is a C pointer to a C int
f2p = pointer(f2)

print("Part 1a - Basic types and pointers")

print("PY - i1: %d"%i1.value)
print("PY - f1: %f"%f1.value)

print("PY - i2 before: %d"%i2.value)
print("PY - f2 before: %f"%f2.value)
print("PY - i2p before: %d"%i2p[0])
print("PY - f2p before: %f"%f2p[0])

part1(i1, f1, i2p, f2p)

print("PY - i2 after: %d"%i2.value)
print("PY - f2 after: %f"%f2.value)
print("PY - i2p after: %d"%i2p[0])
print("PY - f2p after: %f"%f2p[0])

print("\nPart 1b - Basic types byref")

part1(i1, f1, byref(i2), byref(f2))

print("PY - i2 after: %d"%i2.value)  # Python <int>
print("PY - f2 after: %f"%f2.value)
print("PY - i2p after: %d"%i2p[0])   # NOTE: Python <int>!
print("PY - f2p after: %f"%f2p[0])
#====================================================================

### begin C-block
"""
#else

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int part1(int i1, float f1, int *i2, float *f2) {
    printf("C - i1: %d\n", i1);
    printf("C - f1: %f\n", f1);

    printf("C - *i2 before: %d\n", *i2);
    printf("C - *f2 before: %f\n", *f2);
    *i2 = *i2 * 2;
    *f2 = *f2 * 2;
    printf("C - *i2 after: %d\n", *i2);
    printf("C - *f2 after: %f\n", *f2);

    return 1;
}

#endif

#if !defined(CPART)
"""
### end C-block

#====================================================================
# PART 2
#--------------------------------------------------------------------
part2 = wrap_function(libctypesdemo, "part2", c_int, [c_int, POINTER(c_float)])

print("\nPart 2a - Basic array types")

f3arr = (c_float * 3)(*range(3))
f3 = cast(f3arr, POINTER(c_float))

for i in range(3):
    print("PY - f3[%d] before: %f"%(i, f3[i]))   # Python <float>s as accessed

part2(c_int(3), f3)

for i in range(3):
    print("PY - f3[%d] after: %f"%(i, f3[i]))

print("\nPart 2b - Basic array types updates")

for i in range(3):
    f3[i] = c_float(f3[i] * 3)                  # Array items accessed as Python float

for i in range(3):
    print("PY - f3[%d] before: %f"%(i, f3[i]))

part2(c_int(3), f3)

for i in range(3):
    print("PY - f3[%d] after: %f"%(i, f3[i]))
#====================================================================

### begin C-block
"""
#else

int part2(int n, float *f3) {
    int i;

    for(i = 0; i < n; i++) {
      printf("C - f3[%d] before: %f\n", i, f3[i]);
    }

    for(i = 0; i < n; i++) {
        f3[i] *= 2;
    }

    for(i = 0; i < n; i++) {
      printf("C - f3[%d] after: %f\n", i, f3[i]);
    }

    return 2;
}

#endif

#if !defined(CPART)
"""
### end C-block

#====================================================================
# PART 3
#-------------------------------------------------------------------- 

NP3 = 3

FLOAT_NP3 = c_float * NP3

class St1(Structure):
    _fields_ = [
        ("n", c_int),
        ("f1a", FLOAT_NP3),
        ("f2p", POINTER(c_float)),
        ("f3p", POINTER(c_float))
        ]

part3 = wrap_function(libctypesdemo, "part3", c_int, [POINTER(St1)])

s = St1()                 # Create an instance of the C structure
sp = POINTER(St1)(s)      # Point

print("\nPart 3a - Basic structures")

# all seem to have the same effect even though r-sides different
#sp[0].n = NP3                  # Python <int> constant
#sp[0].n = c_int(NP3)           # ctypes c_int
sp[0].n = c_int(NP3).value     # Python <int> object

sarr = FLOAT_NP3()                          # actually a pointer to the array of c_float
sp[0].f2p = cast(sarr, POINTER(c_float))    # cast as a pointer to first c_float

sf = c_float(20.0)
sp[0].f3p = pointer(sf)

for i in range(sp[0].n):
    sp[0].f1a[i] = i
    sp[0].f2p[i] = i * 3

print("PY - s.n, sp[0].n before: %d, %d"%(s.n, sp[0].n))
for i in range(sp[0].n):
    print("PY - sp[0].f1a[%d] before: %f"%(i, sp[0].f1a[i]))
for i in range(sp[0].n):
    print("PY - sp[0].f2p[%d] before: %f"%(i, sp[0].f2p[i]))
print("PY - sp[0].f3p[0] before: %f"%sp[0].f3p[0])

part3(sp)

print("PY - s.n, sp[0].n after: %d, %d"%(s.n, sp[0].n))
for i in range(sp[0].n):
    print("PY - sp[0].f1a[%d] after: %f"%(i, sp[0].f1a[i]))
for i in range(sp[0].n):
    print("PY - sp[0].f2p[%d] after: %f"%(i, sp[0].f2p[i]))
print("PY - sp[0].f3p[0] after: %f"%sp[0].f3p[0])

#====================================================================

### begin C-block
"""
#else

typedef struct {
    int n;
    float f1a[3];
    float *f2p;
    float *f3p;
} st1;

int part3(st1 *sp) {
    int i;

    printf("C - sp->n before: %d\n", sp->n);

    for(i = 0; i < sp->n; i++) {
        printf("C - sp->f1a[%d] before: %f\n", i, sp->f1a[i]);
    }
    for(i = 0; i < sp->n; i++) {
        printf("C - sp->f2p[%d] before: %f\n", i, sp->f2p[i]);
    }

    printf("C - *(sp->f3p) before: %f\n", *(sp->f3p));


    for(i = 0; i < sp->n; i++) {
        sp->f1a[i] *= 5;
    }
    for(i = 0; i < sp->n; i++) {
        sp->f2p[i] *= 6;
    }
    *(sp->f3p) *= 7;

    printf("C - sp->n after: %d\n", sp->n);

    for(i = 0; i < sp->n; i++) {
        printf("C - sp->f1a[%d] after: %f\n", i, sp->f1a[i]);
    }
    for(i = 0; i < sp->n; i++) {
        printf("C - sp->f2p[%d] after: %f\n", i, sp->f2p[i]);
    }

    printf("C - *(sp->f3p) after: %f\n", *(sp->f3p));

    return 3;
}


#endif

#if !defined(CPART)
"""
### end C-block
#====================================================================
# PART 4
#-------------------------------------------------------------------- 

#====================================================================

### begin C-block
"""
#else


#endif

#if !defined(CPART)
"""
### end C-block
#====================================================================
# PART N
#-------------------------------------------------------------------- 


### begin C-block
"""
#else


#endif

#if !defined(CPART)
"""
### end C-block

#endif
