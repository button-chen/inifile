
# inifile 
- a tiny and simple ini parser for C++


## Example

```c++
/*
test.ini:

[system]   
param1=12
    
#支持多行模式  
param3=12, 13,14 ,15  
,18,19,20  

param5=true

*/ 

#include "inifile.h"  
#include <vector>  
using namespace std;  
  
int main()  
{  
    IniFile ini("./test.ini");  
  
    IniFile::section& sec = ini.GetSection("system");  
    // 读取配置  
    int pm1 = sec["param1"].GetAsInt(0);  
    vector<int> pm3 = sec["param3"].GetAsIntArray("", ',');
    bool pm5 = sec["param5"].GetAsBool(false); 
	
    // 更改配置  
    sec["param1"] = "99";
    
    // 添加配置节
    IniFile::section s("extend");  
    s["one"] = "12";  
    s["two"] = "chen:13"; 
    ini.PushSection(s);
  
    // 写文件
    ini.Flush("./test_new.ini");  
  
    return 0;  
}  
``` 

