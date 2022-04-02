from shutil import copyfile
from subprocess import check_output, CalledProcessError
import sys
import os
import platform
import subprocess

Import("env")

def build_web():

    os.chdir("www")
    print("Attempting to build webpage...")
    try:
        print(check_output(["npm", "install"]))
        print(check_output(["npm", "run", "minify"]))

        # Load the resulting minified site and write as a header file
        with open("dist/index.html", "r") as src_file:
            src = src_file.read().replace('"', '\\"')

            dest_filename = "../lib/PageSource/PageSource.h"
            os.makedirs(os.path.dirname(dest_filename), exist_ok=True)
            with open(dest_filename, "w") as dest_file:
                dest_file.write("const char pageSource[] PROGMEM = \"%s\";" % src)

    except Exception as e:
            print("Encountered error", type(e).__name__, "building webpage:", e)
            print("WARNING: Failed to build web package. Using pre-built page.")
    finally:
        print("Webpage build successfully!")
        os.chdir("..");

build_web()
