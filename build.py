from pathlib import Path
import subprocess
import sys


root = Path('src')

files = [str(file) for file in root.rglob('*.cpp')]
arguments = [arg for arg in sys.argv[1:]]

try:
    print("Starting build...")
    subprocess.run([f'g++ {' '.join(arguments)} {' '.join(files)} -o Vindicta $(sdl2-config --cflags --libs) -lSDL2_image -Iinclude -Iexternal'], check=True, shell=True)
    print("Build successful!")
except subprocess.CalledProcessError:
    print("Build terminated")
