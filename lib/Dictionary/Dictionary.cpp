#include "Dictionary.h"

Dictionary::Dictionary() {
}

char * Dictionary::get(const char* str) {  
  strcpy_P(Buffer, str);
  return Buffer;
}
