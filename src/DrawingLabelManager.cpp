/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager and Ritu Khanna
// Copyright (c) 2005 by the
// Regents of the University of Michigan.
// All Rights Reserved.
// 
// The latest version of this program is available from:
// 
//   http://eyegene.ophthy.med.umich.edu/madeline/
//   
// Released under the GNU General Public License.
// A copy of the GPL is included in the distribution
// package of this software, or see:
// 
//   http://www.gnu.org/copyleft/
//   
// ... for licensing details.
// 
/////////////////////////////////////////////////////////

#include "DrawingLabelManager.h"
#include "DataTypes.h"
#include <vector>

// Madeline's drawing metrics:
#include "DrawingMetrics.h"
#include "Exception.h"
#include "LASiWrapper.h"


//
// The _latinBlock is basic Latin (ASCII):
//
UnicodeScript DrawingLabelManager::_latinScript;
//
// The _aggregateBlock sample is sufficient for determining
// good line spacing metrics for most Unicode blocks beyond basic Latin:
// 
UnicodeScript DrawingLabelManager::_aggregatedScripts;
//
// The following set of _unicodeBlocks consists primarily of Indic and Indic-derived scripts
// which have vowels or other diacritical marks both above and below the consonants
// which requires greater inter-line spacing:
//
std::map<SCRIPTCODE,UnicodeScript> DrawingLabelManager::_unicodeScripts;
//
// The static instance of the DrawingLabelManagerLoader:
//
DrawingLabelManagerLoader DrawingLabelManagerLoader::drawingLabelManagerLoader;


//
// Constructor:
//
DrawingLabelManager::DrawingLabelManager(){
	
	_pLASiWrapper=0;
	_hasUnicode=_hasIndicOrArabic=false;
	_yMaximum=_yMinimum=_lineHeight=_labelCount=0;
	
}

//
// setLASiWrapper
//
void DrawingLabelManager::setLASiWrapper(LASiWrapper* pLASiWrapper){
	
	if(pLASiWrapper==0) throw Exception("DrawingLabelManager::setLASiWrapper()","Pointer to LASiWrapper is NULL.");
	
	_pLASiWrapper = pLASiWrapper;
	
}

//
// setLabelSet()
//
void DrawingLabelManager::setLabelSet(const LabelSet* labelSet){
	
	_hasUnicode=false;
	_hasIndicOrArabic=false;
	
	std::vector<UTF8String>sampleLabels = labelSet->getRandomLabelSubsetByType(STRING);
	_labelCount = sampleLabels.size();
	
	// Determine if any labels are outside of ASCII range:
	
	UTF32 sampleUnicodeValue;
	
	// Don't try to dereference a null pointer:
	if(_pLASiWrapper==0) throw Exception("DrawingLabelManager::setLabelSet()","Pointer to LASiWrapper is NULL. Was setPostscriptDocument() called?");
	
	//LASi::PostscriptDocument doc;
	_pLASiWrapper->setFont(DrawingMetrics::getFontFamily().c_str());
	
	std::string fontSizeUnit = DrawingMetrics::getFontSizeUnit();
	double fontSize          = DrawingMetrics::getFontSize();
	//
	// LASi works with points as the native unit.  Therefore
	// convert if FontSize not in "pt" points:
	//
	if( fontSizeUnit == std::string("mm")){
		// Convert from mm to points by dividing by mm/pt conversion factor:
		fontSize/=DrawingMetrics::getPointsToMmConversionFactor();
	}else if( fontSizeUnit == std::string("px")){
		// Convert from pixels to points:
		// Scaling Factor give pixels/mm. pointsToMm...() gives mm/pt.
		// Therefore multiplying those two gives pixels/point.
		// Reciprocal is points/pixel:
		double factor = DrawingMetrics::getScalingFactor()*DrawingMetrics::getPointsToMmConversionFactor();
		fontSize/=factor;
	}else if( fontSizeUnit != std::string("pt")){
		std::cerr << "DrawingLabelManager(): Unknown font size unit: " << fontSizeUnit << std::endl;
	}
	//
	// If font size is in points already, no conversion or scaling is required:
	//
	_pLASiWrapper->setFontSize(fontSize);
	double xAdvance,yMinimum,yMaximum,lineHeight;
	//
	// Set lineHeight, yMinimum & yMaximum assuming basic Latin sample first:
	//
	_pLASiWrapper->getDimensions(_latinScript.getSample(),&_lineHeight,&xAdvance,&_yMinimum,&_yMaximum);
	
	SCRIPTCODE script;
	std::string sampleString;
	static std::map<SCRIPTCODE,UnicodeScript,UnicodeScript::compare>::iterator found;
	//
	// Now iterate over sample labels looking for non-ASCII Unicode labels:
	//
	for(unsigned i=0; i < _labelCount ;i++){
		
	      sampleUnicodeValue = sampleLabels[i].unicodeValueAtPosition(1);
		if(sampleUnicodeValue > 0x007f){
			//
			// It is necessary to set _hasUnicode 
			// and get font metrics from the _aggregatedScripts sample
			// just the first time we detect Unicode:
			//
			if(!_hasUnicode){
				_hasUnicode = true;
				//
				// ... So initially just use the aggregate scripts sample
				//     whose lineHeight dimensions will be presumably greater
				//     than the basic Latin default:
				//
				_pLASiWrapper->getDimensions(_aggregatedScripts.getSample(),&lineHeight,&xAdvance,&yMinimum,&yMaximum);
				if(lineHeight >_lineHeight){
					
					_lineHeight = lineHeight;
					_yMaximum   = yMaximum;
					_yMinimum   = yMinimum;
					
				}
			}
			
			//
			// But we want to check each and every label for the Indic or Arabic
			// cases:
			//
			
			
			//
			// Finally, the really special layout cases are various Indic plus Arabic:
			// 
			if( sampleLabels[i].isIndic() || sampleLabels[i].isArabic() ){
				
				_hasIndicOrArabic = true;
				script = sampleLabels[i].getScriptCode();
				found = _unicodeScripts.find(script);
				if(found != _unicodeScripts.end() ){
					sampleString = found->second.getSample();
					_pLASiWrapper->getDimensions(sampleString,&lineHeight,&xAdvance,&yMinimum,&yMaximum);
					if(lineHeight >_lineHeight){
						
						_lineHeight = lineHeight;
						_yMaximum   = yMaximum;
						_yMinimum   = yMinimum;
						
					}
					
				}
			}
		}
	}
	//
	// Set DrawingMetrics line height: These are dimensions in points
	//
	//DrawingMetrics::setLineMetrics( _lineHeight, _yMaximum, _yMinimum , _labelCount );
	DrawingMetrics::setLineMetrics(_lineHeight,_yMaximum,_yMinimum,labelSet->getNumberOfLabels());
}


//
// fitStringToLabelWidth()
//
std::string DrawingLabelManager::fitStringToLabelWidth(UTF8String label){
	
	double xAdvance;
	//
	// This retrieves the scaled width:
	//
	double maximumLabelWidth = DrawingMetrics::getIconInterval() - 
	                          2 * DrawingMetrics::getLabelMargin();
	//
	// So first divide by the scaling factor to get millimeters ...                          
	//
	maximumLabelWidth/=DrawingMetrics::getScalingFactor();
	//
	// ... and now divide to get points used by LASi:
	//
	maximumLabelWidth/=DrawingMetrics::getPointsToMmConversionFactor();
	
	//
	// Get label width (xAdvance):
	//
	_pLASiWrapper->getDimensions(label,&_lineHeight,&xAdvance,&_yMinimum,&_yMaximum);
	
	// NOTE: If the program is compiled with LASi, xAdvance just uses an average width per character which might
	// not work with all scripts and font sizes. The average value will work for Latin and extended Latin scripts.
	
	if(xAdvance > maximumLabelWidth ){
		
		unsigned totalCharacters = label.unicodeValueCount();
		unsigned allowedCharacters = (unsigned) (totalCharacters/xAdvance * maximumLabelWidth);
		if(allowedCharacters>3) allowedCharacters-=3;
		
		label = label.unicodeSubString(1,allowedCharacters)+UTF8String("...");
		
	}
	
	return label;
	
}

//
// Constructor:
//
DrawingLabelManagerLoader::DrawingLabelManagerLoader(){
	
	//std::cout << ">>>>>> NOW IN DRAWINGLABELMANAGERLOADER CONSTRUCTOR ..." << std::endl;
	
	// Basic ASCII Latin:
	DrawingLabelManager::_latinScript       = UnicodeScript(LATIN,"Latin","Latina","AQWXfqpyz");
	// Unicode Aggregate Block :
	DrawingLabelManager::_aggregatedScripts = UnicodeScript(AGGREGATE,"Aggregate","Aggregate","ẲẴẪẤẦẨẮǠǺỖỔỒỐǗǙǛǮǯǈƢƪƺfpqyzŊŋƢƣɲʃẨῨῘ");
	
	// Arabic: ar
	DrawingLabelManager::_unicodeScripts[ARABIC]     = UnicodeScript(ARABIC,"Arabic","عربي","وَإِنَّهُ لَحَسْرَةٌ عَلَى الْكَافِرِينَ   وَإِنَّهُ لَحَقُّ الْيَقِينِ   فَسَبِّحْ بِاسْمِ رَبِّكَ الْعَظِيمِ");
	// Devanagari: dv
	DrawingLabelManager::_unicodeScripts[DEVANAGARI] = UnicodeScript(DEVANAGARI,"Devanagari","देवनागरी"," सभी मनुष्यों को गौरव और अधिकारों के मामले में जन्मजात स्वतन्त्रता प्राप्त है। उन्हें बुद्धि और अन्तरात्मा की देन प्राप्त है और परस्पर उन्हें भाईचारे के भाव से बर्ताव करना चाहिये।");
	// Bengali: bn
	DrawingLabelManager::_unicodeScripts[BENGALI]    = UnicodeScript(BENGALI,"Bengali","বাংলা লিপি","জনগণমন-অধিনায়ক জয় হে ভারতভাগ্যবিধাতা বিন্ধ্য হিমাচল যমুনা গঙ্গা উচ্ছলজলধিতরঙ্গ তব শুভ নামে জাগে, তব শুভ আশিস মাগে");
	// Gurmukhi: pa
	DrawingLabelManager::_unicodeScripts[GURMUKHI]   = UnicodeScript(GURMUKHI,"Gurmukhi","ਗੁਰਮੁਖੀ","");
	// Gujarati: gu
	DrawingLabelManager::_unicodeScripts[GUJARATI]   = UnicodeScript(GUJARATI,"Gujarati","ગુજરાતી","અસતોમા સદગમય, તમસોમા જ્યોતીર્ ગમય, ર્મુત્યોર્મા અમૃતમ્ ગમય");
	// Oriya: or
	DrawingLabelManager::_unicodeScripts[ORIYA]      = UnicodeScript(ORIYA,"Oriya","ଓଡ଼ିଆ","ଉଇକିପିଡୟ଼ାଃ ପ୍ରାଯ଼ଶଃ ଜିଜ୍ଞାସିତ ପ୍ରଶ୍ନସମୂହ");
	// Tamil: ta
	DrawingLabelManager::_unicodeScripts[TAMIL]      = UnicodeScript(TAMIL,"Tamil","தமிழ்","கட்டற்ற கலைக்களஞ்சியமான விக்கிபீடியாவில் இருந்து.");
	// Telugu: te
	DrawingLabelManager::_unicodeScripts[TELUGU]     = UnicodeScript(TELUGU,"Telugu","తెలుగు","చందమామ సుప్రసిద్ధ పిల్లల మాసపత్రిక. పేరుకు పిల్లల పత్రికే అయినా పెద్దలు కూడా ఇష్టంగా చదివే పత్రిక. భారతీయుల్లో చదవడం వచ్చిన ప్రతి ఒక్కరూ చందమామ ఎప్పుడో ఒకప్పుడు చదివే ఉంటారనడం అతిశయోక్తి కాదు.");
	// Kannada: kn
	DrawingLabelManager::_unicodeScripts[KANNADA]    = UnicodeScript(KANNADA,"Kannada","ಕನ್ನಡ","ಜಲಿಯನ್‍ವಾಲಾ ಬಾಗ್ ಹತ್ಯಾಕಾಂಡ (ಅಥವಾ ಅಮೃತಸರ ಹತ್ಯಾಹಾಂಡ) - ಅಮೃತಸರದಲ್ಲಿರುವ ಜಲಿಯನ್‍ವಾಲಾ ಬಾಗ್ ಉದ್ಯಾನದಲ್ಲಿ ಏಪ್ರಿಲ್ ೧೩, ೧೯೧೯ರಂದು ಬ್ರಿಟೀಷ್ ಭಾರತ ಸೇನೆಯಿಂದ ಅಲ್ಲಿ ನೆರೆದಿದ್ದ ಜನರ ಮೇಲೆ ನಡೆಸಿದ ಗುಂಡಿನ ದಾಳಿಯ ಪರಿಣಾಮವಾಗಿ ನಡೆದ ಮಾರಣಹೋಮ.");
	// Malayalam: ml
	DrawingLabelManager::_unicodeScripts[MALAYALAM]  = UnicodeScript(MALAYALAM,"Malayalam","മലയാളം","രണ്ടാം ലോകമഹായുദ്ധത്തോട് കൂടി ബ്രിട്ടനിലുണ്ടായ രാഷ്ട്രീയ അസ്ഥിരത പരമാവധി മുതലെടുത്ത് ഇന്ത്യ സ്വാതന്ത്ര്യം നേടിയെടുക്കണം എന്നായിരുന്നു ബോസിന്റെ അഭിപ്രായം");
	// Sinhala: si
	DrawingLabelManager::_unicodeScripts[SINHALA]    = UnicodeScript(SINHALA,"Sinhala","සිංහල","ප්‍රධාන ඡන කොට්ටාශය වන සිංහල ඡනයාගේ මවු බස සිංහල වෙයි. සිංහල‍ ඉන්දු-යුරෝපීය භාෂා වල උප ගනයක් වන ඉන්දු-ආර්ය භාෂා ගනයට අයිති වන අතර මාල දිවයින භාවිතා කරන දිවෙලි භාෂාව භාෂාවට විශාල සමානතාවයක");
	// Thai: th
	DrawingLabelManager::_unicodeScripts[THAI]       = UnicodeScript(THAI,"Thai","ไทย","มื่อชั่วพ่อขุนรามคำแหง เมืองสุโขทัยนี้ดี ในน้ำมีปลา ในนามีข้าว");
	// Lao: lo
	DrawingLabelManager::_unicodeScripts[LAO]        = UnicodeScript(LAO,"Lao","ພາສາລາວ","ໃຫ້ເຈົ້າເອົາຄວາມຮູ້");
	// Tibetan: bo 
	DrawingLabelManager::_unicodeScripts[TIBETAN]    = UnicodeScript(TIBETAN,"Tibetan","བོད་སྐད་","དཀར་མཛེས་བོད་རིགས་རང་སྐྱོང་ཁུལ་");
	// Myanmar: my
	DrawingLabelManager::_unicodeScripts[MYANMAR]    = UnicodeScript(MYANMAR,"Myanmar","မ္ရန္‌မာစာ","မ္ဝန္ စာမ္ယားဖ္ရင့္ရေးထုိးခဲ့ပာသည္");
	// Khmer: km
	DrawingLabelManager::_unicodeScripts[KHMER]      = UnicodeScript(KHMER,"Khmer","ភាសាខ្មែរ","ដើម្បីអាចមើលឃើញច្បាស់បំផុត អ្នកចាំបាច់ត្រូវតែ");
	
	//std::cout << ">>>>>> NOW LEAVING DRAWINGLABELMANAGERLOADER CONSTRUCTOR ..." << std::endl;
	
}

