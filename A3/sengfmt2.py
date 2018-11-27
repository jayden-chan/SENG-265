#!/opt/local/bin/python

import sys
import fileinput
from formatter import Formatter

def main():
    """
    Creates an instance of the formatter and process the lines
    """
    f = None
    if len(sys.argv) > 1:
        f = Formatter(filename = sys.argv[1])
    else:
        f = Formatter(inputlines = sys.stdin)

    lines = f.get_lines()

    for line in lines:
        print(line, end='')

if __name__ == "__main__":
    main()
