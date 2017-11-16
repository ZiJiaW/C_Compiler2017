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
    errorMap[1] = "Got an invalid integer beginning with [0] !";
    errorMap[2] = "Got an invalid character!";
    errorMap[3] = "Got an invalid string with illegal character!";
    errorMap[4] = "Missing [\"] when analyzing a string!";
    errorMap[5] = "Unexpected single token: [!] !";
    errorMap[6] = "Got an unrecognized token!";
}
/**
1. record an error with index errNum to errors;
2. errNum must be valid, otherwise it will crush!
*/
void ErrorHandler::SetError(int lineNum, int errNum, string _errToken)
{
    Error *err = new Error;
    err->_lineNum = lineNum;
    err->errMsg = errorMap[errNum];
    err->errToken = _errToken;
    errors.push_back(*err);
    delete err;
}
