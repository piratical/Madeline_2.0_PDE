
#include <iostream>
#include <fstream>
#include <sstream>

#include "CLP.h"
#include "Version.h"
#include "Debug.h"
#include "Parser.h"
#include "DataTable.h"
#include "Pedigree.h"
#include "PedigreeSet.h"
#include "Data.h"
#include "DrawingMetrics.h"
#include "Utility.h"
#include "VT100.h"

int main( int argc, char *argv[] ){
	
	std::ifstream labelreader;
	std::vector<std::string> showColumns; // vector containing field labels that need to be displayed on the pedigree
	CLP clp;
	// ABCDEFGH:
	clp.addSwitch("--bw","-b","Print pedigrees in black and white");
	clp.addSwitch("--color","-c","Print pedigrees in color");
	clp.addSwitch("--custom-icon-colors","-C","Specify a comma- and semicolon-delimited list of custom icon shading color codes in HTML/CSS hex format.",1);
	clp.addSwitch("--debug","-d","Print run-time progress messages");
	clp.addSwitch("--embedded","-e","Produce an XML file that can be embedded in another XML document");
	clp.addSwitch("--font","-f","Font to be used for the display of Pedigree labels",1);
	clp.addSwitch("--font-size","-z","Font size to be used for the display of Pedigree labels",1);
	clp.addSwitch("--help","-h","Print this help and exit");
	clp.addSwitch("--collapsed","-k","“Collapse” multiple individuals into groups (requires “Collapsed” data column)");
	// IJKLMNOP:
	clp.addSwitch("--labels","-l","Path to the file that has a list of labels to be displayed on the Pedigree",1);
	clp.addSwitch("--Labels","-L","Specify labels to be displayed on the Pedigree using a single string containing space-delimited labels",1);
	clp.addSwitch("--noiconlabels","-n","Prevent the printing of affection status labels on the icons");
	clp.addSwitch("--nolabeltruncation","-N","Prevent truncation of labels on a drawing when they are too wide.");
	clp.addSwitch("--outputpedtable","-i","Output the pedigree core/non-core fields as a tab-delimited file with name 'FAMILY_IDinput.txt'");
	clp.addSwitch("--outputdatatable","-I","Output the pedigree input data file (column order preserved) as a tab-delimited file with name 'FAMILY_IDinput.txt'");
	clp.addSwitch("--outputprefix","-o","Specify output file name prefix",1);
	clp.addSwitch("--outputext","-x","Specify output file name extension",1);
	// QRSTUVWXYZ:
	clp.addSwitch("--quadrantshading","-q","Use the quadrant shading method to indicate categorical levels on icons");
	clp.addSwitch("--sort","-s","Field based on which siblings are sorted",1);
	clp.addSwitch("--version","-v","Print version and exit");
	clp.addUsage("madeline2 [option]... [file]...\n\nIf input file is remote, specify the file\nname starting with 'http://' or 'https://'.\nTo retrieve the data from a mysql database use\n'mysql://[host:port/]username:passwd@database:table'");
	
	if(clp.parse(argc,argv)){
		// No error, process the switches
		if(clp.hasSwitchSet("--help")){
			clp.printHelp();
			return 0;
		}
		if(clp.hasSwitchSet("--version")){
			Version ver("MiniMadeline","1");
			ver.printVersion();
			return 0;
		}
		if(clp.hasSwitchSet("--debug")){
				std::cout << "Switch DEBUG is set" << std::endl;
				Debug::DEBUG=true;
		}
		if(clp.hasSwitchSet("--font-size")){
			std::string fontSizeArgument = clp.getSwitchArgument("--font-size",1);
			if(fontSizeArgument != "."){
				double fontSize;
				std::stringstream sstr(fontSizeArgument);
				sstr >> fontSize;
				DrawingMetrics::setFontSize(fontSize);
			}
		}
		if(clp.hasSwitchSet("--font")){
			std::string fontFamily = clp.getSwitchArgument("--font",1);
			if(fontFamily != ".")
				DrawingMetrics::setFontFamily(fontFamily);
		}
		
		//
		// Set drawing metrics for embedded drawings:
		//
		if( clp.hasSwitchSet("--embedded")){
			DrawingMetrics::setEmbeddedState(true);
		}
		
		//
		// Handle a file containing a list of labels, one per line:
		//
		if(clp.hasSwitchSet("--labels") ){
			std::string labelarg = clp.getSwitchArgument("-l",1);
			std::cout << "Labels are in an external file " << labelarg << std::endl;
			
			// Read the labels to be displayed on the pedigree
			labelreader.open(labelarg.c_str());
			std::string label;
			while(getline(labelreader,label))
			{
				//
				// Trim extra white space from labels:
				//
				
				showColumns.push_back(Data::trim(label));
			}
			labelreader.clear();
			labelreader.close();
		}
		
		//
		// Handle a string containing labels to be tokenized
		// and used as the labels on the pedigree drawing:
		//
		if(clp.hasSwitchSet("--Labels")){
			std::string labelString = clp.getSwitchArgument("--Labels",1);
			showColumns = utility::split(labelString);
		}
		
		//
		// Handle custom icon colors:
		//
		if(clp.hasSwitchSet("--custom-icon-colors")){
			
			// Set the boolean switch in DrawingMetrics:
			DrawingMetrics::setUseCustomIconColors(true);
			std::string colorString = clp.getSwitchArgument("--custom-icon-colors",1);
			std::istringstream css(colorString);
			std::string t1,t2;
			ColorSeries tempCS; // temporary color series storage
			std::vector<std::string> tempV; // temporary vector for push back
			std::map<std::string,DrawingColor> tempM; // temporary map for push back
			// Break each set at semicolon delimiters:
			for(unsigned i=0; std::getline(css,t1,';');i++){
				tempV.clear();
				tempCS.clear();
				std::istringstream csst(t1);
				// For each set of colors, break at comma delimiters
				// and push colors onto a temp vector:
				while(std::getline(csst,t2,',')){
					if(t2.find("=")!=std::string::npos){
						// String contains a mapping:
						std::istringstream cssu(t2);
						std::string t3,t4;
						std::getline(cssu,t3,'=');
						std::getline(cssu,t4);
						// Push to map:
						tempM[t3]=DrawingColor(t4,t4);
					}else{
						tempV.push_back(t2);
					}
				}
				if(tempM.size()){
					DrawingMetrics::customColorMap.push_back(tempM);
					std::cout << "MAP " << (i+1) << ":" << std::endl;
					for (std::map<std::string,DrawingColor>::iterator it=DrawingMetrics::customColorMap[i].begin(); it!=DrawingMetrics::customColorMap[i].end(); ++it){
						std::cout << "   " << it->first << " => " << it->second.get() << std::endl;
					}
					tempM.clear();
				}
				// Push the strings in reverse order onto the 
				// tempCS ColorSeries:
				std::vector<std::string>::reverse_iterator r = tempV.rbegin();
				for(; r!= tempV.rend(); ++r){
					tempCS.pushBack(*r);
				}
				// Finally, store each custom color series in the static DrawingMetrics vector:
				DrawingMetrics::customColorSeries.push_back(tempCS);
			}
			// verbose reporting:
			std::cout << "Custom color series: " << DrawingMetrics::customColorSeries.size() << std::endl;
			for(unsigned i=0;i<DrawingMetrics::customColorSeries.size();i++){
				unsigned jmax = DrawingMetrics::customColorSeries.at(i).levels();
				std::cout << "Custom series " << (i+1) << " has " << jmax << " levels:" << std::endl;
				for(unsigned j=0;j<jmax;j++){
					std::cout << "   " << (i+1) << "." << (j+1) << ". " << DrawingMetrics::customColorSeries[i].getColorAtLevel(j) << std::endl;
				}
			}
		}
		//
		// Handle specification of a file name prefix:
		//
		if(clp.hasSwitchSet("--outputprefix")){
			DrawingMetrics::setDrawingFileNamePrefix( clp.getSwitchArgument("--outputprefix",1) );
		}
		//
		// Handle specification of special file name extension:
		//
		if(clp.hasSwitchSet("--outputext")){
			DrawingMetrics::setDrawingFileNameExtension( clp.getSwitchArgument("--outputext",1) );
		}
		
		//
		// Handle the black and white override flag:
		//
		if(clp.hasSwitchSet("--bw")){
			DrawingMetrics::setBlackAndWhite(true);
		}
		//
		// Handle the color override flag:
		//
		if(clp.hasSwitchSet("--color")){
			DrawingMetrics::setColor(true);
		}
		//
		// Handle the quadrant shading method override flag:
		//
		if(clp.hasSwitchSet("--quadrantshading")){
			DrawingMetrics::setQuadrantShading(true);
			//
			// When quadrant shading is turned on,
			// we need to turn icon labeling off:
			//
			DrawingMetrics::setNoIconLabels(true);
		}
		//
		// Handle the noiconlabels override flag:
		//
		if(clp.hasSwitchSet("--noiconlabels")){
			DrawingMetrics::setNoIconLabels(true);
		}
		
		//
		// Handle the noLabelTruncation override flag:
		//
		if(clp.hasSwitchSet("--nolabeltruncation")){
			DrawingMetrics::setNoLabelTruncation(true);
		}
		
		//
		// Print pedigree core/non-core fields to a tab-delimited file
		//
		if(clp.hasSwitchSet("--outputpedtable")){
			DrawingMetrics::setOutputPedTable(true);
		}
		
		//
		// Print pedigree input data table (order of columns preserved) to a tab-delimited file
		//
		if(clp.hasSwitchSet("--outputdatatable")){
			DrawingMetrics::setOutputDataTable(true);
		}
		
		//
		// Set Pedigree class for collapsing drawings:
		//
		if( clp.hasSwitchSet("--collapsed")){
			DrawingMetrics::setCollapsible(true);
		}
		///////////////////////
		//
		// MAIN PROCESSING:
		//
		///////////////////////
		
		
		//
		// DEBUGGING: Show column information
		//
		std::cout << "--------------------------------------------" << std::endl;
		std::cout << vt100::startBlue << " LABELS                          TOTAL: " << showColumns.size() << vt100::stopColor << std::endl;
		std::cout << "--------------------------------------------" << std::endl;
		for(unsigned i=0;i<showColumns.size();i++){
			std::cout << " " << (i+1) << ". " << showColumns[i] << std::endl;
		}
		std::cout << "--------------------------------------------" << std::endl;
		std::vector<std::string> arguments = clp.getArguments();
		
		//
		// The parser holds the DataTables:
		//
		Parser dataTableParser;
		
		//
		// Read data tables from disk files:
		//
		for(unsigned i=0;i<arguments.size();i++){
			
			dataTableParser.readFile(arguments[i]);
		}
		//
		// Read data tables from network files:
		//
		std::vector<std::string> networkarguments = clp.getNetworkArguments();
		for(unsigned i=0;i<networkarguments.size();i++){
			dataTableParser.readNetworkFile(networkarguments[i]);
		}
		
		//
		// Read data from a mysql database table:
		//
		#ifdef MYSQLPP
		if(clp.hasMysqlArguments()){
			std::vector<std::string> mysqlarguments = clp.getMysqlArguments();
			dataTableParser.connectMysql(mysqlarguments[0],mysqlarguments[1],mysqlarguments[2],mysqlarguments[3],mysqlarguments[4]);
			dataTableParser.readMysql(mysqlarguments[5]);
		}
		#endif
		//
		// Process data tables read by the parser
		//
		DataTable *dataTable;
		for( int j=0 ; j < dataTableParser.getNumberOfTables() ; j++ ){
			
			PedigreeSet pedigreeSet;
			
			dataTable = dataTableParser.getTable(j);
			
			// DEBUG:
			//dataTable->display();
			//
			// Tell user the type of table:
			//
			std::cout << "Table " << (j+1) << " is a " << dataTable->getTableTypeAsString() << " table." << std::endl;
		
			//
			// Draw pedigrees
			//
			if( dataTable->getTableType() == DataTable::PEDIGREE ){
				
				// Pedigree table
				dataTable->toggleColumnsForPedigree(showColumns);
				
				
				std::string sortField = "";
				
				if(clp.hasSwitchSet("-s")){
					sortField = clp.getSwitchArgument("-s",1);
				}
				
				pedigreeSet.addPedigreesFromDataTable(dataTable,j,sortField);
				pedigreeSet.draw(dataTable);
			}
		}
		
		
	}
	
	return 0;
	
}

