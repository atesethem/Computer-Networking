#ifndef PHYSICAL_LAYER_PACKET_H
#define PHYSICAL_LAYER_PACKET_H

#include "Packet.h"
#include "Client.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Packet.h"
#include <stack>
#include <queue>
using namespace std;

// Extends Packet class. Have additional layer-specific member variables and overrides the virtual print function.
class PhysicalLayerPacket : public Packet {
public:
    PhysicalLayerPacket(int layerID, const string& senderMAC, const string& receiverMAC);
    ~PhysicalLayerPacket() override;
    string message_data;
    string sender_MAC_address;
    string receiver_MAC_address;
    string yedekaliciID;
    string yedekyollayiciID;
    string gercekalici;
    string gercekyollayici;
    int number_of_hops = 0;
    int framenumber = 1;
    void print() override;
};

#endif // PHYSICAL_LAYER_PACKET_H
