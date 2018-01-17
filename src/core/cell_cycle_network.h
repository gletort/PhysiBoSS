#ifndef __CELL_CYCLE_NETWORK_H
#define __CELL_CYCLE_NETWORK_H

#include "MaBEstEngine.h"
#include "cell_cycle.h"
#include "maboss_network.h"

/**
 * \class CellCycleNetwork
 * \brief Cell Cycle directed by boolean network
 *
 *	Cell cycle and cell properties are controlled by the boolean network linked to the cell, calling MaBoSS instance.
 *	Takes as input current state of the cell (surrounding, acces to O2, contact with other cells, with ECM...) and read outputs from the netowrk (Migration, Proliferation...), and update cell state accordingly (cycle phase, migrative parameter...).
 *
 * Possible inputs and outputs depends on the structures of the network, try to put different possibilities.
 *
 *	\date 05/30/2017
 *	\author Gaelle Letort, Institut Curie
 */

class CellCycleNetwork : public CellCycle
{
	private:

			/** \brief MaBoss Network doing the job */
			MaBossNetwork* maboss;
			
			/** \brief Vector of nodes state current value (0 or 1) */
			std::vector<bool> nodes;

			/** \brief Current state of the network (MaBoSS object) */
			NetworkState networkState;


			/** \brief time left before to update it again */
			double time_to_update;

			/** \brief Tmp attribute to check if got NFkB to 1 at some point or never, can put something else in it or remove later */
			double got_activated;

	protected:

	public:
		/** \brief Default constructor */
		CellCycleNetwork();
		/** \brief Constructor with given network instance */
		CellCycleNetwork( MaBossNetwork* mboss);
		/** \brief Class destructor */
		~CellCycleNetwork();

		/** \brief Initialization: set network 
		 *
		 * @param mboss reference to the common object handling MaBoSS runs */
		void set_maboss( MaBossNetwork* mboss );
	
		/** \brief Copy all parameters specific to cell cycle network object 
		 *
		 * Usefull in particular for dividing cell
		 * @param copy_me cell cycle network object to be copied in current object */
		void copy_cycle( CellCycleNetwork* copy_me );
		
		/** \brief Update MaboSS network states 
		 *
		 * @param dt update network time step 
		 * @param t current time in the simulation */
		void run_maboss(double dt, double t);
		
		/** \brief Set inputs of the network from current cell state 
		 *
		 * For all possible input of MaBoSS network, check if each node is present in MaBoSS network, and if yes, specify its value according to current cell state or surrounding. \n
		 * This is the function to change to add new network input nodes possible. */
		void set_input_nodes();
		
		/** \brief choose a random update time, to asynchronize it between all cells 
		 *
		 * Set the next time at which to update the current cell's network. The time in between two udpates is chosen randomly in order to not update all cells together. */
		void set_time_to_update();
		
		/** \brief Randomize some nodes values */
		void randomize_nodes();
		
		/** \brief What to do when dying cell (apoptosis) 
		 *
		 * \todo put this function in common parrent object CellCycle ? */
		void dying_apoptosis(double dt);

		/** \brief Necrosis lysis dying step 
		 *
		 * \todo put this function in common parent object CellCycle ? */
		void dying_necrosis_lysed(double dt);

		/** \brief Necrosis_swelling dying step 
		 *
		 * \todo put this function in common parent CellCycle object ? */
		void dying_necrosis_swell(double dt);
		
		/** \brief Step just after mitosis (not recycling yet) 
		 *
		 * \todo to put in common CellCycle parent object */
		void after_division(double dt);
		
		/** \brief Go to proliferative phase if proliferation ON and in G0 phase */
		void do_proliferation(double dt);

		/** \brief Do one cell cycle step 
		 *
		 * Main function. \n
		 * Check the current phase of the cell in the cycle, and update the network or the cell cycling according ot it. Call MaBoSS run if has to update it. */
		virtual void do_one_cycle_step(double dt, double t);

		/** \brief For a given node name, test if there are enough protein of given field 
		 *
		 * If the field is not present on the microenv, don't set anthing, then initial value from BN conf files is used */
		void enough_to_node( std::string nody, std::string field );

		/** \brief Read output nodes and transfer result to the cell 
		 *
		 * Read all the possible output nodes of the network. If present in the current network, read its value and modify the current cell in response to it. \n
		 * This is the function to change to add implementation of output nodes of the network. 
		 * @param dt cycling time step
		 * @param t current time in the simulation */
		void from_nodes_to_cell(double dt, double t);
		
		/** \brief De-mobilize the cell components (dying cells): actin, cadherins, integrins 
		 *
		 * @param dt time step 
		 * */
		void demobilize(double dt);
		
		/** \brief Output current phase of the cell */
		virtual void output( std::string& delimeter, std::ofstream* file );

};

#endif
