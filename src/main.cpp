#include <iostream>
#include "zookeeper/connector.h"

using namespace std;

int main() {
    cout << "\nprogram started \n";
    ZooKeeperConnector zkConnector("localhost:2181", 10000);
    bool isConnected = zkConnector.connect();
    if(isConnected){
        cout << "\nreating node\n";
        zkConnector.createZNode("/testing", "manikant");
    }else{
        cout << "\nzookeeper not connected\n";
    }
    return EXIT_SUCCESS;
}