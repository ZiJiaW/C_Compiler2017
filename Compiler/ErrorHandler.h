#ifndef ERRORHANDLER_H_INCLUDED
#define ERRORHANDLER_H_INCLUDED
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;
typedef struct _Error {
    int _lineNum;
    string errMsg;
    string errToken;
} Error;
class ErrorHandler {
public:
    bool IsSuccessful();
    void SetError(int lineNum, int errNum, string _errToken);
    void MapInit();
    ErrorHandler();
private:
    vector<Error> errors;
    map<unsigned int, string> errorMap;
};

#endif // ERRORHANDLER_H_INCLUDED
