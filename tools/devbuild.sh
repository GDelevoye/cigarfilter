cd /home/delevoye/miniconda3/envs/
rm -rvf * 
cd ..
cd conda-bld
rm -rvf *
conda build purge all
conda clean -a -y
pip uninstall -y cigarfilter
conda uninstall -y cigarfilter
cd //home/delevoye/GitHub/cigarfilter
conda build -c conda-forge conda-recipe
conda env remove -n cig -y
conda create -n cig -y
conda install -c conda-forge -c local -n cig cigarfilter -y
