/*
 * This file is part of the EmCAD program which constitutes the client
 * side of an electromagnetic modeler delivered as a cloud based service.
 * 
 * Copyright (C) 2015  Walter Steffe
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef _BUFFER_H_
#define _BUFFER_H_

#ifndef MIN
#define MIN(a,b)  (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b)  (((a) > (b)) ? (a) : (b))
#endif


namespace DB {


template<class T> class Vec{
  public:
  T *data;
  int n;
  int stride;
  Vec(){data=NULL; n=0;  stride=1;}
  ~Vec(){ if(data) {Free(data); data=NULL;} }
  T  &operator[](int i) {return  data[stride*i];}
  void *alloc(int nm) {void *ptr=Malloc(nm*stride*sizeof(T)); data=(T *) ptr; n=nm; return ptr;}
  void *realloc(int nm) {void *ptr=Realloc(data, nm*stride*sizeof(T));  data=(T *) ptr;  n=nm; return ptr;}
  void  resize(int nm) {
	if(data){
	 if(n==nm) return;
	 if(nm==0){Free(data); data=NULL; n=nm; return;}
	 T *temp;
	 if(n>0){
          temp=(T *) Malloc(n*stride*sizeof(T));
          memcpy(temp,data, n*stride*sizeof(T));
	 }
         data=(T *) Realloc(data, nm*stride*sizeof(T));
         if(n>0){
	   memcpy(data,temp, MIN(n,nm)*stride*sizeof(T));
	   Free(temp);
	 }
	} else if(nm>0) data=(T *) Malloc(nm*stride*sizeof(T));
        n=nm; 
      }
  void  free() {Free(data); data=NULL;};
  Vec<T>& operator =(const Vec<T> &rhs)
  {
    if (this != &rhs) { 
        n = rhs.n;
        stride = rhs.stride;
	if(n>0){
	 if(data) data=(T *) Realloc(data, n*stride*sizeof(T));
	 else     data=(T *) Malloc(n*stride*sizeof(T));
         memcpy(data,rhs.data, n*stride*sizeof(T));
	} else  if(data) {Free(data); data=NULL;}
    }
    return *this;    // Return ref for multiple assignment
  }

};




template<class T> class BufferIn{
  public:
  virtual void input(T el)=0;
  virtual void reset()=0;
};


template<class T, int len, int stride=1> class Buffer: public BufferIn<T>{
  List_T *liste;
  public:
  Buffer(){liste=List_Create(len, len, sizeof(T));}
  ~Buffer(){List_Delete(liste);}
  void input(T el) {List_Add(liste, &el);}
  void reset(){
    List_Reset(liste);
  }
  void flush(Vec<T> *vec){
    vec->stride=stride;
    if(vec->data) Free(vec->data);
    vec->alloc(liste->n/stride);
    memcpy(vec->data, liste->array, liste->n * liste->size);
    List_Reset(liste);
  }
};


//**
// Buffer_I read a sequence of sublists of positive numbers with negative numbers
// used as separators. The number of elements of each sublist must be equal to stride .
template<int len, int stride> class Buffer_I: public Buffer<int, len, stride>{
  int is;
  public:

  Buffer_I<len, stride>(): Buffer<int, len, stride>() {is=0;}
  void input(int n){
    if(is++==stride){
       if(n>0) yymsg(FATAL, "wrong number of indices in sublist\n");
       else is=0;
    }
    else {
       if(n<0) yymsg(FATAL, "wrong number of indices in sublist\n");
       else Buffer<int, len, stride>::input(n);
    }
  }
  void flush(Vec<int> *vec){is=0; Buffer<int, len, stride>::flush(vec);}
};


}  //namespace

#endif
