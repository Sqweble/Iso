#ifndef _COMMON_MACROS_
#define _COMMON_MACROS_

#define MIN(a,b) ((a<b)? a : b)
#define MAX(a,b) ((a>b)? a : b)
#define SAFE_DELETE(pointer) if (pointer != nullptr) delete(pointer); pointer = nullptr
#define SAFE_DELETE_ARRAY(pointer) if (pointer != nullptr) delete[](pointer); pointer = nullptr
#define SAFE_FREE(pointer) if(pointer != nullptr) free(pointer); pointer = nullptr
#define FOR_EACH(vector) for(unsigned int i=0;i<vector.size();i++)
#define FOR_EACH_VAR(variableName,vector) for(unsigned int variableName = 0;variableName<vector.size();variableName++)
#define CYCLE_ERASE(vector, element) vector[element] = vector[vector.size()-1]; vector.pop_back()
#define SET_VECTOR(vector, value) for(unsigned int i=0;i<vector.size();i++) vector[i] = value
#define TO_FLOAT(number) static_cast<float>(number)

#endif