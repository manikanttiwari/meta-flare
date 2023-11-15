#include <zookeeper.h>
#include "zookeeper.jute.h"
#include "string"
#include <iostream>
#include "connector.h"
#include <execinfo.h>
using namespace std;
void printStackTrace() {
    cout << "\nprinting the stack trace: ";
    const int maxFrames = 64;
    void* stack[maxFrames];
    int frames = backtrace(stack, maxFrames);
    char** symbols = backtrace_symbols(stack, frames);

    for (int i = 0; i < frames; ++i) {
        std::cout << i << ": " << symbols[i] << std::endl;
    }
    cout << "\nstack trace is printed";
    free(symbols);
}
void my_data_completion(int rc, const char *value, int value_len, const struct Stat *stat, const void *data) {
    if (rc == ZOK) {
        std::string *userData = static_cast<std::string *>(const_cast<void *>(data));
        userData->assign(value, value_len);
        std::cout << "Data: " << *userData << std::endl;

        // Continue processing or signaling completion
    } else {
        std::cerr << "Error in data retrieval: " << zerror(rc) << std::endl;
        // Handle error
    }
}
void my_completion(int rc, const char *value, const void *data) {
    cout << "\n\n======\n\n";
    cout << rc;
    cout << "====\n\n";
    if (rc == ZOK) {
        // Znode creation successful
        // Process the value, if needed
        cout << "Znode created successfully!" << endl;
    } else {
        // Handle error
        cerr << "Error creating znode: " << zerror(rc) << endl;
    }
}
ZooKeeperConnector::ZooKeeperConnector(const std::string &hosts, int timeout)
    : zh(nullptr), zkHosts(hosts), zkTimeout(timeout)
{
    zoo_set_debug_level(ZOO_LOG_LEVEL_DEBUG);
}

ZooKeeperConnector::~ZooKeeperConnector()
{
    if (zh)
    {
        zookeeper_close(zh);
    }
}

void ZooKeeperConnector::watcher(zhandle_t *zzh, int type, int state, const char *path, void *watcherCtx)
{
    // Watcher callback - can be implemented if needed
}

bool ZooKeeperConnector::connect()
{
    cout << "\ntrying to connect the zookeeper...\n";
    zh = zookeeper_init(zkHosts.c_str(), watcher, zkTimeout, 0, 0, 0);
    // cout << "===========================================";
    // cout << (zh != nullptr);
    return zh != nullptr;
}

bool ZooKeeperConnector::createZNode(const std::string &path, const std::string &data)
{
    if (!zh)
    {
        cout << "\nzookeeper handler is null\n";
        return false;
    }
    struct ACL CREATE_ONLY_ACL[] = {{ZOO_PERM_CREATE, ZOO_AUTH_IDS}};
    struct ACL_vector CREATE_ONLY = {1, CREATE_ONLY_ACL};
    cout << zh;
    cout << "\nready to create the node\n\n";
    try
    {
        int rc = zoo_acreate(zh, "/xyz", "value", 7, &CREATE_ONLY, ZOO_PERSISTENT_SEQUENTIAL, my_completion, nullptr);
        // int rc = zoo_acreate(zh, path.c_str(), data.c_str(), data.size(), &CREATE_ONLY, ZOO_PERSISTENT, my_completion, nullptr);
        cout << "\nresult of node creation: ";
        cout << rc;
        // printStackTrace();
        return rc == ZOK;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
    
    // int rc = zoo_acreate(zh, "/xyz", "value", 7, &CREATE_ONLY, ZOO_PERSISTENT, my_completion, nullptr);
    // // int rc = zoo_acreate(zh, path.c_str(), data.c_str(), data.size(), &CREATE_ONLY, ZOO_PERSISTENT, my_completion, nullptr);
    // cout << "\nresult of node creation: ";
    // cout << rc;
    // printStackTrace();
    // return rc == ZOK;
}

bool ZooKeeperConnector::readZNode(const std::string &path, std::string &data) {
    if (!zh) {
        return false;
    }

    char buffer[512];
    int buflen = sizeof(buffer);
    struct Stat stat;
    int rc = zoo_awget(zh, path.c_str(), watcher, nullptr, my_data_completion, &data);

    if (rc == ZOK) {
        data.assign(buffer, buflen);
        return true;
    } else {
        std::cerr << "Error " << rc << " for " << path << std::endl;
        return false;
    }
}