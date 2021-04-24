#ifndef CHECK_PERM
#define CHECK_PERM

#include <vector>
#include <string>

using namespace std;

/**
 * Return the list of absolute path of files which the user or group have an access.
 * @param path - path where the program checks files.
 * @param username - username for which the program will check file access.
 * @param group_name - group for which the program will check file access.
 * @return the list with absolute path of files which the user or group have an access.
 */
vector<string> checkPerm(string path, string username, string group_name);

#endif