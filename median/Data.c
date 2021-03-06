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

////////////////////////////////////////////////////////////////////////////////////
// Data.c
//
// Jorge L. Vittes
//
// This code is for finding the median of a tree
// Algorithm by Guy Blelloch, and Jorge Vittes
///////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>
#include "Data.h"

////////////////////////////////////////////////////////////////////////////////////
// using the collected information for the unary clusters, and the data from the 
// binary clusters combine it, and place it in dest for when a contract occured
////////////////////////////////////////////////////////////////////////////////////
void dataContract(bin_data* left,bin_data* right, unary_data* sumCl, bin_data* dest)
{
  left = GET_DT(left);
  right = GET_DT(right);
  dest->totalweight = left->totalweight + right->totalweight + sumCl->totalweight;
  deprintf("Contracting dest->totalweight = %lf\n",dest->totalweight);
}

////////////////////////////////////////////////////////////////////////////////////
// using the collected information fro the unary clusters, and the data from the
// binary edge cluster combine it, and place it in dest for when a rake occurered
////////////////////////////////////////////////////////////////////////////////////
void rakeIn(bin_data* edgecl,unary_data* sumcl,unary_data* dest)
{
  edgecl = GET_DT(edgecl);
  dest->totalweight = edgecl->totalweight + sumcl->totalweight;
  deprintf("Raking dest->totalweight = %lf\n",dest->totalweight);
}

////////////////////////////////////////////////////////////////////////////////////
// combine the two unary datas clA, and clB and place that into dest
////////////////////////////////////////////////////////////////////////////////////
void addData(unary_data* clA, unary_data* clB, unary_data* dest)
{
  dest->totalweight = clA->totalweight + clB->totalweight;
  deprintf("AddData: dest->totalweight = %lf\n",dest->totalweight);
}

///////////////////////////////////////////////////////////////////////////////////
// copy the necessary data from unary data cl to the final destination dest
///////////////////////////////////////////////////////////////////////////////////
void finalizeData(unary_data* cl, final_data* dest) 
{
}

void pushDownData(bin_data* parent, bin_data* child) 
{
}

////////////////////////////////////////////////////////////////////////////////////
// constructor for bin_data unsing edge information u,v, are endpoints, w is a
// double.
////////////////////////////////////////////////////////////////////////////////////
bin_data::bin_data(int u, int v, double w)
{
  totalweight = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////
// default constructor for binary data
////////////////////////////////////////////////////////////////////////////////////
bin_data::bin_data()
{
  totalweight = 0.0;
}

///////////////////////////////////////////////////////////////////////////////////
// Reset function for binary data
///////////////////////////////////////////////////////////////////////////////////
void bin_data::reset()
{
  totalweight = 0.0;
}

///////////////////////////////////////////////////////////////////////////////////
// constructor for unary data using vertex data to be extracted from a file
///////////////////////////////////////////////////////////////////////////////////
unary_data::unary_data(FILE* file)
{
  int val,mark;
  fscanf(file,"%i %i",&val,&mark);
  totalweight   = (double) val;
}

///////////////////////////////////////////////////////////////////////////////////
// Default constructor for unary data
/////////////////////////////////////////////////////////////////////////////////// 
unary_data::unary_data()
{
  totalweight = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////
// Reset function for unary data
////////////////////////////////////////////////////////////////////////////////////
void unary_data::reset()
{
  totalweight = 0.0;
}

/////////////////////////////////////////////////////////////////////////////////////
// Default constructor for final data
/////////////////////////////////////////////////////////////////////////////////////
final_data::final_data()
{ 
}

//////////////////////////////////////////////////////////////////////////////////////
// Reset function for final data
//////////////////////////////////////////////////////////////////////////////////////
void final_data::reset()
{
}

///////////////////////////////////////////////////////////////////////////////////////
// Return true(1) if the data matches up for the purposes of this algorithm
///////////////////////////////////////////////////////////////////////////////////////
int isEqual2(final_data a, final_data b)
{
  return 1;
}

///////////////////////////////////////////////////////////////////////////////////////
// make the edge data given the edge information
///////////////////////////////////////////////////////////////////////////////////////
bin_data makeEdgeData (int u, int v, int w) 
{
  bin_data ret(u,v,w);
  return ret;
}

////////////////////////////////////////////////////////////////////////////////////////
//   Read vertex data from a file
////////////////////////////////////////////////////////////////////////////////////////
unary_data readVertexData(FILE* file)
{
  unary_data ret(file);
  return ret;
}

////////////////////////////////////////////////////////////////////////////////////////
// Read the edge data given endpoints v1 and v2
////////////////////////////////////////////////////////////////////////////////////////
bin_data readEdgeData (FILE* file,int v1, int v2)
{
  int w;
  fscanf(file,"%i",&w);
  return makeEdgeData (v1,v2,w);
}

void updateDataWeight(bin_data* bd)
{
}

/////////////////////////////////////////////////////////////////////////////////////////
// change the weight on a vertex
/////////////////////////////////////////////////////////////////////////////////////////
void changeNodeWeight(unary_data* dat)
{
  double w = (double) (rand() % MILLION);
  dat->totalweight = w;
}
