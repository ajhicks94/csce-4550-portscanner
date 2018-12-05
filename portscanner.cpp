#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::regex;
using std::regex_match;
using std::cmatch;
using std::ifstream;

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
    cout << "                          list of ports:  --port 21, 80, 443" << endl;
    cout << "          --ip" << endl;
    cout << "                      scan designated ip address" << endl;
    cout << "                      accepted formats:" << endl;
    cout << "                          single ip:      --ip 129.120.93.94" << endl;
    cout << "                          ip range:       --ip 129.120.93.87-200" << endl;
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

    regex re_ip ("\\b(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\."
  "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\."
  "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\."
  "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\b");

    regex re_ip_range ("\\b(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\."
  "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\."
  "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\."
  "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])-(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\b");

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
            // or add port range to port list (decode range)
        }
        else if(strcmp(argv[i], "--ip") == 0)
        {
            if(i + 1 != argc)
            {
               if(regex_match(argv[i+1], re_ip))
                {
                    ips.push_back(argv[i+1]);
                }
                else if(regex_match(argv[i+1], re_ip_range))
                {
                    
                    //ips.push_back(argv[i+1]);
                }
                else
                {
                    cout << "unrecognized ip: " << argv[i+1] << endl;
                    exit(1);
                }

                i++;
            }
            else
            {
                cout << "missing arg: " << argv[i] << endl;
                exit(1);
            }
            
            // add ip to ip list
            // or add ip range to ip list (decode range first)
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