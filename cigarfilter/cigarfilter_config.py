#!/usr/bin/env python
import os
import sys
import shutil
import subprocess
import traceback
from subprocess import Popen, PIPE, STDOUT

from cigarfilter import _CIGARFILTER_RESOURCES_DIR

def list_filters():
    plugin_dir = os.path.join(_CIGARFILTER_RESOURCES_DIR,"plugins")
    available = [x for x in os.listdir(plugin_dir)]
    for x in sorted(available):
        complete_path = os.path.join(plugin_dir, x)
        if x != "__EMPTY" and x != "." and os.path.isfile(
                complete_path):  # __EMPTY Was put there by me only so that the directory can exist by default
            if "." in x:
                x = ".".join(x.split('.')[0:-1])
            if x.startswith("lib"):  # This is a prefix added by convention in many UNIX-like OS
                x = "".join(x[3:])
            if x.strip():
                print(x)

def remove(plugin_name):
    # The problem here will be to handle the file extension in a +/- cross-platform manner

    matches_found = []
    plugin_dir = os.path.join(_CIGARFILTER_RESOURCES_DIR,"plugins")

    print('Seeking for filter {}...'.format(plugin_name))

    for elt in os.listdir(plugin_dir):
        fullname = elt
        if "." in elt:
            stripped_name = ".".join(fullname.split(".")[:-1]) # Without extension
        else:
            stripped_name = elt
        if stripped_name == plugin_name:
            matches_found.append({"Full name": fullname, "Alias": stripped_name})

    if len(matches_found) > 1:
        print("Error. Several potential matches were found: \n", matches_found)

    elif len(matches_found) == 0:
        print("Error. No match found for {}".format(plugin_name))
        print("Available filters :  \n".format())
        list_filters()
    else :
        print("Removing filter {}...".format(matches_found))
        filter_path = os.path.join(plugin_dir,matches_found[0]["Full name"])
        os.remove(filter_path)

def clean_tmp():
    print("Cleaning tmp files...")
    plugin_dir = os.path.join(_CIGARFILTER_RESOURCES_DIR,"plugins")
    for file in os.listdir(plugin_dir):
        path = os.path.join(plugin_dir,file)
        if os.path.isdir(file):
            if "tmp_buildplugin" in file:
                print("Removing " + path)
                shutil.rmtree(path)

# Quick and dirty scripting. This can (should ?) be replaced by real argparse subcommands
def main():    

    plugin_dir = os.path.join(_CIGARFILTER_RESOURCES_DIR,"plugins")

    try:
        assert len(sys.argv) > 1
    except Exception as e:
        print("Bad invocation.\nExpected usage : cigarfilter_config [pluginpath, help, list, add, remove, clean, purge]")
        return

    first_arg = sys.argv[1].lower().replace('-','')
    if (first_arg == "help") or (first_arg == "h") or (first_arg == "?"):
        print("  * help :\n\t Displays this help\n")
        print("  * pluginpath :\n\t Displays the path were filters are stored. Manually modify them only at your own risks.\n")
        print("  * list :\n\t Lists all available filters.\n")
        print("  * add :\n\t Adds a filter from a .cpp file (see README online at https://github.com/GDelevoye/cigarfilter).\n")
        print("  * remove :\n\t Removes a filter. Example : \"cigarfilter_config remove default\".\n")
        print("  * clean :\n\t Cleans all logs and temporary files.\n")
        print("  * purge :\n\t Cleans all logs, temporary files and all filters except the default one.\n")
        
    elif sys.argv[1] == "pluginpath":
        print(plugin_dir.strip(),end='')
        
    elif sys.argv[1] == "list":
        list_filters()
                
    elif sys.argv[1] == "add":
        path_to_cpp_file = os.path.realpath(sys.argv[2])
        basename_cpp_file = os.path.basename(path_to_cpp_file)
        plugin_name = sys.argv[3]

        src_dir = os.path.join(_CIGARFILTER_RESOURCES_DIR,"plugin_template")
        dest_dir = os.path.join(_CIGARFILTER_RESOURCES_DIR,"tmp_buildplugin_"+plugin_name)

        # Copy the template directory for a new project
        shutil.copytree(src_dir,dest_dir)
        shutil.copy(path_to_cpp_file,os.path.join(dest_dir,"src",basename_cpp_file))

        HERE = os.getcwd()
        # Build it in the build subdir :
        os.chdir(os.path.join(dest_dir,"build"))

        # CMake...
        callProcess = subprocess.Popen(["cmake",".."],stdin=PIPE, stdout=PIPE, stderr=STDOUT, close_fds=True)
        stdout, stderr = callProcess.communicate()
        if stdout:
            print(stdout.decode())
        if stderr:
            print(stderr.decode())

        # Make...
        callProcess = subprocess.Popen(["make"],stdin=PIPE, stdout=PIPE, stderr=STDOUT, close_fds=True)
        stdout, stderr = callProcess.communicate()
        if stdout:
            print(stdout.decode())
        if stderr:
            print(stderr.decode())

        # Now we want to move the plugin to its destination [APP]/resources/plugins/[plugin_name].[extension]

        # Many platforms can have many extensions... We need to know which it is
        for extension in [".dll",".so",".a",".dylib",""]:
            candidate_file = os.path.join(os.getcwd(), "lib", "plugin"+extension)
            if os.path.isfile(candidate_file):
                outputlib = candidate_file
                break

        # Rename the compiled library (while keeping its extension/decoration)
        renamed_file = os.path.basename(candidate_file).replace("plugin",plugin_name)
        # Copy it in the plugin_dir
        shutil.copy(candidate_file,os.path.join(_CIGARFILTER_RESOURCES_DIR,"plugins",renamed_file))

        # Remove the temporary filles
        shutil.rmtree(os.path.join(_CIGARFILTER_RESOURCES_DIR,"tmp_buildplugin_"+plugin_name))

        os.chdir(HERE)

    elif sys.argv[1] == "remove":
        remove(sys.argv[2])
        
    elif sys.argv[1] == "clean":
        clean_tmp()
        
    elif sys.argv[1] == "purge":
        print('Purging the tmp files and the plugins...')
        clean_tmp()
        plugin_dir = os.path.join(_CIGARFILTER_RESOURCES_DIR, "plugins")

        for elt in os.listdir(plugin_dir):
            full_path = os.path.join(plugin_dir,elt)

            if "." in elt:
                stripped_name = ".".join(elt.split(".")[:-1])  # Without extension
            else:
                stripped_name = elt

            if stripped_name == "default":
                print("Ignoring default : {}".format(full_path))
                pass
            else:
                print("Removing {}, full path = {}".format(stripped_name, full_path))
                os.remove(full_path)


    else:
       print("Bad invocation.\nExpected usage : cigarfilter_config [help, list, add, remove, clean, purge]")
       return 


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        # printing stack trace
        traceback.print_exc()
        print(e)

    print("Cleaning tmp...")
    clean_tmp()
