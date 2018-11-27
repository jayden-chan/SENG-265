#!/opt/local/bin/python

import sys
import queue
import re
import calendar

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
        mon = False
        q = queue.Queue()
        qsize = 0
        mrgn = 0
        width = -1
        rep = None

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
                elif tokens[0] == "?monthabbr":
                    mon = tokens[1] == "on"
                    continue
                elif tokens[0] == "?replace":
                    rep = []
                    rep.append(tokens[1])
                    rep.append(tokens[2])
                    continue


            if not fmt and line != "\n":
                self.lines.append(line)
                continue

            if width == -1:
                tmp = ""
                if fmt and line != "\n":
                    tmp += (mrgn * ' ')

                tmp += (line)
                self.lines.append(tmp)
                continue

            if line == "\n":
                self.unload(q, qsize, width, cap, mrgn)
                qsize = 0
                self.lines.append("\n")
                continue

            line = line.strip()
            if rep is not None:
                line = re.sub(rep[0], rep[1], line)

            if mon:
                line = self.replace_date(line)

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

        return self.lines


    def unload(self, q, qsize, width, cap, mrgn):
        line = ""
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
        line += (mrgn * ' ')
        while not q.empty():
            if cap:
                line += (q.get().upper())
            else:
                line += (q.get())

            if ctr < len(space_array):
                line += (space_array[ctr] * ' ')
                ctr += 1

        line += "\n"
        self.lines.append(line)

    def replace_date(self, string):
        pattern = r"(\d\d)[\/\-\.](\d\d)[\/\-\.](\d\d\d\d)"
        matches = re.search(pattern, string)
        if matches is None:
            return string
        return re.sub(pattern, "{0}. {1}, {2}".format(calendar.month_abbr[int(matches.group(1))], matches.group(2), matches.group(3)), string)
