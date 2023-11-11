#include <iostream>
#include "zookeeper/connector.h"

using namespace std;

int main() {
    ZooKeeperConnector zkConnector("localhost:2181", 5000);
    zkConnector.connect();
    zkConnector.createZNode("/testing", "manikant");
    return EXIT_SUCCESS;
}