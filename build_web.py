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
        print(check_output(["node_modules/.bin/gulp"]))
        copyfile("build/index.html.gz.h", "../dist/index.html.gz.h")
    except OSError as e:
        print("Encountered error OSError building webpage:", e)
        if e.filename:
            print("Filename is", e.filename)
        print("WARNING: Failed to build web package. Using pre-built page.")
    except CalledProcessError as e:
        print(e.output)
        print("Encountered error CalledProcessError building webpage:", e)
        print("WARNING: Failed to build web package. Using pre-built page.")
    except Exception as e:
        print("Encountered error", type(e).__name__, "building webpage:", e)
        print("WARNING: Failed to build web package. Using pre-built page.")
    finally:
        os.chdir("..");

build_web()
