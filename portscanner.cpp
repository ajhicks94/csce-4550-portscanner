#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::regex;
using std::regex_match;
using std::cmatch;
using std::ifstream;
using std::to_string;
using std::stringstream;
using std::sort;
using std::unique;
using std::stoi;

void printUsage()
{
    cout << "\nportScan: usage:" << endl;
    cout << "          ./portScan --help" << endl;
    cout << "          ./portScan [OPTIONS] [IP]" << endl;
    cout << "          ./portScan [--port] [--ip] [--file] [--transport]" << endl << endl;
    cout << "          NOTE: If neither --ip nor --file is used, provide one IP address" << endl << endl;
    cout << "          OPTIONS" << endl;
    cout << "          --help" << endl;
    cout << "                      display options and accepted input format" << endl;
    cout << "          --port" << endl;
    cout << "                      scan designated ports" << endl;
    cout << "                      defaults to 1-1024" << endl;
    cout << "                      accepted range: 1-65535" << endl;
    cout << "                      accepted formats:" << endl;
    cout << "                          single port:    --port 23" << endl;
    cout << "                          range of ports: --port 2000-3000" << endl;
    cout << "                          list of ports:  --port 21,80,443" << endl;
    cout << "                                          NOTE: no space between values and delimiters" << endl;
    cout << "          --ip" << endl;
    cout << "                      scan designated ip address" << endl;
    cout << "                      accepted formats:" << endl;
    cout << "                          single ip:      --ip 129.120.93.94" << endl;
    cout << "                          range of ips:   --ip 129.120.93.87-200" << endl;
    cout << "                          list of ips:    --ip 129.120.95.101,129.121.96.40" << endl;
    cout << "                                          NOTE: no space between values and delimiters" << endl;
    cout << "          --file" << endl;
    cout << "                      scan ip addresses in file" << endl;
    cout << "          --transport" << endl;
    cout << "                      scan designed transport protocol" << endl;
    cout << "                      values = [TCP,UDP]  --transport UDP" << endl;
    cout << "                      defaults to both" << endl;
}

bool parseOptions(int argc, char* argv[], vector<int> &ports, vector<string> &ips, vector<string> &protocols)
{
    bool single_ip = false;

    string filename = "";

    //vector<int> ports;
    //vector<string> ips;
    //vector<string> protocols;

    cmatch match;

    // Matches a single "port" "0", "99999", we bounds check later
    regex re_port ("\\b([0-9]{1,5})\\b");

    // Matches a port range "0-0", "5000-2122"
    regex re_port_range ("\\b([0-9]{1,5})-([0-9]{1,5})\\b");

    // Matches one port followed by a comma followed by a digit followed by anything
    regex re_port_list ("\\b([0-9]{1,5})\\b,\\d.*");

    // Matches a single ip address
    regex re_ip ("\\b(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\."
    "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\."
    "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\."
    "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\b");

    // Matches a range of ip addresses on the same subnet
    // 129.120.93.87-95
    regex re_ip_range ("\\b(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\."
    "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\."
    "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\."
    "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])-(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\b");

    // Matches one ip addresses followed by a comma followed by a digit followed by anything
    // 129.120.93.85,1
    // Just used to determine the mode of parsing
    regex re_ip_list ("\\b(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\."
    "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\."
    "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\."
    "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\b,\\d.*");

    // Loop through all the arguments
    for(size_t i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "--help") == 0)
        {
            printUsage();
            return false;
        }
        else if(strcmp(argv[i], "--port") == 0)
        {
            // add ports to port list
            if(i + 1 != argc)
            {
                // add list of ports
                if(regex_match(argv[i+1], re_port_list))
                {
                    vector<string> split;
                    stringstream ss (argv[i+1]);

                    // Split argument by commas
                    while(ss.good())
                    {
                        string substr;
                        getline(ss, substr, ',');
                        split.push_back(substr);
                    }

                    // loop through each port
                    for(int j = 0; j < split.size(); j++)
                    {
                        if(regex_match(split[j], re_port))
                        {
                            ports.push_back(stoi(split[j]));
                        }
                        else
                        {
                            cout << "invalid port: " << split[j] << endl;
                            return false;
                        }
                    }
                }
                // add single port
                else if(regex_match(argv[i+1], re_port))
                {
                    ports.push_back(stoi(argv[i+1]));                    
                }
                // add range of ports
                else if(regex_match(argv[i+1], match, re_port_range))
                {
                    int begin = stoi(match[1].str());
                    int end = stoi(match[2].str());
                    int range = end - begin;

                    if(end < begin)
                    {
                        cout << "invalid port range: " << begin << "-" << end << endl;
                        return false;
                    }

                    for(int j = 0; j <= range; j++)
                    {
                        ports.push_back(begin + j);
                    }
                }
                else
                {
                    cout << "invalid port: " << argv[i+1] << endl;
                    return false;
                }

                i++;
            }
            else
            {
                cout << "missing arg: " << argv[i] << endl;
                return false;
            }
            // or add port range to port list (decode range)
        }
        else if(strcmp(argv[i], "--ip") == 0)
        {
            if(!single_ip)
            {
                if(i + 1 != argc)
                {
                    // matches list of ips
                    if(regex_match(argv[i+1], re_ip_list))
                    {
                        // Split the argument by commas
                        vector<string> split;
                        stringstream ss(argv[i+1]);

                        while(ss.good())
                        {
                            string substr;
                            getline(ss, substr, ',');
                            split.push_back(substr);
                        }

                        for(int j = 0; j < split.size(); j++)
                        {
                            if(regex_match(split[j], re_ip))
                            {
                                ips.push_back(split[j]);
                            }
                            else
                            {
                                cout << "invalid ip: " << split[j] << endl;
                                return false;
                            }
                        }
                    }
                    // matches single ip address
                    else if(regex_match(argv[i+1], re_ip))
                    {
                        if(!single_ip)
                        {
                            ips.push_back(argv[i+1]);
                            single_ip = true;
                        }
                        else
                        {
                            cout << argv[i] << ": individual address has already been supplied" << endl;
                            return false;
                        }
                        
                    }
                    // matches ip address range
                    else if(regex_match(argv[i+1], match, re_ip_range))
                    {
                        int begin = stoi(match[4].str());
                        int end = stoi(match[5].str());
                        int range = end - begin;

                        if(end < begin)
                        {
                            cout << "invalid address range: " << begin << "-" << end << endl;
                            return false;
                        }

                        string base = match[1].str() + "." + match[2].str() + "." + match[3].str() + ".";
                        for(int j = 0; j <= range; j++)
                        {
                            ips.push_back(base + to_string(begin + j));
                        }
                    }
                    else
                    {
                        cout << "invalid ip: " << argv[i+1] << endl;
                        return false;
                    }

                    i++;
                }
                else
                {
                    cout << "missing arg: " << argv[i] << endl;
                    return false;
                }
            }
            else
            {
                cout << argv[i] << ": individual address has already been supplied" << endl;
                return false;
            }
        }
        else if(strcmp(argv[i], "--file") == 0)
        {
            // add ips from file to ip list
            if(i + 1 != argc)
            {
                // get filename
                filename = argv[i+1];

                // parse ips from file and add them to ip list
                ifstream ifs (filename);

                if(!ifs.is_open())
                {
                    cout << "problem opening file: " << filename << endl;
                    return false;
                }

                string ip;

                while(ifs >> ip)
                {
                    ips.push_back(ip);
                }

                ifs.close();
                i++;
            }
            else
            {
                cout << "missing arg: " << argv[i] << endl;
                return false;
            }
        }
        else if(strcmp(argv[i], "--transport") == 0)
        {
            // Add to protocol list
            if(i + 1 != argc)
            {
                protocols.push_back(argv[i+1]);
                i++;
            }
            else
            {
                cout << "missing arg: " << argv[i] << endl;
                return false;
            }
        }
        else if(regex_match(argv[i], re_ip))
        {
            // Allow for one individual address without the presence of flags
            if(!single_ip)
            {
                ips.push_back(argv[i]);
                single_ip = true;
            }
            else
            {
                cout << argv[i] << ": individual address has already been supplied" << endl;
                return false;
            }
        }
        else
        {
            cout << "Unknown argument: " << argv[i] << endl;
            return false;
        }
    }

    // Check for any duplicate ips in ip list and remove them
    sort(ips.begin(), ips.end());
    ips.erase(unique(ips.begin(), ips.end()), ips.end());

    // Check port bounds
    cout << endl;
    for(int i = 0; i < ports.size(); i++)
    {
        if(ports[i] < 1 || ports[i] > 65535)
        {
            cout << "port out of range: " << ports[i] << endl;
            cout << "accepted range: 1-65535" << endl;
            return false;
        }
    }

    // Technically this isn't possible, but I'm going to leave it in
    if(protocols.size() > 2)
    {
        cout << "too many protocols, max of 2" << endl;
        return false;
    }

    // Check if protocol is in accepted list
    for(int i = 0; i < protocols.size(); i++)
    {
        if(protocols[i] != "TCP" && protocols[i] != "tcp" && protocols[i] != "UDP" && protocols[i] != "udp")
        {
            cout << "unknown protocol: " << protocols[i] << endl;
            return false;
        }
        else if(protocols[i] == "TCP")
        {
            protocols[i] = "tcp";
        }
        else if(protocols[i] == "UDP")
        {
            protocols[i] = "udp";
        }
    }

    // Check if no ip was specified
    if(ips.empty())
    {
        cout << "no ip specified, use --ip and/or --file" << endl;
        return false;
    }
    
    /******************* POPULATE DEFAULTS *******************/
    if(ports.empty())
    {
        cout << "no ports specified, defaulting: 1-1024" << endl;
        for(int i = 1; i <= 1024; i++)
        {
            ports.push_back(i);
        }
    }

    if(protocols.empty())
    {
        cout << "no protocol specified, defaulting: [tcp,udp]" << endl;
        protocols.push_back("tcp");
        protocols.push_back("udp");
    }

    cout << endl;
    return true;
}

bool scan(int port, string ip, string protocol)
{
    int sockfd;
    int type;
    int pro;
    char buffer[256];

    struct sockaddr_in serv_addr;
    struct hostent *server;

    short int fd = -1;
    struct timeval tv;
    fd_set fdset;
    int rc;
    int so_error;
    socklen_t len;
    int seconds = 1;

    // Set connection type
    if(protocol == "tcp")
    {
        type = SOCK_STREAM;
        pro = 0;
    }
    else if(protocol == "udp")
    {
        type = SOCK_DGRAM;
        pro = IPPROTO_UDP;
    }
    else
    {
        cout << "unknown protocol: " << protocol << endl;
        exit(1);
    }

    sockfd = socket(AF_INET, type, pro);
    fcntl(sockfd, F_SETFL, O_NONBLOCK);

    if(sockfd < 0)
    {
        perror("error creating socket");
        exit(1);
    }

    server = gethostbyname(ip.c_str());

    if(server == NULL)
    {
        perror("error setting host");
        exit(1);
    }

    // Zero out the server struct
    bzero((char*) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    memcpy(&serv_addr.sin_addr, server->h_addr, server->h_length);
    
    if(protocol == "TCP")
    {
        // Send a TCP RST when using close() instead of waiting
        linger lin;
        lin.l_onoff = 1;
        lin.l_linger = 0;

        setsockopt(sockfd, SOL_SOCKET, SO_LINGER, (void*) &lin, sizeof(lin));

        // Connect
        rc = connect(fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
        if(rc == -1 && errno != EINPROGRESS)
        {
            // Failed to connect
            //cout << "TCP connection failed" << endl;
            close(sockfd);
            return false;
        }
        if(rc == 0)
        {
            //cout << "TCP connection succeeded" << endl;
            close(sockfd);
            return true;
        }       

        FD_ZERO(&fdset);
        FD_SET(sockfd, &fdset);
        tv.tv_sec = seconds;
        tv.tv_usec = 0;

        rc = select(fd + 1, NULL, &fdset, NULL, &tv);
        switch(rc)
        {
            case 1:
                len = sizeof(so_error);

                getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &so_error, &len);

                if(so_error == 0)
                {
                    //cout << "socket connected." << endl;
                    close(sockfd);
                    return true;
                }
                else
                {
                    // error?
                }
                break;
            case 0: //timeout
                break;
        }

        close(sockfd);

        return false;
    }
    else if(protocol == "UDP")
    {
        if(sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
        {
            perror("could not send udp message");
            exit(1);
        }
        
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &serv_addr, (socklen_t*) sizeof(serv_addr));
        //printf("Received packet from %s:%d\nData: %s\n\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port), buffer);

        //if(buffer == "ICMP whatever") do stuff
        close(sockfd);
    }

    return true;
}

bool scanPorts(vector<int> ports, vector<string> ips, vector<string> protocols)
{
    cout << "portscan of ports: " << ports[0] << "-" << ports[ports.size()-1] << endl;
    cout << "---------------------------------------------------" << endl;
    for(int i = 0; i < ips.size(); i++)
    {
        cout << ips[i] << endl << endl;

        for(int j = 0; j < protocols.size(); j++)
        {
            cout << "   " << protocols[j] << " port\tstate\tservice" << endl;

            for(int k = 0; k < ports.size(); k++)
            {
                cout << "\t" << ports[k];
                // Scan the port and return the state
                if(scan(ports[k], ips[i], protocols[j]))
                {
                    // print open
                    cout << "\topen";
                }
                else
                {
                    // print closed
                    cout << "\tclosed";
                }

                servent* service = getservbyport(htons(ports[k]), protocols[j].c_str());

                if(service != NULL)
                {
                    cout << "\t" << service->s_name << endl;
                }
                else
                {
                    cout << endl;
                }
            }
            cout << endl;
        }
        cout << "---------------------------------------------------" << endl;
    }

    return true;
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printUsage();
        return 1;
    }

    vector<int> ports;
    vector<string> ips;
    vector<string> protocols;

    if(!parseOptions(argc, argv, ports, ips, protocols))
    {
        return 1;
    }
    
    if(!scanPorts(ports, ips, protocols))
    {
        return 1;
    }
    
    return 0;
}