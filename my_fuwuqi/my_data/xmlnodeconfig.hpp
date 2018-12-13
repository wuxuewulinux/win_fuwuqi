
#ifndef XMLNODECONFIG_H_
#define XMLNODECONFIG_H_

#include<iostream>
#include<string>
#include<sstream>
#include "tinystr.hpp"  
#include "tinyxml.hpp"

template<class T>
bool StringToBaseType(const std::string & str,T & val);

template<class T>
bool GetNodeValue(TiXmlElement *element,T & val);

template<class T>
bool GetSubNodeValue(TiXmlElement *element,const std::string & name,T & val);





template<class T>
bool StringToBaseType(const std::string & str,T & val)
{
	std::istringstream stream;
	stream.str(str);
	if(stream>>val)
	{
		return true;
	}

	return false;
}



template<class T>
bool GetNodeValue(TiXmlElement *element,T & val)
{
	if(!element || !element->FirstChild() || !StringToBaseType(element->GetText(),val))
	{
		return false;
	}
	
	return true;
}



template<class T>
bool GetSubNodeValue(TiXmlElement *element,const std::string & name,T & val)
{
	if (!element)
	{
		return false;
	}
	TiXmlElement * TmpElement = element->FirstChildElement(name.c_str());
	return GetNodeValue(TmpElement,val);
}


#endif