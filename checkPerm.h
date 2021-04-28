#ifndef CHECK_PERM
#define CHECK_PERM

#include <vector>
#include <string>

using namespace std;

/**
 * Return the list of absolute path and type of files/directories which the user or group have an access.
 * @param path - path where the program checks files.
 * @param username - username for which the program will check file access.
 * @param group_name - group for which the program will check file access.
 * @return the list with absolute path and type of files which the user or group have an access.
 */
vector<string> checkPerm(string path, bool has_user, bool has_group);

#endif