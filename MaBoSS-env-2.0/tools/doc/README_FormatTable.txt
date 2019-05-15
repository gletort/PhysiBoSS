1) Create a directory, with the same name as the cfg file (adding a number if directory already exists). Copy MaBoSS, .bnd and .cfg files in the directory.
2) Run MaBoSS in the directory
3) Create tables from output files in the directory, that have a column for each network state (file_cfg_probtraj_table.csv and file_cfg_statdist_table.cfg (if the threshold is given)). The threshold is used in the file "file_cfg_statdist_table.cfg", in order to suppress states whose probability is below the threshold.

MBSS_FormatTable.pl file.bnd file.cfg (optional)threshold (optional)-mb MaBoSS_executable_name

The input files (.bnd and .cfg) can be located in other directories (given by absolute path), they just need to be readable.

By default, MaBoSS executable name is "MaBoSS". It should be in the working directory or accessible within PATH variable. Executable name can be specified by the -mb option; any name/location can be provided. 

If MaBoSS executable name is a readable file, it is copied in the directory.


