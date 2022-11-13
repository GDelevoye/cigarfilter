This directory is included in the final app during the pip installation (see "conda-recipe/meta-yaml").

Normally, it should be installed in a location where the user have the right to write, which is why I use it as an "app directory".

The final user is not supposed to touch manually this directory. It contains the files required for the whole project/App to run correctly :

* "plugin" is empty right now, but after the compilation of cigarfilter, the examples and default plugins will be placed there by CMake install. New plugins added with cigarfilter_config also end up here. 
* "plugin_template" as its name clearly indicates, contains a ready-to-use template that will be copy-pasted by cigarfilter_config in a fresh tmp director when adding a new filter. 
* "test_data" contains the files required for "cigarfilter_test"

During the compilation of a new filter with "cigarfilter_config add", this directory will temporarily contain files or directories that start with the prefix "tmp_", and that are cleaned at each invocation of "cigarfilter_config clean".

The same may happen during a "cigarfilter_test" invocation; the standard output of several tests will be redirected here temporarily.
