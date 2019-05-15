Generate .bnd file with external variables that control mutations (file_mut.bnd from file.bnd).

MBSS_MutBnd.pl  <file.bnd> "<node_list>"

Generate .bnd file and .cfg file with external variables that control mutations.

MBSS_MutBndCfg.pl  <file.bnd> <file.cfg> "<node_list>" (file_mut.bnd and file_mut.cfg from file.bnd and file.cfg).

External variable that controls overexpression of node has the prefix ``$High_'' (eg node ``N'' has ``$High_N''). For underexpression, the prefix is ``$Low_N''. The standard output provides the nodes that has been found in the .bnd file. If these variables are set to 1, the state of the node is forced, no matter the initial condition. This is realized by an almost instantaneous transition, with a maximum rate given by the ``max_rate'' node variable in the new .bnd file. The underexpression variable is prioritized against the overexpression variable.

Careful: the ``max_rate'' is the maximum rate possible in c++, divided by the number of nodes. If the user is using such a large rate in another place, MaBoSS run could produce wrong results because addition of large rate may overflow c++ maximum number.