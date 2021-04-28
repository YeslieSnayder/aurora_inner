#include <sys/stat.h>
#include <filesystem>
#include "checkPerm.h"
using std::filesystem::directory_iterator;

/**
 * Return the type of a file if the user or group has an access to it.
 * @param params - struct with parameters of a file.
 * @param has_user - flag for checking access by user.
 * The user has to be effective during calling this function.
 * @param has_group - flag for checking access by group.
 * The group has to be effective during calling this function.
 * @return string that is represented the type of a file.
 * if user or group does not have an access to the file, then return empty string.
 */
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

/**
 * Return all files in directory from input.
 * @param dir - path to target directory.
 * @return stl_vector with all files from directory.
 */
vector<string> getFilesInDir(string &dir) {
    vector<string> res;
    for (const auto & file : directory_iterator(dir))
        res.push_back(file.path());
    return res;
}

/**
 * Return the list of absolute path and type of files/directories which the user or group have an access.
 * @param path - path where the program checks files.
 * @param has_user - flag for existing effective user.
 * @param has_group - group for which the program will check file access.
 * @return the list with absolute path and type of files which the user or group have an access.
 */
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
