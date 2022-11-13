#!/usr/bin/env python

from setuptools import setup, Extension, find_packages
import os
import sys

def package_files(directory):
    paths = []
    for (path, directories, filenames) in os.walk(directory):
        for filename in filenames:
            paths.append(os.path.join('..', path, filename))
    return paths

extra_files = package_files('cigarfilter/resources')

setup(
    name='cigarfilter',
    version='1.0',
    description="",
    long_description="A fast and hackable tool to filter .bam files by CIGAR string",
    author='DELEVOYE Guillaume',
    author_email="delevoye.guillaume@gmail.com",
    packages=find_packages(),
    url="https://github.com/GDelevoye/cigarfilter",
    package_data={'cigarfilter': extra_files},
    install_requires=["setuptools"],
    entry_points={'console_scripts': [
        "cigarfilter_config=cigarfilter.cigarfilter_config:main"
    ]}
)
