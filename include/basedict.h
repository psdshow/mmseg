#ifndef BASEDICT_H
#define BASEDICT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 *  Basic Dictionary Interface for Tokenizer.
 *
 *  * Define dictionary
 *  * Import
 *  * Add new lemma
 *  * Set lemma property
 *  * Load from dict
 *  * Build & Save new dict
 *
 *
 *  Due to dictionary will be wrap in swig, use plain c type in function.
 */
#include "csr_typedefs.h"

// CharMapper define
#define MAX_UNICODE_CODEPOINT   65535       // UCS-2's MAX

enum LemmaPropertyType {
    PROP_STRING,        // variant size
    PROP_SHORT,         // 2bit
    PROP_INT,           // 4bit
    PROP_LONG           // 8bit
};

typedef struct LemmaPropertyDefine
{
    char key[64];
    LemmaPropertyType prop_type;
}LemmaPropertyDefine;

typedef struct LemmaPropertyEntry
{
    char* key;
    LemmaPropertyType prop_type;
    void* data;
    u4          data_len;
}LemmaPropertyEntry;


class BaseDict
{
public:
    BaseDict();
    ~BaseDict();

public:
    int Open(const char* dict_path, char mode); // mode can be 'r', 'n'.  'n' stands for new; 'r' load pre-build dict from disk.
    int Save(const char* dict_path);            // save to disk
    int Build();                                // build trie-tree in memory.

    int Init(const LemmaPropertyDefine* props, int prop_count);     // define how many propery a lemma in this dictionary can have.
                                                                    // once this func been call, all data in dict will be trunc

    int Insert(const char* term, int freq, const u4* pos, int pos_count); // add new term -> dict, pos = char[4]

    int SetProp(const char* term, const char* key, const void* data, int data_len); // when prop_type is short|int|long, data_len will be ignored.
    int GetProp(const char* term, const char* key, void** data, int* data_len);

    int Properties(const char* term, LemmaPropertyEntry** entries);

};

class CharMapper
{
    /*
     * Convert char -> simp | sym case. eg. A->a; 0xF900 -> 0x8c48
     * Get char's category number. ascii...
     *
     * 256K in memory by default
     */
public:
    CharMapper(bool default_pass=false):_bDefaultPass(default_pass) {
        memset(_char_mapping, 0, sizeof(u4)*MAX_UNICODE_CODEPOINT);
    }

public:
    int Load(const char* filename);
    int Save(const char* filename);

    // mapping opt, not support A..Z/2 , should be done @ script side.
    int Mapping(u4 src, u4 dest, u1 tag = 0);
    int MappingRange(u4 src_begin, u4 src_end, u4 dest_begin, u4 dest_end, u1 tag = 0);
    int MappingPass(u4 src_begin, u1 tag = 0);
    int MappingRangePass(u4 src_begin, u4 src_end, u1 tag = 0);

    u4 Transform(u4 src, u1* out_tag);

private:
    u4   _char_mapping[MAX_UNICODE_CODEPOINT];
    bool _bDefaultPass;
};

/*
# The expected value format is a commas-separated list of mappings.
# Two simplest mappings simply declare a character as valid, and map a single character
# to another single character, respectively. But specifying the whole table in such
# form would result in bloated and barely manageable specifications. So there are
# several syntax shortcuts that let you map ranges of characters at once. The complete
# list is as follows:
#
# A->a
#     Single char mapping, declares source char 'A' as allowed to occur within keywords
#     and maps it to destination char 'a' (but does not declare 'a' as allowed).
# A..Z->a..z
#     Range mapping, declares all chars in source range as allowed and maps them to
#     the destination range. Does not declare destination range as allowed. Also checks
#     ranges' lengths (the lengths must be equal).
# a
#     Stray char mapping, declares a character as allowed and maps it to itself.
#     Equivalent to a->a single char mapping.
# a..z
#     Stray range mapping, declares all characters in range as allowed and maps them to
#     themselves. Equivalent to a..z->a..z range mapping.
# A..Z/2
#     Checkerboard range map. Maps every pair of chars to the second char.
#     More formally, declares odd characters in range as allowed and maps them to the
#     even ones; also declares even characters as allowed and maps them to themselves.
#     For instance, A..Z/2 is equivalent to A->B, B->B, C->D, D->D, ..., Y->Z, Z->Z.
#     This mapping shortcut is helpful for a number of Unicode blocks where uppercase
#     and lowercase letters go in such interleaved order instead of contiguous chunks.
*/

#endif // BASEDICT_H