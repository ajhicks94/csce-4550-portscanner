#!/usr/bin/python3.6

import argparse
import ipaddress
import re

PORT_MIN = 1
PORT_MAX = 65535


class PortParser(argparse.Action):
    def __call__(self, parser, namespace, values, option_string=None):
        lst = []
        for string in values:
            if '-' in string:
                groups = string.split('-')
                if groups[0] == '' or groups[-1] == '' or len(groups) != 2:
                    raise argparse.ArgumentTypeError("'" + string + "' is not a valid range.")

                begin = int(groups[0])
                end = int(groups[1]) or begin

                if end < begin:
                    raise ValueError("invalid port range: {}-{}".format(begin, end))

                if begin < PORT_MIN or end > PORT_MAX:
                    raise ValueError("range out of bounds\naccepted range: 1-65535")

                lst.extend(list(range(begin, end+1)))
            else:
                try:
                    value = int(string)
                except ValueError:
                    raise ValueError("invalid port: " + string)

                if value < PORT_MIN or value > PORT_MAX:
                    raise ValueError("port out of bounds\naccepted range: 1-65535")

                lst.append(value)
            setattr(namespace, self.dest, lst)


def main():
    parser = argparse.ArgumentParser()

    # range 1-65535
    # accepted formats:
    #     single port:   --port 23
    #     range      :   --port 2000-3000
    #     list       :   --port 21 80 443
    parser.add_argument(
        '--port',
        dest="port",
        action=PortParser,
        nargs='*',
        default=list(range(1, 1024+1))
    )

    # parser.add_argument(
    #     '--ip',
    #     action='store',
    #     dest='addresses',
    #     type=lambda: x re.match()
    # )

    args = parser.parse_args()
    print(args)


if __name__ == "__main__":
    main()
