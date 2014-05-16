%module(directors="1") mmseg

// Remove some warnings
#pragma SWIG nowarn=362,503,401,389,516,511

// Use STL support
%include <std_vector.i>
%include <std_string.i>
%include <std_map.i>
#if SWIGPYTHON || SWIGRUBY
%include <std_complex.i>
#endif
// Use C99 int support
%include <stdint.i>

// Use exceptions
%include "exception.i"
// Use pointer.
%include cpointer.i

%{
#include <csr_typedefs.h>
%}

// apply type mapping
%apply unsigned char { u1 }; 
%apply unsigned short { u2 }; 
%apply unsigned int { u4 }; 
%apply unsigned long long { u8 }; 

/* Create some functions for working with "int *" */
%pointer_functions(u2, ushortp);

%ignore mm::CharMapper::STATUS_OK; 
%ignore mm::CharMapper::STATUS_FILE_NOT_FOUND; 
%ignore MAX_UNICODE_CODEPOINT;
%ignore UNICODE_MASK;
%ignore UNICODE_BITS;
%ignore UNICODE_PAGE_SIZE;

/*
// Global Tweaks to basedict
%ignore mm::LemmaPropertyEntry; 
%ignore mm::LemmaPropertyDefine; 
%ignore mm::LemmaPropertyType; 
%ignore mm::BaseDict::Init;
%ignore mm::CharMapper::Transform;
%ignore mm::BaseDictSchema::GetColumnType;
%ignore mm::DictMatchResult::ClearResult;
%ignore mm::DictMatchResult::AddResult;
%ignore mm::DictMatchResult::GetResultPtr;
*/

// This section is copied verbatim into the generated source code.
// Any include files, definitions, etc. need to go here.
%{
#include <mm_charmap.h>
%}

// Language Specific Sections
%include Java.i
%include Python.i

/*
%newobject get_dict_property_string;
%newobject get_dict_property_string_by_value;
*/

// Help SWIG handle std vectors
namespace std
{
  //%template(VectorBool) vector<bool>;
  //%template(VectorUInt8) vector<uint8_t>;
}

/* stl support */
%include stl.i
 
/* Wrapper并生成代码 */
%include "mm_charmap.h"
