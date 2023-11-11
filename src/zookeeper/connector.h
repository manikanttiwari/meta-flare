#include <zookeeper.h>
#include "zookeeper.jute.h"
#include "string"
#include <iostream>


class ZooKeeperConnector
{
public:
    ZooKeeperConnector(const std::string &hosts, int timeout);
    ~ZooKeeperConnector();

    bool connect();
    bool createZNode(const std::string &path, const std::string &data);
    bool readZNode(const std::string &path, std::string &data);

private:
    zhandle_t *zh;
    std::string zkHosts;
    int zkTimeout;

    static void watcher(zhandle_t *zzh, int type, int state, const char *path, void *watcherCtx);
};
