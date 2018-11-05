#!/usr/bin/env python3

# File: sengfmt.py
# Student Name  : Jayden Chan
# Student Number: V00898517
# SENG 265 - Assignment 2

import sys
import queue

def main():
    file = None
    fmt = False
    cap = False
    q = queue.Queue()
    qsize = 0
    mrgn = [0]
    width = -1

    if len(sys.argv) > 1:
        file = open(sys.argv[1], "r")
    else:
        file = sys.stdin

    for line in file:
        if line.startswith("?"):
            line = line.strip()
            tokens = line.split(" ")

            if tokens[0] == "?mrgn":
                if tokens[1].startswith("+"):
                    if q.qsize() > 0:
                        mrgn.append(mrgn[-1] + int(tokens[1][1:]))
                    else:
                        mrgn[0] += int(tokens[1][1:])
                elif tokens[1].startswith("-"):
                    if q.qsize() > 0:
                        mrgn.append(mrgn[-1] - int(tokens[1][1:]))
                    else:
                        mrgn[0] -= int(tokens[1][1:])
                else:
                    mrgn[0] = int(tokens[1])

                del tokens[:1]

                if width != -1 and mrgn[-1] > width - 20:
                    mrgn[-1] = width - 20
                if mrgn[-1] < 0:
                    mrgn[-1] = 0
                fmt = True
                continue
            elif tokens[0] == "?maxwidth":
                width = int(tokens[1])
                fmt = True
                del tokens[:1]
                continue
            elif tokens[0] == "?fmt":
                fmt = tokens[1] == "on"
                del tokens[:1]
                continue
            elif tokens[0] == "?cap":
                cap = tokens[1] == "on"
                del tokens[:1]
                continue

        if not fmt and line != "\n":
            print(line, end='')
            continue

        if width == -1:
            if fmt and line != "\n":
                print(mrgn[0] * ' ', end='')

            print(line, end='')
            continue

        if line == "\n":
            unload(q, qsize, width, cap, mrgn)
            qsize = 0
            print()
            continue

        line = line.strip()
        tokens = line.split(" ")

        for token in tokens:
            if token == "":
                continue

            if width == -1 or len(token) + qsize + q.qsize() + mrgn[0] <= width:
                q.put(token)
                qsize += len(token)
            else:
                unload(q, qsize, width, cap, mrgn)
                qsize = 0
                q.put(token)
                qsize += len(token)

    if width != -1:
        unload(q, qsize, width, cap, mrgn)


def unload(q, qsize, width, cap, mrgn):
    if qsize == 0:
        return
    space_array = [1] * (q.qsize() - 1)
    num_spaces = width - mrgn[0] - qsize - q.qsize() + 1
    ctr = 0

    while q.qsize() > 1 and num_spaces > 0:
        space_array[ctr % (q.qsize()-1)] += 1
        ctr += 1
        num_spaces -= 1

    ctr = 0
    print(mrgn[0] * ' ', end='')
    while not q.empty():
        if cap:
            print(q.get().upper(), end = '')
        else:
            print(q.get(), end = '')

        if ctr < len(space_array):
            print(space_array[ctr] * ' ', end = '')
            ctr += 1

    print()
    if len(mrgn) > 1:
        del mrgn[0]

if __name__ == "__main__":
    main()
