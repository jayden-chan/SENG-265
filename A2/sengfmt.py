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
    mrgn = 0
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
                del tokens[0]
                if tokens[0].startswith("+"):
                    mrgn += int(tokens[0][1:])
                elif tokens[0].startswith("-"):
                    mrgn -= int(tokens[0][1:])
                else:
                    mrgn = int(tokens[0])
                del tokens[0]
                if width != -1 and mrgn > width - 20:
                    mrgn = width - 20
                if mrgn < 0:
                    mrgn = 0
                fmt = True
                continue
            elif tokens[0] == "?maxwidth":
                del tokens[0]
                width = int(tokens[0])
                fmt = True
                del tokens[0]
                continue
            elif tokens[0] == "?fmt":
                del tokens[0]
                if tokens[0] == "on":
                    fmt = True
                else:
                    fmt = False
                del tokens[0]
                continue
            elif tokens[0] == "?cap":
                del tokens[0]
                if tokens[0] == "on":
                    cap = True
                else:
                    cap = False
                del tokens[0]
                continue

        if width == -1:
            if fmt and line != "\n":
                print(mrgn * ' ', end='')

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

            if width == -1 or len(token) + qsize + q.qsize() + mrgn <= width:
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
    num_spaces = width - mrgn - qsize - q.qsize() + 1
    ctr = 0

    # print("num spaces: ", num_spaces)
    while q.qsize() > 1 and num_spaces > 0:
        space_array[ctr % (q.qsize()-1)] += 1
        ctr += 1
        num_spaces -= 1

    ctr = 0
    print(mrgn * ' ', end='')
    while not q.empty():
        print(q.get(), end = '')
        # print("ctr ", ctr)
        if ctr < len(space_array):
            print(space_array[ctr] * ' ', end = '')
            ctr += 1

    print()

if __name__ == "__main__":
    main()
