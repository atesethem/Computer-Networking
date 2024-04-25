#include "Network.h"

Network::Network() {

}

void Network::process_commands(vector<Client>& clients, vector<string>& commands, int message_limit,
    const string& sender_port, const string& receiver_port) {

    for (int i = 0; i < commands.size(); i++) {
        std::cout << "--------------------------------------------------------------------------------------" << "\n";
        std::cout << "Command: " << commands[i] << "\n";
        std::cout << "--------------------------------------------------------------------------------------" << "\n";
        std::istringstream iss(commands[i]);
        std::vector<std::string> words;
        std::string word;
        while (iss >> word) {
            words.push_back(word);
        }

        if (words[0] == "MESSAGE") {
            for (int j = 0; j < clients.size(); j++) {
                if (words[1] == clients[j].client_id) {
                    sender_MAC = clients[j].client_mac;
                    thesender = &clients[j];
                    if (thesender->routing_table.find(words[2]) != thesender->routing_table.end()) {
                        std::string theid = thesender->routing_table[words[2]];
                        for (int k = 0; k < clients.size(); k++) {
                            if (theid == clients[k].client_id) {
                                receiver_MAC = clients[k].client_mac;
                                yedekalici = &clients[k];

                            }
                        }
                    }
                }
                if (words[2] == clients[j].client_id) {
                    thereceiver = &clients[j];
                }
            }
            int frame = 0;
            std::string ara_metin = getTextBetweenHashtags(commands[i]);
            std::vector<std::string> partsofstring = stringBol(ara_metin, message_limit);
            std::vector<stack<Packet*>> themessage;
            std::cout << "Message to be sent: " << "\"" << ara_metin << "\"" << "\n" << "\n";
            int number = 1;
            for (std::string part : partsofstring) {
                std::stack<Packet*> messagepart;

                ApplicationLayerPacket* applayer = new ApplicationLayerPacket(0, thesender->client_id, thereceiver->client_id, part);
                TransportLayerPacket* translayer = new TransportLayerPacket(1, sender_port, receiver_port);
                NetworkLayerPacket* networklayer = new NetworkLayerPacket(2, thesender->client_ip, thereceiver->client_ip);
                PhysicalLayerPacket* physicallayer = new PhysicalLayerPacket(3, sender_MAC, receiver_MAC);
                physicallayer->message_data = part;
                messagepart.push(applayer);
                messagepart.push(translayer);
                messagepart.push(networklayer);
                messagepart.push(physicallayer);
                themessage.push_back(messagepart);
                thesender->outgoing_queue.push(messagepart);
                physicallayer->framenumber = number;
                physicallayer->yedekaliciID = yedekalici->client_id;
                physicallayer->yedekyollayiciID = thesender->client_id;
                physicallayer->gercekalici = thereceiver->client_id;
                physicallayer->gercekyollayici = thesender->client_id;
                std::cout << "Frame #" << number << "\n";
                std::cout << "Sender MAC address: " << sender_MAC << ", Receiver MAC address: " << receiver_MAC << "\n";
                std::cout << "Sender IP address: " << thesender->client_ip << ", Receiver IP address: " << thereceiver->client_ip << "\n";
                std::cout << "Sender port number: " << sender_port << ", Receiver port number :" << receiver_port << "\n";
                std::cout << "Sender ID: " << thesender->client_id << ", Receiver ID: " << thereceiver->client_id << "\n";
                std::cout << "Message chunk carried: " << "\"" << part << "\"" << "\n";
                std::cout << "Number of hops so far: " << physicallayer->number_of_hops << "\n";
                number++;

            } auto now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
            std::tm tm_now = *std::localtime(&now_c);
            std::stringstream ss;
            ss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
            std::string str_time = ss.str();
            ActivityType activityType = ActivityType::MESSAGE_SENT;
            Log log(str_time, ara_metin,0, 0, thesender->client_id, thereceiver->client_id, true, ActivityType::MESSAGE_SENT);
            thesender->log_entries.push_back(log);
           
        }
     
        else if (words[0] == "SHOW_FRAME_INFO") {
            Client* X = nullptr;
            for (int j = 0; j < clients.size(); j++) {
                if (words[1] == clients[j].client_id) {
                    X = &clients[j];
                    break;
                }
            }
            if (X != nullptr) {
                int number = std::stoi(words[3]);
                if (words[2] == "in") {
                    if (number - 1 > X->incoming_queue.size()) {
                        std::cout << "No such frame." << endl;
                    }
                    else {
                        queue<stack<Packet*>> yedek;
                        std::cout << "Current Frame #" << number << " on the outgoing queue of client " << X->client_id << endl;

                        yedek = X->incoming_queue;

                        int currentFrame = 1;
                        while (!yedek.empty()) {
                            stack<Packet*> tempStack = yedek.front();
                            yedek.pop();

                            if (currentFrame == number) {
                                std::stack<Packet*> yedek2;

                                while (!tempStack.empty()) {
                                    yedek2.push(tempStack.top());
                                    tempStack.pop();
                                }
                                int thenumber = 0;
                                while (!yedek2.empty()) {
                                    Packet* topPacket = yedek2.top();
                                    if (topPacket->layer_ID == 0) {
                                        ApplicationLayerPacket* physPacket = dynamic_cast<ApplicationLayerPacket*>(topPacket);
                                        std::cout << "Carried Message: " << "\"" << physPacket->message_data << "\"" << endl;
                                    }

                                    std::cout << "Layer " << thenumber << " info: ";
                                    if (topPacket->layer_ID != 0) { topPacket->print(); }

                                    if (topPacket->layer_ID == 0) {
                                        ApplicationLayerPacket* physPacket = dynamic_cast<ApplicationLayerPacket*>(topPacket);
                                        topPacket->print();
                                        std::cout << "Number of hops so far: " << physPacket->number_of_hops << "\n";
                                    }

                                    yedek2.pop();
                                    thenumber++;

                                }

                                break;
                            }

                            ++currentFrame;
                        }
                    }
                }
                if (words[2] == "out") {
                    if (number - 1 > X->outgoing_queue.size()) {
                        std::cout << "No such frame." << endl;
                    }
                    else {
                        queue<stack<Packet*>> yedek;
                        std::cout << "Current Frame #" << number << " on the outgoing queue of client " << X->client_id << endl;

                        yedek = X->outgoing_queue;

                        int currentFrame = 1;
                        while (!yedek.empty()) {
                            stack<Packet*> tempStack = yedek.front();
                            yedek.pop();

                            if (currentFrame == number) {
                                std::stack<Packet*> yedek2;

                                while (!tempStack.empty()) {
                                    yedek2.push(tempStack.top());
                                    tempStack.pop();
                                }
                                int thenumber = 0;
                                while (!yedek2.empty()) {
                                    Packet* topPacket = yedek2.top();
                                    if (topPacket->layer_ID == 0) {
                                        ApplicationLayerPacket* physPacket = dynamic_cast<ApplicationLayerPacket*>(topPacket);
                                        std::cout << "Carried Message: " << "\"" << physPacket->message_data << "\"" << endl;
                                    }

                                    std::cout << "Layer " << thenumber << " info: ";
                                    if (topPacket->layer_ID != 0) { topPacket->print(); }

                                    if (topPacket->layer_ID == 0) {
                                        ApplicationLayerPacket* physPacket = dynamic_cast<ApplicationLayerPacket*>(topPacket);
                                        topPacket->print();
                                        std::cout << "Number of hops so far: " << physPacket->number_of_hops << "\n";
                                    }

                                    yedek2.pop();
                                    thenumber++;

                                }

                                break;
                            }

                            ++currentFrame;
                        }
                    }
                }
            }
        }
        else if (words[0] == "SHOW_Q_INFO") {
            Client* X = nullptr;
            for (int j = 0; j < clients.size(); j++) {
                if (words[1] == clients[j].client_id) {
                    X = &clients[j];
                    break;
                }
            }
            if (X != nullptr) {
                if (words[2] == "out") {
                    std::cout << "Client " << words[1] << " Outgoing Queue Status" << endl;
                    std::cout << "Current total number of frames: " << X->outgoing_queue.size() << endl;
                }
                if (words[2] == "in") {
                    std::cout << "Client " << words[1] << " Incoming Queue Status" << endl;
                    std::cout << "Current total number of frames: " << X->incoming_queue.size() << endl;
                }
            }
        }
        else if (words[0] == "SEND") {
            string theid;
            for (int j = 0; j < clients.size(); j++) {
                if (clients[j].outgoing_queue.size() != 0) {
                    while (!clients[j].outgoing_queue.empty()) {
                        int number = 0;

                        if (clients[j].outgoing_queue.front().top()->layer_ID = 3) {
                            PhysicalLayerPacket* physPacket = dynamic_cast<PhysicalLayerPacket*>(clients[j].outgoing_queue.front().top());
                            physPacket->number_of_hops++;
                            theid = physPacket->yedekaliciID;
                            number = physPacket->framenumber;
                            physPacket->number_of_hops++;
                            std::cout << physPacket->yedekaliciID;
                            std::cout << physPacket->yedekyollayiciID;
                        }
                        std::cout << "Client " << clients[j].client_id << " sending frame #" << number << " to client " << theid << endl;
                        queue<stack<Packet*>> theyedek = clients[j].outgoing_queue;
                        while (!theyedek.front().empty()) {
                            theyedek.front().top()->print();
                           theyedek.front().pop();
                        }
                        std::cout << "\n";
                        for (int k = 0; k < clients.size(); k++) {
                            if (theid == clients[k].client_id) {
                                clients[k].incoming_queue.push(clients[j].outgoing_queue.front());
                                
                            }                                             
                        }                       
                        clients[j].outgoing_queue.pop();

                    }
                }

            }
            
        }
       
        else if (words[0] == "PRINT_LOG") {
            Client* theclient;
            for (Client client : clients) {
                if (words[1] == client.client_id) {
                    theclient = &client;
                }
            }
            if (theclient->log_entries.empty()) {
                std::cout << "Client " << theclient->client_id << "Logs" << endl;
            }
            for (int i = 0; i < theclient->log_entries.size(); ++i) {
                string activity;
                Log entries = theclient->log_entries[i];
                if (entries.activity_type == ActivityType::MESSAGE_SENT) {
                    activity = "Message Sent";
                }
                else if (entries.activity_type == ActivityType::MESSAGE_DROPPED) {
                    activity = "Message Dropped";
                }
                else if (entries.activity_type == ActivityType::MESSAGE_FORWARDED) {
                    activity = "Message Forwarded";
                }
                std::cout << "--------------\nLog Entry #" << i + 1 << ":\nActivity: " << activity << "\nTimestamp: " << entries.timestamp << endl;
                std::cout << "Number of frames: " << entries.number_of_frames << "\nNumber of hops: " << entries.number_of_hops << endl;
                std::cout << "Sender ID: " << entries.sender_id << "\nReceiver ID: " << entries.receiver_id << endl;
                std::cout << "Success: " << entries.success_status << "\nMessage: " << '"' << entries.message_content << '"' << endl;
            }


        }
   


          // TODO: Execute the commands given as a vector of strings while utilizing the remaining arguments.
          /* Don't use any static variables, assume this method will be called over and over during testing.
           Don't forget to update the necessary member variables after processing each command. For example,
           after the MESSAGE command, the outgoing queue of the sender must have the expected frames ready to send. */
    }
}
std::vector<std::string> Network::stringBol(const std::string& cumle, int number) {
    std::vector<std::string> bolunmus_kelimeler;
    size_t uzunluk = cumle.length();

    for (size_t i = 0; i < uzunluk; i += number) {
        bolunmus_kelimeler.push_back(cumle.substr(i, number));
    }

    return bolunmus_kelimeler;
}
vector<Client> Network::read_clients(const string& filename) {
    vector<Client> clients;
    std::ifstream file(filename);

    if (file.is_open()) {

        std::string line;
        while (std::getline(file, line)) {

            std::vector<std::string> words;
            std::istringstream stream(line);
            std::string word;
            while (stream >> word) {
                words.push_back(word);
            }
            if (words.size() == 3) {

                Client newclient(words[0], words[1], words[2]);
                clients.push_back(newclient);
            }
        }
    }
    return clients;
}

void Network::read_routing_tables(vector<Client>& clients, const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        string senderID, receiverID;
        int number = 0;
        while (getline(file, line)) {
            if (line == "-") {
                number++;
                continue;

            }
            istringstream stream(line);
            stream >> senderID >> receiverID;
            clients[number].routing_table[senderID] = receiverID;

        }
        file.close();
    }
}

vector<string> Network::read_commands(const string& filename) {
    vector<string> commands;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            commands.push_back(line);
        }
        file.close();
    }
    return commands;
    // TODO: Read commands from the given input file and return them as a vector of strings.

}

std::string Network::getTextBetweenHashtags(const std::string& str) {
    size_t startPos = str.find('#');
    size_t endPos = str.find('#', startPos + 1);
    if (startPos != string::npos && endPos != string::npos && endPos > startPos) {
        return str.substr(startPos + 1, endPos - startPos - 1);
    }

    return "";
}




Network::~Network() {
    for (auto& message : allmessages) {
        for (auto& frame : message) {
            while (!frame.empty()) {
                Packet* packet = frame.top();
                frame.pop();


                delete packet;
            }
        }
    }
}
