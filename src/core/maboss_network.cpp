#include "maboss_network.h"
#include "../base/utilities.h"

MaBossNetwork::MaBossNetwork()
{
	// This is needed for the external functions
	MaBEstEngine::init();
	network = new Network();
	runConfig = RunConfig::getInstance();
	runConfig->setSeedPseudoRandom( UniformInt() );
	symbTable = SymbolTable::getInstance();
	update_time = 10;
	def_nodes.clear();
	conffile = "";
}

MaBossNetwork::~MaBossNetwork()
{
	delete network;
}

/* Read properties from XML parameter file for network */
void MaBossNetwork::read_properties( ReadXML* reader, std::string what )
{
	reader->getDoubleValue( what, "network_update_step", &update_time );
	
	// Read mutations
	int left = 1; // if has still some mutation to read
	int next = 0; // number of mutation read so far
	std::string sname = "";
	double rate = 1;
	int cl = 0;
	while ( left )
	{
		left = reader->getStringValue( what, "symbol_name", &sname, "mutation_"+std::to_string(next) );
		left *= reader->getDoubleValue( what, "rate", &rate, "mutation_"+std::to_string(next) );
		left *= reader->getIntValue( what, "cell_line", &cl, "mutation_"+std::to_string(next) );
		
		// has succeed to read one other mutation, add it in the map
		if ( left )
		{
			strip(&sname);
			mbRates[std::pair<std::string, int>(sname, cl)] = rate;
			next ++;
		}
	}
}

/* Write current properties to output stream */
void MaBossNetwork::write_properties(std::ostream& os)
{
	os << "\t<!-- ------ MaBoSS network properties ------- -->" << std::endl;
	os << "\t\t<network_update_step> " << update_time << " </network_update_step>" << std::endl;
	os << std::endl;
}

/* Initialize the network by reading input files */
void MaBossNetwork::init( std::string networkFile, std::string configFile )
{
		network->parse(networkFile.c_str());
		conffile = configFile;
		runConfig->parse(network, conffile.c_str());
		
		// This was missing, for some reason
		network->updateRandomGenerator(RunConfig::getInstance());
		
		initNetworkState();
}

void MaBossNetwork::initNetworkState()
{
	runConfig->setSeedPseudoRandom( UniformInt() );
	std::vector<Node *> nodes = network->getNodes();
	int i = 0;
	def_nodes.resize( nodes.size() );
	for (auto node : nodes)
	{
		node_names[ node->getLabel() ] = i;
		def_nodes[i] = (bool) node->getIState( network );
		i++;	
		//std::cout << "initial state of node " << node->getLabel() << " = " << (bool)node->getIState(network) << std::endl;
	}
}

/* Set values of nodes to default values */
void MaBossNetwork::set_default( std::vector<bool>* nodes )
{
	for ( int i = 0; i < (int) def_nodes.size(); i++ )
	{
		(*nodes)[i] = def_nodes[i];
	}
}


/* Load previous network states and inputs */
void MaBossNetwork::load( NetworkState* netState, std::vector<bool>* inputs )
{
	network->initStates( (*netState) );
	std::vector<Node*> nodes;
	nodes = network->getNodes();
	int ind = 0;
	for (auto node : nodes)
	{
		node->setIState( (*inputs)[ind] );
		ind ++;
	}
}

/* Load symbol rates value if they are in the map for the current cell line, usefull for mutations */
void MaBossNetwork::loadSymbol( int cellline )
{
	for (auto symit = mbRates.begin(); symit != mbRates.end(); symit++ )
	{
		// current cell line, keep it
		if ( symit->first.second == cellline )
		{
			const Symbol* symb = symbTable->getSymbol( symit->first.first );
			if ( symb != NULL )
			{
				symbTable->overrideSymbolValue( symb, symit->second );
				//std::cout << symit->second << " " << symit->first.first << " " << symit->first.second << std::endl;
			}
		}

	}
}

/* Run the current network */
bool MaBossNetwork::run( NetworkState* netStates, std::vector<bool>* nodes_val, int cellline )
{
	runConfig->setSeedPseudoRandom( UniformInt() ); // pick random numbber
	// Load network state and values of current cell in the network instance
	loadSymbol( cellline );
	load( netStates, nodes_val );
	MaBEstEngine mabossEngine( network, runConfig );
	// No output from MaBoSS run
	std::ostream* os = NULL; 
	// And run it
	mabossEngine.run(os);
		
	// save fixed point as initial state for the network for the next time step
	const STATE_MAP<NetworkState_Impl, double>& states = mabossEngine.getAsymptoticStateDist();
	if (states.begin() != states.end()) 
	{
		(*netStates) = states.begin()->first;
	}
	bool converged = true;	
	/**if ( ! mabossEngine.converges() )
	{
		std::cerr << "WARNING: Network did not converge;" << std::endl;
//		converged = false;
	}*/

	int i = 0;
	std::vector<Node*> node3 = network->getNodes();
	for ( auto node: node3 )
	{
		(*nodes_val)[i] = netStates->getNodeState( node ) ;
		//std::cout << node->getLabel() << " " << (*nodes_val)[i] << std::endl;
		i ++;
	}
	//std::cout << std::endl;

	return converged;
}

/* Print current state of all the nodes of the network */
void MaBossNetwork::printNodes(NetworkState* netStates)
{
	std::vector<Node*> node3 = network->getNodes();
	for ( auto node: node3 )
	{
		std::cout << node->getLabel() << "=" << netStates->getNodeState(node) << "; ";
	}
	std::cout << std::endl;
}

int MaBossNetwork::get_node_index( std::string name )
{
	auto res = node_names.find(name);
	if ( res != node_names.end() )
		return res->second;
	return -1;	
}
