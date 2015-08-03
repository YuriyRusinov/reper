/**********************************************************************
 * $Id: cpl_minixml.h,v 1.1 2007-11-06 15:52:10 ksa Exp $
 *
 * Project:  CPL - Common Portability Library
 * Purpose:  Declarations for MiniXML Handler.
 * Author:   Frank Warmerdam, warmerdam@pobox.com
 *
 **********************************************************************
 * Copyright (c) 2001, Frank Warmerdam
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 ****************************************************************************/

#ifndef _XGIS_MINIXML_H_INCLUDED
#define _XGIS_MINIXML_H_INCLUDED

#include "configUtils.h"
#include "kks_port.h"

/**
 * \file xgis_minixml.h
 *
 * Definitions for CPL mini XML Parser/Serializer.
 */

XGIS_C_START

typedef enum 
{
    /*! Node is an element */           XXT_Element = 0,
    /*! Node is a raw text value */     XXT_Text = 1,
    /*! Node is attribute */            XXT_Attribute = 2,  
    /*! Node is an XML comment. */      XXT_Comment = 3,    
    /*! Node is a special literal */    XXT_Literal = 4     
} XGISXMLNodeType;

/**
 * Document node structure. 
 *
 * This C structure is used to hold a single text fragment representing a 
 * component of the document when parsed.   It should be allocated with the
 * appropriate CPL function, and freed with CPLDestroyXMLNode().  The structure
 * contents should not normally be altered by application code, but may be
 * freely examined by application code.
 *
 * Using the psChild and psNext pointers, a heirarchical tree structure
 * for a document can be represented as a tree of CPLXMLNode structures. 
 */

typedef struct _XGISXMLNode
{
    /** 
     * \brief Node type
     *
     * One of CXT_Element, CXT_Text, CXT_Attribute, CXT_Comment, 
     * or CXT_Literal.
     */
    XGISXMLNodeType      eType;       
    
    /** 
     * \brief Node value
     *
     * For CXT_Element this is the name of the element, without the angle
     * brackets.  Note there is a single CXT_Element even when the document
     * contains a start and end element tag.  The node represents the pair. 
     * All text or other elements between the start and end tag will appear
     * as children nodes of this CXT_Element node. 
     *
     * For CXT_Attribute the pszValue is the attribute name.  The value of 
     * the attribute will be a CXT_Text child. 
     *
     * For CXT_Text this is the text itself (value of an attribute, or a 
     * text fragment between an element start and end tags. 
     *
     * For CXT_Literal it is all the literal text.  Currently this is just
     * used for !DOCTYPE lines, and the value would be the entire line. 
     *
     * For CXT_Comment the value is all the literal text within the comment, 
     * but not including the comment start/end indicators ("<--" and "-->"). 
     */
    char                *pszValue;   

    /** 
     * \brief Next sibling. 
     *
     * Pointer to next sibling, that is the next node appearing after this
     * one that has the same parent as this node.  NULL if this node is the
     * last child of the parent element. 
     */
    struct _XGISXMLNode  *psNext;     

    /** 
     * \brief Child node.
     *
     * Pointer to first child node, if any.  Only CXT_Element and CXT_Attribute
     * nodes should have children.  For CXT_Attribute it should be a single
     * CXT_Text value node, while CXT_Attribute can have any kind of child. 
     * The full list of children for a node are identified by walking the 
     * psNext's starting with the psChild node. 
     */

    struct _XGISXMLNode  *psChild;    
} XGISXMLNode;


XGISXMLNode __EXPORT_UTILS *XGISParseXMLString( const char * );
void       __EXPORT_UTILS XGISDestroyXMLNode( XGISXMLNode * );
XGISXMLNode __EXPORT_UTILS *XGISGetXMLNode( XGISXMLNode *poRoot, 
                            const char *pszPath );
XGISXMLNode __EXPORT_UTILS *XGISSearchXMLNode( XGISXMLNode *poRoot, 
                               const char *pszTarget );
const char __EXPORT_UTILS *XGISGetXMLValue( XGISXMLNode *poRoot, 
                             const char *pszPath, 
                             const char *pszDefault );
XGISXMLNode __EXPORT_UTILS *XGISCreateXMLNode( XGISXMLNode *poParent, 
                               XGISXMLNodeType eType,
                               const char *pszText );
char       __EXPORT_UTILS *XGISSerializeXMLTree( XGISXMLNode *psNode );
void       __EXPORT_UTILS  XGISAddXMLChild( XGISXMLNode *psParent,
                             XGISXMLNode *psChild );
int        __EXPORT_UTILS  XGISRemoveXMLChild( XGISXMLNode *psParent,
                                XGISXMLNode *psChild );
void       __EXPORT_UTILS  XGISAddXMLSibling( XGISXMLNode *psOlderSibling,
                               XGISXMLNode *psNewSibling );
XGISXMLNode __EXPORT_UTILS *XGISCreateXMLElementAndValue( XGISXMLNode *psParent,
                                          const char *pszName,
                                          const char *pszValue );
XGISXMLNode __EXPORT_UTILS *XGISCloneXMLTree( XGISXMLNode *psTree );
int        __EXPORT_UTILS XGISSetXMLValue( XGISXMLNode *psRoot,  const char *pszPath,
                            const char *pszValue );
void       __EXPORT_UTILS XGISStripXMLNamespace( XGISXMLNode *psRoot, 
                                  const char *pszNameSpace, 
                                  int bRecurse );
void       __EXPORT_UTILS XGISCleanXMLElementName( char * );

XGISXMLNode __EXPORT_UTILS *XGISParseXMLFile( const char *pszFilename );
int        __EXPORT_UTILS XGISSerializeXMLTreeToFile( XGISXMLNode *psTree, 
                                              const char *pszFilename );
XGIS_C_END

#endif /* _XGIS_MINIXML_H_INCLUDED */
