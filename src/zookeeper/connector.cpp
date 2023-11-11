#include <zookeeper.h>
#include "zookeeper.jute.h"
#include "string"
#include <iostream>
#include "connector.h"
using namespace std;
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
    cout << "trying to connect the zookeeper...";
    zh = zookeeper_init(zkHosts.c_str(), watcher, zkTimeout, 0, 0, 0);
    return zh != nullptr;
}

bool ZooKeeperConnector::createZNode(const std::string &path, const std::string &data)
{
    if (!zh)
    {
        return false;
    }

    struct ACL CREATE_ONLY_ACL[] = {{ZOO_PERM_CREATE, ZOO_AUTH_IDS}};
    struct ACL_vector CREATE_ONLY = {1, CREATE_ONLY_ACL};

    int rc = zoo_acreate(zh, path.c_str(), data.c_str(), data.size(), &CREATE_ONLY, ZOO_PERSISTENT, nullptr, 0);
    return rc == ZOK;
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