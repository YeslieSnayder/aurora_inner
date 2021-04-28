#include <sys/stat.h>
#include <filesystem>
#include "checkPerm.h"
using std::filesystem::directory_iterator;

string getPermission(struct stat &params, bool &has_user, bool &has_group) {
    string res;
    if (!has_user && !has_group ||
            has_user && (params.st_mode & S_IWUSR) == 0 ||
            has_group && (params.st_mode & S_IWGRP) == 0) // no access
        return "";

    switch (params.st_mode & S_IFMT) {  // type of a file
        case S_IFREG: res.append("f "); break; // regular file
        case S_IFDIR: res.append("d "); break; // directory
        case S_IFLNK: return "";    // soft link
    }
    return res;
}

vector<string> getFilesInDir(string &dir) {
    vector<string> res;
    for (const auto & file : directory_iterator(dir))
        res.push_back(file.path());
    return res;
}

vector<string> checkPerm(string path, bool has_user, bool has_group) {
    vector<string> res;
    struct stat params{};
    lstat(path.c_str(), &params);
    string perm = getPermission(params, has_user, has_group);
    if (perm.empty())
        return {};

    res.push_back(perm + path);

    if (res[0][0] == 'd') {
        vector<string> files = getFilesInDir(path);
        for (string &s : files) {
            vector<string> paths = checkPerm(s, has_user, has_group);
            for (string &p : paths)
                res.push_back(p);
        }
    }

    return res;
}
