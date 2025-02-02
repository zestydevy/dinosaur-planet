#!/usr/bin/env python3

import argparse
from io import BufferedReader

def main():
    parser = argparse.ArgumentParser(description="Find differences between two binary files.")
    parser.add_argument("a", type=argparse.FileType("rb"), help="The first file (used as base).")
    parser.add_argument("b", type=argparse.FileType("rb"), help="The second file (compared to a).")
    parser.add_argument("-c", "--count", type=int, help="Maximum number of differences to find (default=1, specify 0 for infinite).", default=1)
    parser.add_argument("-s", "--step", type=int, help="How many bytes to compare at a time per step (default=4).", default=4)

    args = parser.parse_args()

    max_diffs: int = args.count
    step: int = args.step
    diffs = 0

    with args.a as a_file, args.b as b_file:
        a_file: BufferedReader
        b_file: BufferedReader
        a = bytearray(a_file.read())
        b = bytearray(b_file.read())
        a_len = len(a)
        b_len = len(b)
        min_len = min(a_len, b_len)

        if a_len != b_len:
            print("Difference in length. Expected 0x{:X}, found 0x{:X}."
                .format(a_len, b_len))

        i = 0
        while i < min_len:
            k = i
            diff = False
            while (k - i) < step and k < min_len:
                if a[k] != b[k]:
                    diff = True
                    break
                k += 1
                
            if diff:
                print("Difference at: 0x{:X}. Expected 0x{:X}, found 0x{:X}."
                    .format(i, a[k], b[k]))
                diffs += 1
                if max_diffs > 0 and diffs >= max_diffs:
                    break

            i += step
    
    if diffs == 0:
        print("No differences")

if __name__ == "__main__":
    main()
