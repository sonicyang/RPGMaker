#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <fstream>
#include <string>
#include <cerrno>
#include <vector>
#include <deque>
#include "item.h"
#include "enum.h"
#include "skill.h"

#define paraVarType int, unsigned int, wchar_t*, std::string, Stats, svc, std::vector<std::string>, Skill

std::string get_file_contents(const char*);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

#endif // UTILS_H_INCLUDED
