#ifndef _MaBoSS_Net_h_
#define _MaBoSS_Net_h_

/**
 * \brief Interface with MaBoSS software
 *
 * One network object contains network configs.
 * Handle initialization, running it...
 *
 * Created on 05/19/2017
 * G. Letort, Institute Curie
 */

#include "MaBEstEngine.h"
#include "../base/readXML.h"

class MaBossNetwork
{

	private:
		/** \brief MaBoSS instances: network */
		Network* network;
		/** \brief MaBoSS instances: configurations */
		RunConfig* runConfig;
		/** \brief MaBoSS instances: symbols list */
		SymbolTable* symbTable;

		/** \brief Name of input configuration file */
		std::string conffile;

		/** \brief Time step to update the cycle */
		double update_time;

		/** \brief Names and indices of network nodes */
		std::map< std::string, int > node_names;
		
		/** \brief List of special rate values in maboss network, usefull to simulate mutations
		 *
		 * Map key is the node name (first element) and cell line number (second element).
		 * Map value is the corresponding rate value */
		std::map< std::pair<std::string,int>, double> mbRates;

		/** \brief Keep default value of nodes from CFG file */
		std::vector<bool> def_nodes;

	protected:

	public:
		/** \brief Constructor */
		MaBossNetwork();
		/** \brief Destructor */
		~MaBossNetwork();

		/** \brief Initialize the network from input files */
		void init( std::string networkFile, std::string configFile );
		/** \brief Read and load nodes initial states */
		void initNetworkState();

		/** \brief return number of nodes */
		inline int number_of_nodes()
		{ return node_names.size(); };
		
		/** \brief Set values of nodes to default values */
		void set_default( std::vector<bool>* nodes );

		/** \brief Read properties from XML parameter file for network */
		void read_properties( ReadXML* reader, std::string what );
		
		/** \brief Print current state of all the nodes of the network */
		void printNodes( NetworkState* netStates);

		/** \brief If time to update network */
		inline bool time_to_update( double elapsed_time )
		{ return ( elapsed_time > update_time ); };

		/** \brief Return update time value */
		inline double update_time_step()
		{ return update_time; };

		/** \brief Run the current network
		 *
		 * Otherwise put new network states in input netStates
		 * cellline: index of current cell_line for specific properties (rates) 
		 * */
		bool run( NetworkState* netStates, std::vector<bool>* nodes_val, int cellline );
		
		/** \brief Load previous network states and inputs */
		void load( NetworkState* netState, std::vector<bool>* inputs );

		/** \brief Load symbol rates value if they are in the map for the current cell line */
	void loadSymbol( int cellline );

		/** \brief Return node of given name current value
		 *
		 * Return -1 if node doesn't exit \n
		 * Return 0 if node is 0 \n
		 * Return 1 if node is 1 */
		int get_node_index( std::string name );

		/** \brief Write current properties to output stream */
		void write_properties(std::ostream& os);
};

#endif
