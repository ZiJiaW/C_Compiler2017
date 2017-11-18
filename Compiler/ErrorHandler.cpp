#include "ErrorHandler.h"
using namespace std;
ErrorHandler::ErrorHandler()
{
    MapInit();
}
bool ErrorHandler::IsSuccessful()
{
    return errors.empty();
}
void ErrorHandler::MapInit()
{
    errorMap[1] = "Invalid integer beginning with [0]: ";
    errorMap[2] = "Invalid character in [const char] statement: ";
    errorMap[3] = "Invalid string with illegal character: ";
    errorMap[4] = "Missing [\"] when analyzing a string: ";
    errorMap[5] = "Unexpected single \'!\' : ";
    errorMap[6] = "Unrecognized token: ";
}
/**
print all errors!
*/
void ErrorHandler::PrintError()
{
    cout<<"Build failed! Errors following:"<<endl;
    for(vector<Error>::iterator p = errors.begin(); p != errors.end(); p++)
    {
        cout<<"line ["<<(*p)._lineNum<<"] "<<(*p).errMsg<<endl;
    }
}
/**
1. record an error with index errNum to errors;
2. errNum must be valid, otherwise it will crush!
*/
void ErrorHandler::SetError(int lineNum, int errNum, string _errToken)
{
    Error *err = new Error;
    err->_lineNum = lineNum;
    err->errMsg = errorMap[errNum]+_errToken;
    err->errToken = _errToken;
    errors.push_back(*err);
    delete err;
}
