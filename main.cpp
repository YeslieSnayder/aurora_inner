#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <vector>
#include <iostream>

#include "checkPerm.h"

using namespace std;

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

    if (!group_name.empty()) {
        group *gr = getgrnam(group_name.c_str());
        setgid(gr->gr_gid);
    }
    if (!username.empty()) {
        passwd *user = getpwnam(username.c_str());
        setuid(user->pw_uid);
    }

    vector<string> paths = checkPerm(path, username, group_name);
    for (string &s : paths) {
        cout << s << endl;
    }
    return 0;
}
