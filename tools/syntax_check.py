# Script to be be ran during the build that runs a compile command 
# with IDO followed by a syntax check command with GCC.

import subprocess
import sys

def main():
    # Get full compile command
    dash_split = sys.argv.index("--")
    check_cmd = sys.argv[1:dash_split]
    compile_cmd = sys.argv[(dash_split + 1):]

    # Run compile command
    retcode = subprocess.call(compile_cmd)

    # Don't run check if the compile failed
    if retcode != 0:
        sys.exit(retcode)

    # Run check command
    in_file_path = sys.argv[-1]
    retcode = subprocess.call(check_cmd + [in_file_path])

    # Don't run check if the compile failed
    if retcode != 0:
        sys.exit(retcode)

if __name__ == "__main__":
    main()
