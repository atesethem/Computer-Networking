#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Packet.h"
#include "Client.h"
#include <stack>
#include <queue>
#include <chrono>
#include <iomanip>
using namespace std;

class Network {
public:
    Network();
    ~Network();


    // Executes commands given as a vector of strings while utilizing the remaining arguments.

    void process_commands(vector<Client>& clients, vector<string>& commands, int message_limit, const string& sender_port,
        const string& receiver_port);

    // Initialize the network from the input files.

    std::string sender_MAC;
    std::string receiver_MAC;
    Client* thesender;
    Client* thereceiver;
    Client* yedekalici;
    std::vector <std::vector<stack<Packet*>>> allmessages;
    vector<Client> read_clients(string const& filename);
    std::vector<std::string> stringBol(const std::string& cumle, int number);
    string getTextBetweenHashtags(const std::string& str);
    void read_routing_tables(vector<Client>& clients, string const& filename);
    vector<string> read_commands(const string& filename);
};

#endif  // NETWORK_H
