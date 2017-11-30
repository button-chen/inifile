#include "inifile.h"
#include <fstream>
#include <iostream>
#include <locale>

namespace ini
{

IniFile::IniFile(const string &ConfigFile)
    :m_fileName(ConfigFile){
    Load(m_fileName);
}

IniFile::IniFile(){ }

IniFile::~IniFile(void) { }

// value值支持多行模式
bool IniFile::Load(string config)
{
    m_fileName = config;
    ifstream in(m_fileName.c_str());
    if (!in) {
        cout << "配置文件不存在" << endl;
        return false;
    }
    string line, secName, lastSecName;
    while (!in.eof()){
        getline(in, line);
        secName = GetSectionName(line);
        if (secName.empty()){
            continue;
        }
        break;
    }
    lastSecName = secName;
    Section secConf;
    string strline;
    do
    {
    __reboot:
        Trim(line);
        if (line.empty() || line.find(';') == 0 || line.find('#') == 0){
            continue;
        }
        secName = GetSectionName(line);
        if (!secName.empty()){
            if (lastSecName != secName){
                m_mapConfig[lastSecName] = secConf;
                lastSecName = secName;
            }
            secConf = Section(secName);
            continue;
        }
        strline.clear();
        strline += line;
        bool  flag = false;
        while (getline(in, line)){
            Trim(line);
            if (line.empty() || line.find(';') == 0 || line.find('#') == 0){
                break;
            }
            string tmp = GetSectionName(line);
            if (!tmp.empty()){
                flag = true;
                break;
            }
            if (line.find('=') == string::npos){
                strline += line;
            }
            else{
                flag = true;
                break;
            }
        }
        string key = GetKey(strline);
        string svalue = GetValue(strline);
        secConf[key] = svalue;
        if (flag){
            goto __reboot;
        }

    } while (getline(in, line));
    m_mapConfig[lastSecName] = secConf;
    return true;
}

Section & IniFile::GetSection(const string strSecName)
{
    map<string, Section>::iterator itr = m_mapConfig.find(strSecName);
    if (itr != m_mapConfig.end()){
        return itr->second;
    }
    return m_mapConfig[strSecName];
}

string IniFile::GetSectionName(string inputLine)
{
    size_t sec_begin_pos = inputLine.find('[');
    if (sec_begin_pos == string::npos || sec_begin_pos != 0){
        return string();
    }
    size_t sec_end_pos = inputLine.find(']', sec_begin_pos);
    if (sec_end_pos == string::npos){
        return string();
    }

    inputLine = (inputLine.substr(sec_begin_pos + 1, sec_end_pos - sec_begin_pos - 1));
    Trim(inputLine);
    return inputLine;
}

string IniFile::GetKey(string inputLine)
{
    size_t keyPos = inputLine.find('=');
    if (keyPos == string::npos){
        return string();
    }
    string key = inputLine.substr(0, keyPos);
    Trim(key);
    return key;
}

string IniFile::GetValue(string inputLine)
{
    size_t keyPos = inputLine.find('=');
    if (keyPos == string::npos){
        return string();
    }
    size_t notePos1 = inputLine.find(';');
    size_t notePos2 = inputLine.find('#');
    size_t pos = (notePos1 != string::npos && notePos1 <= notePos2) ? notePos1 : notePos2;
    pos = (pos == string::npos) ? inputLine.size() : pos;
    string svalue = inputLine.substr(keyPos + 1, pos - keyPos - 1);
    Trim(svalue);
    return svalue;
}

bool IniFile::Flush(string savepath)
{
    ofstream ou;
    if (savepath.empty()) {
        ou.open(m_fileName.c_str());
    }
    else {
        ou.open(savepath.c_str());
    }
    ou.clear();
    map<string, Section>::iterator itr = m_mapConfig.begin();
    for (itr; itr != m_mapConfig.end(); itr++){
        ou << "[" << itr->first << "]" << endl;
        map<string, KeyVal> data = itr->second.GetData();
        map<string, KeyVal>::iterator it = data.begin();
        for (it; it != data.end(); it++){
            ou << it->first << " = " << it->second.Get() << endl;
        }
        ou << endl;
    }
    ou.close();
    return true;
}

bool IniFile::PushSection(Section & sec)
{
    m_mapConfig[sec.GetName()] = sec;
    return true;
}

void Trim(string & str)
{
    int poss = -1, pose = -1;
    for (int i = 0; i < str.size(); i++) {
        if (isspace(str[i], std::locale(".OCP")))
            continue;
        poss = i;
        break;
    }
    for (int i = str.size() - 1; i >= 0; i--) {
        if (isspace(str[i], std::locale(".OCP")))
            continue;
        pose = i;
        break;
    }
    if (poss != -1 && pose != -1) {
        str = str.substr(poss, pose + 1);
    }
    else {
        str.clear();
    }
}

}; // end namespace ini

