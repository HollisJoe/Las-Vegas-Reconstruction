/*
 * DataCollector.h
 *
 *  Created on: 07.10.2010
 *      Author: Thomas Wiemann
 */

#ifndef DATACOLLECTOR_H_
#define DATACOLLECTOR_H_



#include <string>
using std::string;

#include "model3d/Renderable.h"

#include "../app/Types.h"
#include "../viewers/Viewer.h"
#include "../widgets/CustomTreeWidgetItem.h"

class DataManager;

class DataCollector
{
public:
	DataCollector(Renderable* renderable, string name, DataManager* manager, CustomTreeWidgetItem* item = 0);
	virtual ~DataCollector();
	Renderable* renderable();
	string	name();
	BoundingBox* boundingBox() { return m_renderable->boundingBox();}

	virtual CustomTreeWidgetItem* treeItem() { return m_treeItem;}
	virtual ViewerType supportedViewerType() = 0;

protected:

	CustomTreeWidgetItem*        m_treeItem;
	DataManager* 	        m_manager;
	Renderable*	            m_renderable;
	string		            m_name;


};

#endif /* DATACOLLECTOR_H_ */
