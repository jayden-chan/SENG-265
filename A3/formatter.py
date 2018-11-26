#!/opt/local/bin/python

import sys
import queue

class Formatter:
    """This is the definition for the class"""

    def __init__(self, filename=None, inputlines=None):
        self.filename = filename
        self.inputlines = inputlines
        self.lines = []

    def get_lines(self):
        file = None
        if self.filename is None:
            file = self.inputlines
        else:
            file = open(self.filename)

        fmt = False
        cap = False
        q = queue.Queue()
        qsize = 0
        mrgn = 0
        width = -1

        for line in file:
            if line.startswith("?"):
                line = line.strip()
                tokens = line.split(" ")

                if tokens[0] == "?mrgn":
                    if tokens[1].startswith("+"):
                        mrgn += int(tokens[1][1:])
                    elif tokens[1].startswith("-"):
                        mrgn -= int(tokens[1][1:])
                    else:
                        mrgn = int(tokens[1])

                    if width != -1 and mrgn > width - 20:
                        mrgn = width - 20
                    if mrgn < 0:
                        mrgn = 0
                    fmt = True
                    continue
                elif tokens[0] == "?maxwidth":
                    width = int(tokens[1])
                    fmt = True
                    continue
                elif tokens[0] == "?fmt":
                    fmt = tokens[1] == "on"
                    continue
                elif tokens[0] == "?cap":
                    cap = tokens[1] == "on"
                    continue

            if not fmt and line != "\n":
                print(line, end='')
                continue

            if width == -1:
                if fmt and line != "\n":
                    print(mrgn * ' ', end='')

                print(line, end='')
                continue

            if line == "\n":
                self.unload(q, qsize, width, cap, mrgn)
                qsize = 0
                print()
                continue

            line = line.strip()
            tokens = line.split(" ")

            for token in tokens:
                if token == "":
                    continue

                if width == -1 or len(token) + qsize + q.qsize() + mrgn <= width:
                    q.put(token)
                    qsize += len(token)
                else:
                    self.unload(q, qsize, width, cap, mrgn)
                    qsize = 0
                    q.put(token)
                    qsize += len(token)

        if width != -1:
            self.unload(q, qsize, width, cap, mrgn)

        return "this is formatted"


    def unload(self, q, qsize, width, cap, mrgn):
        if qsize == 0:
            return
        space_array = [1] * (q.qsize() - 1)
        num_spaces = width - mrgn - qsize - q.qsize() + 1
        ctr = 0

        while q.qsize() > 1 and num_spaces > 0:
            space_array[ctr % (q.qsize()-1)] += 1
            ctr += 1
            num_spaces -= 1

        ctr = 0
        print(mrgn * ' ', end='')
        while not q.empty():
            if cap:
                print(q.get().upper(), end = '')
            else:
                print(q.get(), end = '')

            if ctr < len(space_array):
                print(space_array[ctr] * ' ', end = '')
                ctr += 1

        print()
