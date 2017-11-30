#ifndef BNT_INI_PARSE_H_
#define BNT_INI_PARSE_H_

#include <string>
#include <map>
#include <strstream>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

namespace ini
{
void Trim(string & str);

class KeyVal
{
public:
    KeyVal() { m_strVal.clear(); }
    KeyVal(string strVal) :m_strVal(strVal) { }
    void operator= (const string & v) { m_strVal = v; }
    template<typename T>
    T StringTo(string val) {
        istrstream ins(val.c_str());
        T tmp;
        ins >> tmp;
        return tmp;
    }
    template<typename T>
    T GetValue(T default = T()) {
        if (m_strVal.empty()) {
            return default;
        }
        return StringTo<T>(m_strVal);
    }
    template<typename T>
    vector<T> GetArray(string default = string(), char sep = char()) {
        if (m_strVal.empty()) {
            m_strVal = default;
        }
        vector<T> vRet;
        istringstream iss(m_strVal);
        string toke;
        while (getline(iss, toke, sep)) {
            Trim(toke);
            vRet.push_back(StringTo<T>(toke));
        }
        return vRet;
    }
    void Set(string other) { m_strVal = other; }
    string Get() { return m_strVal; }

private:
    string m_strVal;
};

class Section
{
public:
    Section() { m_sectionName.clear(); }
    Section(string secName) : m_sectionName(secName) { }
    KeyVal& operator [](string key) { return m_sectionConf[key]; }
    Section & operator= (const Section & sec) {
        if (&sec == this) {
            return *this;
        }
        m_sectionName = sec.m_sectionName;
        m_sectionConf = sec.m_sectionConf;
        return *this;
    }
    bool IsValid() { return !m_sectionName.empty(); }
    string GetName() { return m_sectionName; }
    map<string, KeyVal> GetData() { return m_sectionConf; }

private:
    string   m_sectionName;
    map<string, KeyVal> m_sectionConf;
};

class IniFile
{
public:
    IniFile();
    explicit IniFile(const string& ConfigFile);
    ~IniFile();

public:
    bool Load(string ConfigFile);
    Section & GetSection(string StrSecName);
    bool PushSection(Section & sec);
    bool Flush(string savepath = "");

private:
    string GetSectionName(string inputLine);
    string GetKey(string inputLine);
    string GetValue(string inputLine);

private:
    string m_fileName;
    map<string, Section> m_mapConfig;
};

} // end namespace ini
#endif
