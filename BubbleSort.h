#pragma once

#define BUBBLESORT_VECTOR(_arr_, _type_, _method_)					\
   for (size_t pass = 1; pass < _arr_.size(); pass++) {				\
       for (size_t i = 0; i < _arr_.size() - pass; i++) {			\
           if (_arr_[i]_method_ > _arr_[i+1]_method_) {				\
               _type_ temp	= _arr_[i];								\
			   _arr_[i]		= _arr_[i+1];							\
			   _arr_[i+1]	= temp;									\
           }														\
       }															\
   }