/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2011 Jens Gustedt, INRIA, France                    */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#include "orwl_futex.h"

#ifdef HAVE_FUTEX
P99_INSTANTIATE(int, orwl_futex, int*, int, int, const struct timespec*, int*, int);
P99_INSTANTIATE(int, orwl_futex_wait_once, int*, int);
P99_INSTANTIATE(int, orwl_futex_wake, int*, int);
P99_INSTANTIATE(int, orwl_futex_signal, int*);
P99_INSTANTIATE(int, orwl_futex_broadcast, int*);
#endif