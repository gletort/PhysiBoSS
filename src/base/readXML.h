#ifndef _READ_XML_
#define _READ_XML_

#include <iostream>
#include "../tinyxml/tinyxml2.h"
#include <vector>
#include <map>

using namespace tinyxml2;

/**
 * \brief Handle parsing of PhysiBoSS-specific XML files
 *
 * Use tinyXML2 library (developped by Lee Thomason (<a href="http://www.grinninglizard.com"> website </a>) and freely available to parse the XML file. \n
 * Extract structures and values specific to PhysiBoSS implementation
 *
 * \author Gaëlle Letort, Institut Curie
 * \date March 2017
 * */

class ReadXML
{
	private:
		XMLDocument doc;

		// Datas of each part of the xml file
		/** Contains parameters of the simulation, nb 0 */
		XMLElement* pSimu;   
		/** Contains parameters of cell properties, nb 3 */
		XMLElement* pCells;
		/** \brief Vector of all cell line properties definition */
		std::vector<XMLElement*> props;
		/** Contains parameters of initial configuration, nb 1 */
		XMLElement* pInit;
		/** Contains parameters of the network when is used, nb 2 */
		XMLElement* pNetwork;   

		/** \brief Name of the XML file to read */
		std::string fileName;

		/** \brief Convert prop name to index */
		std::map<std::string, int> names;

	public:
		ReadXML();
		~ReadXML();

		/** \brief Set the parameter file name */
		inline void set_file( std::string name ) 
		{ fileName = name; }; 

		/** Check if there was a pb or not */
		int XMLCheckResult( XMLError a_eResult );

		/** \brief Load datas as XMLDocument 
		 *
		 * cat0 is the name of the first node (simulation for parameter file)
		 * cat1 is the name of the second node (initial_configuration for parameter file)
		 * cat2 is the name of the network node (network for parameter file)
		 * ncat is the name of the node that can have one or more repetition (cell_properties for parameter file) */
		void loadXMLFile( std::string cat0, std::string cat1, std::string cat2, std::string ncat) throw();

		/** \brief Returns number of cell lines defined */
		inline int nbCellProp() { return (int) props.size(); };

/** \brief Retrieve node of given name 
 *
 * in the element container number element:
 * 0 = Simu
 * 1 = Initial configuration
 * > 2: Cell properties
 * sub: sub child name if should look in it
 */
XMLElement* getFirstChild( std::string where, std::string name, std::string sub = "", std::string subsub = "" );

	/** \brief Retrieve double value of parameter name and replace value if fond
	 * in the element container named where
	 * sub: name of the child of child (e.g. phenotype is child of property)
	 */
	int getDoubleValue( std::string where, std::string name, double* num, std::string sub = "", std::string subsub = "" );
	
	/** \brief Retrieve int value of parameter name and replace value if found
	 * in the element container named where
	 * sub: name of the child of child (e.g. phenotype is child of property)
	 */
	int getIntValue( std::string where, std::string name, int* num, std::string sub = "", std::string subsub = "" );
	
	/** \brief Retrieve string of parameter name and replace value if found
	 * in the element container named where
	 */
	int getStringValue( std::string where, std::string name, std::string* num, std::string sub = "", std::string subsub = "" );
};


#endif
