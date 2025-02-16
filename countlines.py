#!/usr/bin/python3
##
## yhOS
## Copyright (c) aceinet
## License: GPL-2.0
##
import os
import argparse

def should_count(filepath):
    """Determine if a file should be counted based on extension or shebang."""
    ext = os.path.splitext(filepath)[1].lower()
    name = os.path.basename(filepath)
    allowed_exts = {'.asm', '.s', '.c', '.h', '.cpp', '.sh', '.ld', '.py', '.dc'}
    unallowed_exts = {'.sample'}
    
    if ext in unallowed_exts:
        return False
    if ext in allowed_exts or name == '.Makefile':
        return True
    
    # Check for shebang
    try:
        with open(filepath, 'rb') as f:
            return f.read(2) == b'#!'
    except:
        return False

def count_lines(filepath):
    """Count lines in a file using binary mode for reliability."""
    with open(filepath, 'rb') as f:
        return sum(1 for _ in f)

def main():
    parser = argparse.ArgumentParser(description='Count code lines recursively for specific file types.')
    parser.add_argument('directory', nargs='?', default='.', 
                       help='Starting directory (default: current directory)')
    args = parser.parse_args()

    start_dir = os.path.abspath(args.directory)
    total_lines = 0

    for root, dirs, files in os.walk(start_dir):
        for filename in files:
            filepath = os.path.join(root, filename)
            if should_count(filepath):
                try:
                    lines = count_lines(filepath)
                    total_lines += lines
                    #print(f"{filepath}: {lines}")
                except Exception as e:
                    #print(f"Error reading {filepath}: {str(e)}")
                    pass

    print(f"Total lines counted: {total_lines}")

if __name__ == "__main__":
    main()
