#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>
#include <algorithm>

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

int parseOptions(int argc, char* argv[])
{
    bool single_ip = false;

    string filename = "";

    vector<int> ports;
    vector<string> ips;
    vector<string> protocols;

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
            exit(1);
        }
        else if(strcmp(argv[i], "--port") == 0)
        {
            // add ports to port list
            if(i + 1 != argc)
            {
                // add list of ports
                if()
                {

                }
                // add range of ports
                else if()
                {

                }
                // add single port
                else if()
                {

                }
                else
                {
                    cout << "invalid port: " << argv[i+1] << endl;
                    exit(1);
                }

            }
            else
            {
                cout << "missing arg: " << argv[i] << endl;
                exit(1);
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
                                exit(1);
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
                            exit(1);
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
                            exit(1);
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
                        exit(1);
                    }

                    i++;
                }
                else
                {
                    cout << "missing arg: " << argv[i] << endl;
                    exit(1);
                }
            }
            else
            {
                cout << argv[i] << ": individual address has already been supplied" << endl;
                exit(1);
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
                exit(1);
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
                exit(1);
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
                exit(1);
            }
        }
        else
        {
            cout << "Unknown argument: " << argv[i] << endl;
            exit(1);
        }
    }

    // Check for any duplicate ips in ip list and remove them
    sort(ips.begin(), ips.end());
    ips.erase(unique(ips.begin(), ips.end()), ips.end());

    // Check to see if anything integral wasn't assigned
    // If so, assign defaults here and return object? json?
    
    cout << endl;
    for(int i = 0; i < ips.size(); i++)
    {
        cout << "ips[" << i << "] = " << ips[i] << endl;
    }
    cout << endl;

    for(int i = 0; i < protocols.size(); i++)
    {
        cout << "protocols[" << i << "] = " << protocols[i];
    }

    cout << endl;
    return 0;
}

int main(int argc, char* argv[])
{
    // ./portScanner --port 
    // Not sure if we can specify a max argc since the individual
    // ports could be any amount?
    if(argc < 2)
    {
        printUsage();
        return 1;
    }
    if(parseOptions(argc, argv) == -1)
    {
        //return -1;
    }
    //if(parseOptions(argc, argv))
    {

    }

    return 0;
}