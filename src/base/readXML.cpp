#include "readXML.h"

ReadXML::ReadXML(): doc()
{
	fileName = "parameters.xml";
	pSimu = NULL;
	pCells = NULL;
	pInit = NULL;
	pNetwork = NULL;
	props.resize(0);
}

ReadXML::~ReadXML()
{
}

/** read the file with given path and load datas as XMLDocument */
void ReadXML::loadXMLFile( std::string cat0, std::string cat1, std::string cat2, std::string ncat ) throw()
{
	std::cout << "Reading input parameter XML file " << fileName << "..." << std::endl;
	// load the file
	XMLError eRes = doc.LoadFile( fileName.c_str() );
	if ( !XMLCheckResult( eRes ) ) return;
	
	// load first node: simulation parameters
	pSimu = doc.FirstChildElement( cat0.c_str() );
	if ( pSimu == NULL )
	{
		std::cout << "First node: " << cat0 << " is not defined in the XML file" << std::endl;
	}
	names[cat0] = 0;

	pCells = doc.FirstChildElement(ncat.c_str());

	if ( pSimu != NULL && pCells != NULL )
		std::cout << "xml file loaded \n" << std::endl;

	int num = 0;
	XMLElement* propCells = doc.FirstChildElement(ncat.c_str());
    props.push_back( propCells );	
	names[ncat+"_"+std::to_string(num)] = num + 3;
	num ++;

	while ( pCells != doc.LastChildElement(ncat.c_str()) )
	{
		XMLElement* prCells = pCells->NextSiblingElement(ncat.c_str());
		props.push_back( prCells );
		names[ncat+"_"+std::to_string(num)] = num + 3;
		num ++;
		pCells = pCells->NextSiblingElement(ncat.c_str());
	}

	pInit = doc.FirstChildElement(cat1.c_str());
	if ( pInit == NULL )
	{
		std::cout << "no initial configuration defined in parameter file" << std::endl;
	}
	names[cat1] = 1;
	
	pNetwork = doc.FirstChildElement(cat2.c_str());
	if ( pNetwork == NULL )
	{
		std::cout << "no network configuration defined in parameter file" << std::endl;
	}
	names[cat2] = 2;


}

/** Check if there was a pb or not */
int ReadXML::XMLCheckResult( XMLError a_eResult) 
{
	if ( a_eResult != XML_SUCCESS ) 
	{ 
		printf("!!!!!! Error in XML file: %i !!!!!!! \n", a_eResult); 
		return 0; 
	}
	return 1;
}


/** Retrieve node of given name 
 * in the element container number element:
 * 0 = Simu
 * 1 = Initial configuration
 * 2 = Network
 * > 3: Cell properties
 * sub: if has a subchild where should look for the parameter
 */
XMLElement* ReadXML::getFirstChild( std::string where, std::string name, std::string sub, std::string subsub )
{
	int which = names[where];
	XMLElement* pRoot = NULL;
	if ( which == 0 )
			pRoot = pSimu;
	else if ( which == 1 )
			pRoot = pInit;
	else if ( which == 2 )
			pRoot = pNetwork;
	else
	{
		int ind = which - 3;
		pRoot = props[ind];
	}

	if ( pRoot == NULL ) 
	{
		std::cerr << std::endl;
		std::cerr << "!!! Element container in XML file not read !!!" << std::endl;
		throw;
	}
	
	// the value should be in child node
	if ( sub != "" )
	{
		XMLElement* subChild = pRoot->FirstChildElement( sub.c_str() );
		if ( subChild == NULL )
		{
			std::cout << "Node " << sub << " in node " << where << " not found" << std::endl;
			return NULL;
		}
		pRoot = subChild;
	}
	
	// the value should be in child of child node
	if ( subsub != "" )
	{
		XMLElement* subsubChild = pRoot->FirstChildElement( subsub.c_str() );
		if ( subsubChild == NULL )
		{
			std::cout << "Node " << subsub << " in node " << sub << " in node " << where << " not found" << std::endl;
			return NULL;
		}
		pRoot = subsubChild;
	}

	XMLElement * pEl = pRoot->FirstChildElement( name.c_str() );
	
	if ( pEl == NULL )
	{
		std::cout << "Parameter or Node " << name << " not found in parameter file." << std::endl;
		return NULL;
	}
	
	return pEl;
}

/** Retrieve string value of parameter name and replace value if found */
int ReadXML::getStringValue( std::string where, std::string name, std::string* num, std::string sub, std::string subsub )
{
	XMLElement * pEl = getFirstChild( where, name, sub );
	if ( pEl == NULL )
	{
		std::cout << " Keep default value: " << (*num) << std::endl;
		return 0;
	}
	
	(*num) = std::string( pEl->GetText() );
	if ( !num )
	{
		std::cout << "Parameter " << name << " not well-defined in parameter file. Keep default value: " << (*num) << std::endl;
		return 0;
	}	
	return 1;
}


/** Retrieve double value of parameter name and replace value if found */
int ReadXML::getDoubleValue( std::string where, std::string name, double* num, std::string sub, std::string subsub )
{
	XMLElement * pEl = getFirstChild( where, name, sub );
	if ( pEl == NULL )
	{
		std::cout << " Keep default value: " << (*num) << std::endl;
		return 0;
	}
	
	XMLError eRes = pEl->QueryDoubleText( num );
	if ( !XMLCheckResult( eRes ) )
	{
		std::cout << "Parameter " << name << " not well-defined in parameter file. Keep default value: " << (*num) << std::endl;
		return 0;
	}
	return 1;
}

/** Retrieve int value of parameter name and replace value if found */
int ReadXML::getIntValue( std::string where, std::string name, int* num, std::string sub, std::string subsub )
{
	XMLElement * pEl = getFirstChild( where, name, sub );
	if ( pEl == NULL )
	{
		std::cout << " Keep default value: " << (*num) << std::endl;
		return 0;
	}
	
	XMLError eRes = pEl->QueryIntText( num );
	if ( !XMLCheckResult( eRes ) )
	{
		std::cout << "Parameter " << name << " not well-defined in parameter file. Keep default value: " << (*num) << std::endl;
		return 0;
	}
	return 1;
}
