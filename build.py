from pathlib import Path
import subprocess


root = Path('src')

files = [str(file) for file in root.rglob('*.cpp')]

try:
    print("Starting build...")
    subprocess.run([f'g++ {' '.join(files)} -o Vindicta $(sdl2-config --cflags --libs) -Iinclude -Iexternal'], check=True, shell=True)
    print("Build successful!")
except subprocess.CalledProcessError:
    print("Build terminated")
