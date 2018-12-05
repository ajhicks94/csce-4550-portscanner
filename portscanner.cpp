#include <iostream>

using std::cout;
using std::endl;

void printUsage(char* name)
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

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printUsage(argv[0]);
        return 1;
    }
}