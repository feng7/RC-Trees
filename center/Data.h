////////////////////////////////////////////////////////////////////////
// Copyright (c) 2004
// Carnegie Mellon University:
// Guy Blelloch, Umut Acar, Jorge Vittes
//
// All Rights Reserved.
//
// Permission to use, copy, modify and distribute this software and its
// documentation is hereby granted, provided that both the copyright
// notice and this permission notice appear in all copies of the
// software, derivative works or modified versions, and any portions
// thereof.
//
// CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
// CONDITION.  CARNEGIE MELLON AND THE SCAL PROJECT DISCLAIMS ANY
// LIABILITY OF ANY KIND FOR ANY DAMAGES WHATSOEVER RESULTING FROM
// THE USE OF THIS SOFTWARE.
//
// The ALADDIN requests users of this software to return to
//
//  Guy Blelloch                         blelloch@cs.cmu.edu
//  School of Computer Science
//  Carnegie Mellon University
//  5000 Forbes Ave.
//  Pittsburgh PA 15213-3890
//
// any improvements or extensions that they make and grant Carnegie Mellon
// the rights to redistribute these changes.
///////////////////////////////////////////////////////////////////////


/*************************************************
 * data.h
 *
 * Jorge L. Vittes
 *
 * defines the interface for the data files, which
 * define what operation the tree contraction does
 ************************************************/

#ifndef _DATA_H_
#define _DATA_H_ 1

#include "Globals.h"
#include <iostream>

#define GET_DT(c) (bin_data *)((int) c & 0xFFFFFFFC)
#define GET_IN(c) (int) c & 1
#define GET_OFF(c) (int) c & 1
#define GET_OT(c) ~((int)c & 1) & 1
#define THIS_DATA(d) ((GET_DT(d))->data + (GET_IN(d)))
#define OTHER_DATA(d) ((GET_DT(d))->data + (GET_OT(d)))

class bin_data{
 public:
  bin_data(int u, int v, double w);
  bin_data();
  double len;
  double mpath1;
  double mpath2;
  void print() {
     deprintf("len = %lf, mpath1 = %lf, mpath2 = %lf\n",len,mpath1,mpath2);
  }
  void reset();
};

class unary_data{
 public:
  unary_data(FILE* file);
  unary_data();
  double len;
  double mpath;
  void print() {
    deprintf("len = %lf, mpath = %lf\n",len,mpath); 
  }
  void reset(); 
};

class final_data{
 public:
  final_data();
  void print() {
   
  }
  void reset(); 
};

double getThisData(bin_data* bd);
double getLen(bin_data* bd);
void dataContract(bin_data* left,bin_data* right, unary_data* sumCl, bin_data* dest);
void rakeIn(bin_data* edgecl,unary_data* sumcl,unary_data* dest);
void finalizeData(unary_data* udata, final_data* fdata);
void addData(unary_data* clA, unary_data* clB, unary_data* dest);
void addNodeCluster(unary_data* cl, unary_data* ndata, unary_data* dest);

int isEqual2(final_data a, final_data b);
bin_data makeEdgeData (int u, int v, int w); 

// <MUST HAVE>
//   Read vertex data from a file
unary_data readVertexData(FILE* file);
bin_data readEdgeData (FILE* file,int v1, int v2);

void pushDownData(bin_data* parent, bin_data* child);

void updateDataWeight(bin_data* bd);
void changeNodeWeight(unary_data* dat);


// </MUST HAVE>
//  Not necessary
 
#endif
