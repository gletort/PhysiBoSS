Files: MBSS_PrepareProjectFile.sh, MBSS_PrepareProjectFile.pm, MBSS_prepareProjectFileTrajectoryFig.py, MBSS_PrepareProjectFilePieChart.py

Create a project folder with the name of .pmbss file, with all data/inputs and a shell script with the name of the .pmbss file. Launching the shell script runs (multiple) MaBoSS and creates the tables (if specified).

MBSS_PrepareProjectFile.sh File.pmbss

The file File.pmbss contain the following fields:

    MABOSS = Executable_name;
    BND= bnd_file.bnd;
    CFG= cfg_file.cfg;

The file File.pmbss can contain the optional fields:

    INIT_COND=[Previous_model.bnd,Previous_model_probtraj.csv,#line_number]
    MUT= First_node Second_Node ...;
    COMB_MUT=#number_of_possible_combined_mutation;
    VAR_SENS=[$External_variable1,Suffix_to_add] [$External_variable2,Suffix_to_add]  ...;
    COMB_VAR_SENS=#number_of_possible_combined_variable_sensitivity;
    TRAJ_TABLE=yes;
    STAT_TABLE=[yes,#probability_threshold];

Default number of possible combined mutation/variable sensitivity is 1 (if COMB_MUT or COMB_VAR_SENS are not specified).

Avoid space in file names.

MBSS_PrepareProjectFile.pm, MBSS_PreapreProjectFilePieChart.py, MBSS_PrepareProjectFileTrajectoryFig.py should be accessible (within an environment variable). If it is in the working folder, it is copied in the project folder.
The file File.pmbss can be in another location (eg /dir1/dir2/File.pmbss or ../../dir/File.pmbss).
The inputs files (.bnd, .cfg, Previous_model_probtraj) can be other locations.
MaBoSS executable name should be an executable file or accessible within PATH variable. If MaBoSS executable name is a readable file, it is copied in the project folder. 