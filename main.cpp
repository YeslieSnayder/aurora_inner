#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <vector>
#include <iostream>
#include <algorithm>

#include "checkPerm.h"

using namespace std;

/**
 * Check the parameters from launching the application. Return the correctness of the input parameters.
 * @param argc - the amount of parameters.
 * @param argv - the array of parameters.
 * @param path - string that is represented a path.
 * @param username - string that is represented a username.
 * @param group - string that is represented name of a group.
 * @return If parameters are correct => true, otherwise => false.
 */
bool initParams(int argc, const char *argv[], string &path, string &username, string &group) {
    if (argc < 2) {
        cout << "The call has to contain parameters:\n\t" << argv[0] << " -p path -u username -g groupname" << endl;
        return false;
    }

    std::vector<std::string> all_args;
    all_args.assign(argv + 1, argv + argc);
    for (int i = 0; i < argc-1; i++) {
        if (all_args[i] == "-u") {
            username.assign(all_args[++i]);
        } else if (all_args[i] == "-g") {
            group.append(all_args[++i]);
        } else if (all_args[i] == "-p") {
            path.append(all_args[++i]);
        }
    }
    if (path.empty()) {
        cout << "The call has to contain a path explicitly:\n\t" << argv[0] << " -p path" << endl;
        return false;
    }
    if (username.empty() && group.empty()) {
        cout << "The call has to contain minimum 1 parameter for username (-u name) or group_name (-g group)" << endl;
        return false;
    }
    return true;
}

int main(int argc, const char *argv[]) {
    string path, username, group_name;
    if (!initParams(argc, argv, path, username, group_name))
        return -1;

    try {
        if (!group_name.empty()) {
            group *gr = getgrnam(group_name.c_str());
            setgid(gr->gr_gid);
        }
    } catch (exception &e) {
        cout << "The group: " << group_name << " does not exist!" << endl;
    }

    try {
        if (!username.empty()) {
            passwd *user = getpwnam(username.c_str());
            setuid(user->pw_uid);
        }
    } catch (exception &e) {
        cout << "The user: " << username << " does not exist!" << endl;
    }

    vector<string> paths = checkPerm(path, !username.empty(), !group_name.empty());
    sort(paths.begin(), paths.end());
    for (string &s : paths) {
        cout << s << endl;
    }
    return 0;
}
