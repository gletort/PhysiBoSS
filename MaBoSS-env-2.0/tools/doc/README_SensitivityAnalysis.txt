Creat a new folder, copy the bnd file in it, generate .cfg files and a shell script for multiple MaBoSS runs (within MBSS_FormatTable.pl). A string is be added to every external variable. MBSS_FormatTable.pl must be accessible by command line.
MBSS_SensitivityAnalysis.pl  <file.bnd> <file.cfg> "<add_string>"

The name of the new folder is the name of the .cfg file with "Sensitivity_" as prefix
The name of the generated shell script is the name of the .cfg file, with "Sensitivity_" as prefix.
The names of the new .cfg files have the name of initial cfg file, with the modified external variable as suffix.