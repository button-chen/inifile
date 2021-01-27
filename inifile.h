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
    T GetValue(T defval = T()) {
        if (m_strVal.empty()) {
            return defval;
        }
        return StringTo<T>(m_strVal);
    }
    template<typename T>
    vector<T> GetArray(string defval = string(), char sep = char()) {
        if (m_strVal.empty()) {
            m_strVal = defval;
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
		m_pre_comment = sec.m_pre_comment;
		m_next_comment = sec.m_next_comment;
		m_key_order = sec.m_key_order;
        return *this;
    }
    bool IsValid() { return !m_sectionName.empty(); }
    string GetName() { return m_sectionName; }
    map<string, KeyVal> GetData() { return m_sectionConf; }

	std::vector<std::string> m_key_order;
	map<string, std::vector<std::string> > m_pre_comment;
	map<string, std::vector<std::string> > m_next_comment;
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
	std::vector<string> m_order_sec;
};

} // end namespace ini
#endif
