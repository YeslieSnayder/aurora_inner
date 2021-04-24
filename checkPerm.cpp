#include <sys/stat.h>
#include <filesystem>
#include "checkPerm.h"
using std::filesystem::directory_iterator;

string getPermission(struct stat &params, string &username, string &group_name) {
    string res;
    if ((params.st_mode & S_IWUSR) == 0 && (params.st_mode & S_IWGRP) == 0) // no access
        return "no access";

    switch (params.st_mode & S_IFMT) {  // type of a file
        case S_IFREG: res.append("f "); break; // regular file
        case S_IFDIR: res.append("d "); break; // directory
        case S_IFLNK: res.append("l "); break; // soft link
    }

    if (S_ISDIR(params.st_mode)) {
        // recursive call
    }

    res.append(to_string(params.st_uid));
    res.append(" ");
    res.append(to_string(params.st_gid));
    return res;
}

vector<string> getFilesInDir(string &dir) {
    vector<string> res;
    for (const auto & file : directory_iterator(dir))
        res.push_back(file.path());
    return res;
}

vector<string> checkPerm(string path, string username, string group_name) {
    vector<string> res;
    struct stat params;
    lstat(path.c_str(), &params);
    res.push_back(getPermission(params, username, group_name));

    vector<string> files = getFilesInDir(path);
    for (string &s : files) {
        lstat(s.c_str(), &params);
        res.push_back(getPermission(params, username, group_name));
    }

    return res;
}
