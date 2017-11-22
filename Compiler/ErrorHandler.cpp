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
    // lexical error
    errorMap[1] = "Invalid integer beginning with [0]: ";
    errorMap[2] = "Invalid character in [const char] statement: ";
    errorMap[3] = "Invalid string with illegal character: ";
    errorMap[4] = "Missing [\"] when analyzing a string: ";
    errorMap[5] = "Unexpected single \'!\' : ";
    errorMap[6] = "Unrecognized token: ";

    // syntax error
    errorMap[7] = "Expect function definition here, Unexpected token: ";
    errorMap[8] = "Missing main function, or your definition is invalid!";
    errorMap[9] = "Unexpected token after main definition: ";
    errorMap[10] = "Expect type(int/char) statement, got: ";
    errorMap[11] = "Illegal identifier: ";
    errorMap[12] = "Expect a number here, got: ";
    errorMap[13] = "Duplicate definition of identifier: ";
    errorMap[14] = "Expect \',\' or \';\', got: ";
    errorMap[15] = "Expect a character here, got: ";
    errorMap[16] = "Invalid array size: ";
    errorMap[17] = "Expect \']\', got: ";
    errorMap[18] = "Expect \'(\', got: ";
    errorMap[19] = "Expect \')\', got: ";
    errorMap[20] = "Expect \'{\', got: ";
    errorMap[21] = "Expect \'}\', got: ";
    errorMap[22] = "Expect \';\', got: ";
    errorMap[23] = "Can't find a return statement in function: ";
    errorMap[24] = "Got a return statement in a void function: ";
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
