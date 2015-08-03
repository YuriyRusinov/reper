/**********************************************************************
 * $Id: cpl_minixml.cpp,v 1.2 2007-12-11 07:57:15 ksa Exp $
 *
 * Project:  XGIS - Common Portability Library
 * Purpose:  Implementation of MiniXML Parser and handling.
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
 **********************************************************************
 *
 * Independent Security Audit 2003/04/05 Andrey Kiselev:
 *   Completed audit of this module. Any documents may be parsed without
 *   buffer overflows and stack corruptions.
 * 
 * Security Audit 2003/03/28 warmerda:
 *   Completed security audit.  I believe that this module may be safely used 
 *   to parse, and serialize arbitrary documents provided by a potentially 
 *   hostile source.
 *
 */

#include "kks_minixml.h"
#include <ctype.h>
#include "strutils.h"

//XGIS_CVSID("$Id: cpl_minixml.cpp,v 1.2 2007-12-11 07:57:15 ksa Exp $");

typedef enum {
    TNone,
    TString, 
    TOpen, 
    TClose,
    TEqual,
    TToken,
    TSlashClose,
    TQuestionClose,
    TComment,
    TLiteral
} XMLTokenType;

typedef struct {
    const char *pszInput;
    int        nInputOffset;
    int        nInputLine;
    int        bInElement;
    XMLTokenType  eTokenType;
    char       *pszToken;
    size_t     nTokenMaxSize;
    size_t     nTokenSize;

    int        nStackMaxSize;
    int        nStackSize;
    XGISXMLNode **papsStack;

    XGISXMLNode *psFirstNode;
} ParseContext;


/************************************************************************/
/*                              ReadChar()                              */
/************************************************************************/

static char ReadChar( ParseContext *psContext )

{
    char        chReturn;

    chReturn = psContext->pszInput[psContext->nInputOffset++];

    if( chReturn == '\0' )
        psContext->nInputOffset--;
    else if( chReturn == 10 )
        psContext->nInputLine++;
    
    return chReturn;
}

/************************************************************************/
/*                             UnreadChar()                             */
/************************************************************************/

static void UnreadChar( ParseContext *psContext, char chToUnread )

{
    if( chToUnread == '\0' )
    {
        /* do nothing */
    }
    else
    {
        XGISAssert( chToUnread 
                   == psContext->pszInput[psContext->nInputOffset-1] );

        psContext->nInputOffset--;

        if( chToUnread == 10 )
            psContext->nInputLine--;
    }
}

/************************************************************************/
/*                             AddToToken()                             */
/************************************************************************/

static void AddToToken( ParseContext *psContext, char chNewChar )

{
    if( psContext->pszToken == NULL )
    {
        psContext->nTokenMaxSize = 10;
        psContext->pszToken = (char *) XGISMalloc(psContext->nTokenMaxSize);
    }
    else if( psContext->nTokenSize >= psContext->nTokenMaxSize - 2 )
    {
        psContext->nTokenMaxSize *= 2;
        psContext->pszToken = (char *) 
            XGISRealloc(psContext->pszToken,psContext->nTokenMaxSize);
    }

    psContext->pszToken[psContext->nTokenSize++] = chNewChar;
    psContext->pszToken[psContext->nTokenSize] = '\0';
}

/************************************************************************/
/*                             ReadToken()                              */
/************************************************************************/

static XMLTokenType ReadToken( ParseContext *psContext )

{
    char        chNext;

    psContext->nTokenSize = 0;
    psContext->pszToken[0] = '\0';
    
    chNext = ReadChar( psContext );
    while( isspace(chNext) )
        chNext = ReadChar( psContext );

/* -------------------------------------------------------------------- */
/*      Handle comments.                                                */
/* -------------------------------------------------------------------- */
    if( chNext == '<' 
        && EQUAL_SN(psContext->pszInput+psContext->nInputOffset,"!--",3) )
    {
        psContext->eTokenType = TComment;

        // Skip "!--" characters
        ReadChar(psContext);
        ReadChar(psContext);
        ReadChar(psContext);

        while( !EQUAL_SN(psContext->pszInput+psContext->nInputOffset,"-->",3)
               && (chNext = ReadChar(psContext)) != '\0' )
            AddToToken( psContext, chNext );

        // Skip "-->" characters
        ReadChar(psContext);
        ReadChar(psContext);
        ReadChar(psContext);
    }
/* -------------------------------------------------------------------- */
/*      Handle DOCTYPE.                                                 */
/* -------------------------------------------------------------------- */
    else if( chNext == '<' 
          && EQUAL_SN(psContext->pszInput+psContext->nInputOffset,"!DOCTYPE",8) )
    {
        int   bInQuotes = false;
        psContext->eTokenType = TLiteral;
        
        AddToToken( psContext, '<' );
        do { 
            chNext = ReadChar(psContext);
            if( chNext == '\0' )
            {
                XGISError( XE_Failure, XGISE_AppDefined, 
                          "Parse error in DOCTYPE on or before line %d, "
                          "reached end of file without '>'.", 
                          psContext->nInputLine );
                
                break;
            }
            
            /* The markup declaration block within a DOCTYPE tag consists of:
             * - a left square bracket [
             * - a list of declarations
             * - a right square bracket ]
             * Example:
             * <!DOCTYPE RootElement [ ...declarations... ]>
             */
            if( chNext == '[' )
            {
                AddToToken( psContext, chNext );

                do
                {
                    chNext = ReadChar( psContext );
                    AddToToken( psContext, chNext );
                }
                while( chNext != ']'
                    && !EQUAL_SN(psContext->pszInput+psContext->nInputOffset,"]>", 2) );

                chNext = ReadChar( psContext );
                AddToToken( psContext, chNext );

                // Skip ">" character, will be consumed below
                chNext = ReadChar( psContext );
            }


            if( chNext == '\"' )
                bInQuotes = !bInQuotes;

             if( chNext == '>' && !bInQuotes )
            {
                AddToToken( psContext, '>' );
                break;
            }

            AddToToken( psContext, chNext );
        } while( true );
    }
/* -------------------------------------------------------------------- */
/*      Handle CDATA.                                                   */
/* -------------------------------------------------------------------- */
    else if( chNext == '<' 
          && EQUAL_SN(psContext->pszInput+psContext->nInputOffset,"![CDATA[",8) )
    {
        psContext->eTokenType = TString;

        // Skip !CDATA[
        ReadChar( psContext );
        ReadChar( psContext );
        ReadChar( psContext );
        ReadChar( psContext );
        ReadChar( psContext );
        ReadChar( psContext );
        ReadChar( psContext );
        ReadChar( psContext );

        while( !EQUAL_SN(psContext->pszInput+psContext->nInputOffset,"]]>",3)
               && (chNext = ReadChar(psContext)) != '\0' )
            AddToToken( psContext, chNext );

        // Skip "]]>" characters
        ReadChar(psContext);
        ReadChar(psContext);
        ReadChar(psContext);
    }
/* -------------------------------------------------------------------- */
/*      Simple single tokens of interest.                               */
/* -------------------------------------------------------------------- */
    else if( chNext == '<' && !psContext->bInElement )
    {
        psContext->eTokenType = TOpen;
        psContext->bInElement = true;
    }
    else if( chNext == '>' && psContext->bInElement )
    {
        psContext->eTokenType = TClose;
        psContext->bInElement = false;
    }
    else if( chNext == '=' && psContext->bInElement )
    {
        psContext->eTokenType = TEqual;
    }
    else if( chNext == '\0' )
    {
        psContext->eTokenType = TNone;
    }
/* -------------------------------------------------------------------- */
/*      Handle the /> token terminator.                                 */
/* -------------------------------------------------------------------- */
    else if( chNext == '/' && psContext->bInElement 
             && psContext->pszInput[psContext->nInputOffset] == '>' )
    {
        chNext = ReadChar( psContext );
        XGISAssert( chNext == '>' );

        psContext->eTokenType = TSlashClose;
        psContext->bInElement = false;
    }
/* -------------------------------------------------------------------- */
/*      Handle the ?> token terminator.                                 */
/* -------------------------------------------------------------------- */
    else if( chNext == '?' && psContext->bInElement 
             && psContext->pszInput[psContext->nInputOffset] == '>' )
    {
        chNext = ReadChar( psContext );
        
        XGISAssert( chNext == '>' );

        psContext->eTokenType = TQuestionClose;
        psContext->bInElement = false;
    }

/* -------------------------------------------------------------------- */
/*      Collect a quoted string.                                        */
/* -------------------------------------------------------------------- */
    else if( psContext->bInElement && chNext == '"' )
    {
        psContext->eTokenType = TString;

        while( (chNext = ReadChar(psContext)) != '"' 
               && chNext != '\0' )
            AddToToken( psContext, chNext );
        
        if( chNext != '"' )
        {
            psContext->eTokenType = TNone;
            XGISError( XE_Failure, XGISE_AppDefined, 
                  "Parse error on line %d, reached EOF before closing quote.", 
                      psContext->nInputLine );
        }

        /* Do we need to unescape it? */
        if( strchr(psContext->pszToken,'&') != NULL )
        {
            int  nLength;
            char *pszUnescaped = XGISUnescapeString( psContext->pszToken, 
                                                    &nLength, XGISES_XML );
            strcpy( psContext->pszToken, pszUnescaped );
            XGISFree( pszUnescaped );
            psContext->nTokenSize = strlen(psContext->pszToken );
        }
    }

    else if( psContext->bInElement && chNext == '\'' )
    {
        psContext->eTokenType = TString;

        while( (chNext = ReadChar(psContext)) != '\'' 
               && chNext != '\0' )
            AddToToken( psContext, chNext );
        
        if( chNext != '\'' )
        {
            psContext->eTokenType = TNone;
            XGISError( XE_Failure, XGISE_AppDefined, 
                  "Parse error on line %d, reached EOF before closing quote.", 
                      psContext->nInputLine );
        }

        /* Do we need to unescape it? */
        if( strchr(psContext->pszToken,'&') != NULL )
        {
            int  nLength;
            char *pszUnescaped = XGISUnescapeString( psContext->pszToken, 
                                                    &nLength, XGISES_XML );
            strcpy( psContext->pszToken, pszUnescaped );
            XGISFree( pszUnescaped );
            psContext->nTokenSize = strlen(psContext->pszToken );
        }
    }

/* -------------------------------------------------------------------- */
/*      Collect an unquoted string, terminated by a open angle          */
/*      bracket.                                                        */
/* -------------------------------------------------------------------- */
    else if( !psContext->bInElement )
    {
        psContext->eTokenType = TString;

        AddToToken( psContext, chNext );
        while( (chNext = ReadChar(psContext)) != '<' 
               && chNext != '\0' )
            AddToToken( psContext, chNext );
        UnreadChar( psContext, chNext );

        /* Do we need to unescape it? */
        if( strchr(psContext->pszToken,'&') != NULL )
        {
            int  nLength;
            char *pszUnescaped = XGISUnescapeString( psContext->pszToken, 
                                                    &nLength, XGISES_XML );
            strcpy( psContext->pszToken, pszUnescaped );
            XGISFree( pszUnescaped );
            psContext->nTokenSize = strlen(psContext->pszToken );
        }
    }
    
/* -------------------------------------------------------------------- */
/*      Collect a regular token terminated by white space, or           */
/*      special character(s) like an equal sign.                        */
/* -------------------------------------------------------------------- */
    else
    {
        psContext->eTokenType = TToken;

        /* add the first character to the token regardless of what it is */
        AddToToken( psContext, chNext );

        for( chNext = ReadChar(psContext); 
             (chNext >= 'A' && chNext <= 'Z')
                 || (chNext >= 'a' && chNext <= 'z')
                 || chNext == '-'
                 || chNext == '_'
                 || chNext == '.'
                 || chNext == ':'
                 || (chNext >= '0' && chNext <= '9');
             chNext = ReadChar(psContext) ) 
        {
            AddToToken( psContext, chNext );
        }

        UnreadChar(psContext, chNext);
    }
    
    return psContext->eTokenType;
}
    
/************************************************************************/
/*                              PushNode()                              */
/************************************************************************/

static void PushNode( ParseContext *psContext, XGISXMLNode *psNode )

{
    if( psContext->nStackMaxSize <= psContext->nStackSize )
    {
        psContext->nStackMaxSize += 10;
        psContext->papsStack = (XGISXMLNode **)
            XGISRealloc(psContext->papsStack, 
                       sizeof(XGISXMLNode*) * psContext->nStackMaxSize);
    }

    psContext->papsStack[psContext->nStackSize++] = psNode;
}
    
/************************************************************************/
/*                             AttachNode()                             */
/*                                                                      */
/*      Attach the passed node as a child of the current node.          */
/*      Special handling exists for adding siblings to psFirst if       */
/*      there is nothing on the stack.                                  */
/************************************************************************/

static void AttachNode( ParseContext *psContext, XGISXMLNode *psNode )

{
    if( psContext->psFirstNode == NULL )
        psContext->psFirstNode = psNode;
    else if( psContext->nStackSize == 0 )
    {
        XGISXMLNode *psSibling;

        psSibling = psContext->psFirstNode;
        while( psSibling->psNext != NULL )
            psSibling = psSibling->psNext;
        psSibling->psNext = psNode;
    }
    else if( psContext->papsStack[psContext->nStackSize-1]->psChild == NULL )
    {
        psContext->papsStack[psContext->nStackSize-1]->psChild = psNode;
    }
    else
    {
        XGISXMLNode *psSibling;

        psSibling = psContext->papsStack[psContext->nStackSize-1]->psChild;
        while( psSibling->psNext != NULL )
            psSibling = psSibling->psNext;
        psSibling->psNext = psNode;
    }
}

/************************************************************************/
/*                         XGISParseXMLString()                          */
/************************************************************************/

/**
 * \brief Parse an XML string into tree form.
 *
 * The passed document is parsed into a XGISXMLNode tree representation. 
 * If the document is not well formed XML then NULL is returned, and errors
 * are reported via XGISError().  No validation beyond wellformedness is
 * done.  The XGISParseXMLFile() convenience function can be used to parse
 * from a file. 
 *
 * The returned document tree is is owned by the caller and should be freed
 * with XGISDestroyXMLNode() when no longer needed.
 *
 * If the document has more than one "root level" element then those after the 
 * first will be attached to the first as siblings (via the psNext pointers)
 * even though there is no common parent.  A document with no XML structure
 * (no angle brackets for instance) would be considered well formed, and 
 * returned as a single XXT_Text node.  
 * 
 * @param pszString the document to parse. 
 *
 * @return parsed tree or NULL on error. 
 */

XGISXMLNode *XGISParseXMLString( const char *pszString )

{
    ParseContext sContext;

    //XGISErrorReset();

    if( pszString == NULL )
    {
        XGISError( XE_Failure, XGISE_AppDefined, 
                  "XGISParseXMLString() called with NULL pointer." );
        return NULL;
    }

/* -------------------------------------------------------------------- */
/*      Initialize parse context.                                       */
/* -------------------------------------------------------------------- */
    sContext.pszInput = pszString;
    sContext.nInputOffset = 0;
    sContext.nInputLine = 0;
    sContext.bInElement = false;
    sContext.pszToken = NULL;
    sContext.nTokenMaxSize = 0;
    sContext.nTokenSize = 0;
    sContext.eTokenType = TNone;
    sContext.nStackMaxSize = 0;
    sContext.nStackSize = 0;
    sContext.papsStack = NULL;
    sContext.psFirstNode = NULL;

    /* ensure token is initialized */
    AddToToken( &sContext, ' ' );
    
/* ==================================================================== */
/*      Loop reading tokens.                                            */
/* ==================================================================== */
    int errorFound = false;
    while( ReadToken( &sContext ) != TNone )
    {
/* -------------------------------------------------------------------- */
/*      Create a new element.                                           */
/* -------------------------------------------------------------------- */
        if( sContext.eTokenType == TOpen )
        {
            XGISXMLNode *psElement;

            if( ReadToken(&sContext) != TToken )
            {
                XGISError( XE_Failure, XGISE_AppDefined, 
                          "Line %d: Didn't find element token after open angle bracket.",
                          sContext.nInputLine );
                errorFound = true;
                break;
            }

            if( sContext.pszToken[0] != '/' )
            {
                psElement = XGISCreateXMLNode( NULL, XXT_Element,
                                              sContext.pszToken );
                AttachNode( &sContext, psElement );
                PushNode( &sContext, psElement );
            }
            else 
            {
                if( sContext.nStackSize == 0
                    || !EQUAL_S(sContext.pszToken+1,
                         sContext.papsStack[sContext.nStackSize-1]->pszValue) )
                {
                    XGISError( XE_Failure, XGISE_AppDefined, 
                              "Line %d: <%.500s> doesn't have matching <%.500s>.",
                              sContext.nInputLine,
                              sContext.pszToken, sContext.pszToken+1 );
                    errorFound = true;
                    break;
                }
                else
                {
                    if( ReadToken(&sContext) != TClose )
                    {
                        XGISError( XE_Failure, XGISE_AppDefined, 
                                  "Line %d: Missing close angle bracket after <%.500s.",
                                  sContext.nInputLine,
                                  sContext.pszToken );
                        errorFound = true;
                        break;
                    }

                    /* pop element off stack */
                    sContext.nStackSize--;
                }
            }
        }

/* -------------------------------------------------------------------- */
/*      Add an attribute to a token.                                    */
/* -------------------------------------------------------------------- */
        else if( sContext.eTokenType == TToken )
        {
            XGISXMLNode *psAttr;

            psAttr = XGISCreateXMLNode(NULL, XXT_Attribute, sContext.pszToken);
            AttachNode( &sContext, psAttr );
            
            if( ReadToken(&sContext) != TEqual )
            {
                XGISError( XE_Failure, XGISE_AppDefined, 
                          "Line %d: Didn't find expected '=' for value of attribute '%.500s'.",
                          sContext.nInputLine, psAttr->pszValue );
                errorFound = true;
                break;
            }

            if( ReadToken(&sContext) != TString 
                && sContext.eTokenType != TToken )
            {
                XGISError( XE_Failure, XGISE_AppDefined, 
                          "Line %d: Didn't find expected attribute value.",
                          sContext.nInputLine );
                errorFound = true;
                break;
            }

            XGISCreateXMLNode( psAttr, XXT_Text, sContext.pszToken );
        }

/* -------------------------------------------------------------------- */
/*      Close the start section of an element.                          */
/* -------------------------------------------------------------------- */
        else if( sContext.eTokenType == TClose )
        {
            if( sContext.nStackSize == 0 )
            {
                XGISError( XE_Failure, XGISE_AppDefined, 
                          "Line %d: Found unbalanced '>'.",
                          sContext.nInputLine );
                errorFound = true;
                break;
            }
        }

/* -------------------------------------------------------------------- */
/*      Close the start section of an element, and pop it               */
/*      immediately.                                                    */
/* -------------------------------------------------------------------- */
        else if( sContext.eTokenType == TSlashClose )
        {
            if( sContext.nStackSize == 0 )
            {
                XGISError( XE_Failure, XGISE_AppDefined, 
                          "Line %d: Found unbalanced '/>'.",
                          sContext.nInputLine );
                errorFound = true;
                break;
            }

            sContext.nStackSize--;
        }

/* -------------------------------------------------------------------- */
/*      Close the start section of a <?...?> element, and pop it        */
/*      immediately.                                                    */
/* -------------------------------------------------------------------- */
        else if( sContext.eTokenType == TQuestionClose )
        {
            if( sContext.nStackSize == 0 )
            {
                XGISError( XE_Failure, XGISE_AppDefined, 
                          "Line %d: Found unbalanced '?>'.",
                          sContext.nInputLine );
                errorFound = true;
                break;
            }
            else if( sContext.papsStack[sContext.nStackSize-1]->pszValue[0] != '?' )
            {
                XGISError( XE_Failure, XGISE_AppDefined, 
                          "Line %d: Found '?>' without matching '<?'.",
                          sContext.nInputLine );
                errorFound = true;
                break;
            }

            sContext.nStackSize--;
        }

/* -------------------------------------------------------------------- */
/*      Handle comments.  They are returned as a whole token with the     */
/*      prefix and postfix omitted.  No processing of white space       */
/*      will be done.                                                   */
/* -------------------------------------------------------------------- */
        else if( sContext.eTokenType == TComment )
        {
            XGISXMLNode *psValue;

            psValue = XGISCreateXMLNode(NULL, XXT_Comment, sContext.pszToken);
            AttachNode( &sContext, psValue );
        }

/* -------------------------------------------------------------------- */
/*      Handle literals.  They are returned without processing.         */
/* -------------------------------------------------------------------- */
        else if( sContext.eTokenType == TLiteral )
        {
            XGISXMLNode *psValue;

            psValue = XGISCreateXMLNode(NULL, XXT_Literal, sContext.pszToken);
            AttachNode( &sContext, psValue );
        }

/* -------------------------------------------------------------------- */
/*      Add a text value node as a child of the current element.        */
/* -------------------------------------------------------------------- */
        else if( sContext.eTokenType == TString && !sContext.bInElement )
        {
            XGISXMLNode *psValue;

            psValue = XGISCreateXMLNode(NULL, XXT_Text, sContext.pszToken);
            AttachNode( &sContext, psValue );
        }
/* -------------------------------------------------------------------- */
/*      Anything else is an error.                                      */
/* -------------------------------------------------------------------- */
        else
        {
            XGISError( XE_Failure, XGISE_AppDefined, 
                      "Parse error at line %d, unexpected token:%.500s\n", 
                      sContext.nInputLine, sContext.pszToken );
            errorFound = true;
            break;
        }
    }

/* -------------------------------------------------------------------- */
/*      Did we pop all the way out of our stack?                        */
/* -------------------------------------------------------------------- */
    //if( errorFound == true && sContext.nStackSize != 0 )
    if( errorFound && sContext.nStackSize != 0 )
    {
        XGISError( XE_Failure, XGISE_AppDefined, 
                  "Parse error at EOF, not all elements have been closed,\n"
                  "starting with %.500s\n", 
                  sContext.papsStack[sContext.nStackSize-1]->pszValue );
    }

/* -------------------------------------------------------------------- */
/*      Cleanup                                                         */
/* -------------------------------------------------------------------- */
    XGISFree( sContext.pszToken );
    if( sContext.papsStack != NULL )
        XGISFree( sContext.papsStack );

    if( errorFound != false )
    {
        XGISDestroyXMLNode( sContext.psFirstNode );
        sContext.psFirstNode = NULL;
    }

    return sContext.psFirstNode;
}

/************************************************************************/
/*                            _GrowBuffer()                             */
/************************************************************************/

static void _GrowBuffer( size_t nNeeded, 
                         char **ppszText, unsigned int *pnMaxLength )

{
    if( nNeeded+1 >= *pnMaxLength )
    {
        *pnMaxLength = MAX(*pnMaxLength * 2,nNeeded+1);
        *ppszText = (char *) XGISRealloc(*ppszText, *pnMaxLength);
    }
}

/************************************************************************/
/*                        XGISSerializeXMLNode()                         */
/************************************************************************/

static void
XGISSerializeXMLNode( XGISXMLNode *psNode, int nIndent, 
                     char **ppszText, unsigned int *pnLength, 
                     unsigned int *pnMaxLength )

{
    if( psNode == NULL )
        return;
    
/* -------------------------------------------------------------------- */
/*      Ensure the buffer is plenty large to hold this additional       */
/*      string.                                                         */
/* -------------------------------------------------------------------- */
    *pnLength += strlen(*ppszText + *pnLength);
    _GrowBuffer( strlen(psNode->pszValue) + *pnLength + 40 + nIndent, 
                 ppszText, pnMaxLength );
    
/* -------------------------------------------------------------------- */
/*      Text is just directly emitted.                                  */
/* -------------------------------------------------------------------- */
    if( psNode->eType == XXT_Text )
    {
        char *pszEscaped = XGISEscapeString( psNode->pszValue, -1, XGISES_XML );

        XGISAssert( psNode->psChild == NULL );

        /* Escaped text might be bigger than expected. */
        _GrowBuffer( strlen(pszEscaped) + *pnLength,
                     ppszText, pnMaxLength );
        strcat( *ppszText + *pnLength, pszEscaped );

        XGISFree( pszEscaped );
    }

/* -------------------------------------------------------------------- */
/*      Attributes require a little formatting.                         */
/* -------------------------------------------------------------------- */
    else if( psNode->eType == XXT_Attribute )
    {
        XGISAssert( psNode->psChild != NULL 
                   && psNode->psChild->eType == XXT_Text );

        sprintf( *ppszText + *pnLength, " %s=\"", psNode->pszValue );
        XGISSerializeXMLNode( psNode->psChild, 0, ppszText, 
                             pnLength, pnMaxLength );
        strcat( *ppszText + *pnLength, "\"" );
    }

/* -------------------------------------------------------------------- */
/*      Handle comment output.                                          */
/* -------------------------------------------------------------------- */
    else if( psNode->eType == XXT_Comment )
    {
        int     i;

        XGISAssert( psNode->psChild == NULL );

        for( i = 0; i < nIndent; i++ )
            (*ppszText)[(*pnLength)++] = ' ';

        sprintf( *ppszText + *pnLength, "<!--%s-->\n", 
                 psNode->pszValue );
    }

/* -------------------------------------------------------------------- */
/*      Handle literal output (like <!DOCTYPE...>)                      */
/* -------------------------------------------------------------------- */
    else if( psNode->eType == XXT_Literal )
    {
        int     i;

        XGISAssert( psNode->psChild == NULL );

        for( i = 0; i < nIndent; i++ )
            (*ppszText)[(*pnLength)++] = ' ';

        strcpy( *ppszText + *pnLength, psNode->pszValue );
        strcat( *ppszText + *pnLength, "\n" );
    }

/* -------------------------------------------------------------------- */
/*      Elements actually have to deal with general children, and       */
/*      various formatting issues.                                      */
/* -------------------------------------------------------------------- */
    else if( psNode->eType == XXT_Element )
    {
        int             bHasNonAttributeChildren = false;
        XGISXMLNode      *psChild;
        
        memset( *ppszText + *pnLength, ' ', nIndent );
        *pnLength += nIndent;
        (*ppszText)[*pnLength] = '\0';

        sprintf( *ppszText + *pnLength, "<%s", psNode->pszValue );

        /* Serialize *all* the attribute children, regardless of order */
        for( psChild = psNode->psChild; 
             psChild != NULL; 
             psChild = psChild->psNext )
        {
            if( psChild->eType == XXT_Attribute )
                XGISSerializeXMLNode( psChild, 0, ppszText, pnLength, 
                                     pnMaxLength );
            else
                bHasNonAttributeChildren = true;
        }
        
        if( !bHasNonAttributeChildren )
        {
            if( psNode->pszValue[0] == '?' )
                strcat( *ppszText + *pnLength, "?>\n" );
            else
                strcat( *ppszText + *pnLength, "/>\n" );
        }
        else
        {
            int         bJustText = true;

            strcat( *ppszText + *pnLength, ">" );

            for( psChild = psNode->psChild; 
                 psChild != NULL; 
                 psChild = psChild->psNext )
            {
                if( psChild->eType == XXT_Attribute )
                    continue;

                if( psChild->eType != XXT_Text && bJustText )
                {
                    bJustText = false;
                    strcat( *ppszText + *pnLength, "\n" );
                }

                XGISSerializeXMLNode( psChild, nIndent + 2, ppszText, pnLength, 
                                     pnMaxLength );
            }
        
            *pnLength += strlen(*ppszText + *pnLength);
            _GrowBuffer( strlen(psNode->pszValue) + *pnLength + 40 + nIndent, 
                         ppszText, pnMaxLength );

            if( !bJustText )
            {
                memset( *ppszText + *pnLength, ' ', nIndent );
                *pnLength += nIndent;
                (*ppszText)[*pnLength] = '\0';
            }

            *pnLength += strlen(*ppszText + *pnLength);
            sprintf( *ppszText + *pnLength, "</%s>\n", psNode->pszValue );
        }
    }
}
                                
/************************************************************************/
/*                        XGISSerializeXMLTree()                         */
/************************************************************************/

/**
 * \brief Convert tree into string document.
 *
 * This function converts a XGISXMLNode tree representation of a document
 * into a flat string representation.  White space indentation is used
 * visually preserve the tree structure of the document.  The returned 
 * document becomes owned by the caller and should be freed with XGISFree()
 * when no longer needed.
 *
 * @param psNode
 *
 * @return the document on success or NULL on failure. 
 */

char *XGISSerializeXMLTree( XGISXMLNode *psNode )

{
    unsigned int nMaxLength = 100, nLength = 0;
    char *pszText = NULL;
    XGISXMLNode *psThis;

    pszText = (char *) XGISMalloc(nMaxLength);
    pszText[0] = '\0';

    for( psThis = psNode; psThis != NULL; psThis = psThis->psNext )
        XGISSerializeXMLNode( psThis, 0, &pszText, &nLength, &nMaxLength );

    return pszText;
}

/************************************************************************/
/*                          XGISCreateXMLNode()                          */
/************************************************************************/

/**
 * \brief Create an document tree item.
 *
 * Create a single XGISXMLNode object with the desired value and type, and
 * attach it as a child of the indicated parent.  
 *
 * @param poParent the parent to which this node should be attached as a 
 * child.  May be NULL to keep as free standing. 
 *
 * @return the newly created node, now owned by the caller (or parent node).
 */

XGISXMLNode *XGISCreateXMLNode( XGISXMLNode *poParent, XGISXMLNodeType eType, 
                              const char *pszText )

{
    XGISXMLNode  *psNode;

/* -------------------------------------------------------------------- */
/*      Create new node.                                                */
/* -------------------------------------------------------------------- */
    psNode = (XGISXMLNode *) XGISCalloc(sizeof(XGISXMLNode),1);
    
    psNode->eType = eType;
    psNode->pszValue = XGISStrdup( pszText );

/* -------------------------------------------------------------------- */
/*      Attach to parent, if provided.                                  */
/* -------------------------------------------------------------------- */
    if( poParent != NULL )
    {
        if( poParent->psChild == NULL )
            poParent->psChild = psNode;
        else
        {
            XGISXMLNode  *psLink = poParent->psChild;

            while( psLink->psNext != NULL )
                psLink = psLink->psNext;

            psLink->psNext = psNode;
        }
    }
    
    return psNode;
}

/************************************************************************/
/*                         XGISDestroyXMLNode()                          */
/************************************************************************/

/**
 * \brief Destroy a tree. 
 *
 * This function frees resources associated with a XGISXMLNode and all its
 * children nodes.  
 *
 * @param psNode the tree to free.
 */

void XGISDestroyXMLNode( XGISXMLNode *psNode )

{
    if( psNode == NULL )
        return;

    if( psNode->psChild != NULL )
        XGISDestroyXMLNode( psNode->psChild );
    
    if( psNode->psNext != NULL )
        XGISDestroyXMLNode( psNode->psNext );

    XGISFree( psNode->pszValue );
    XGISFree( psNode );
}

/************************************************************************/
/*                           XGISSearchXMLNode()                         */
/************************************************************************/

/**
 * \brief Search for a node in document.
 *
 * Searches the children (and potentially siblings) of the documented
 * passed in for the named element or attribute.  To search following
 * siblings as well as children, prefix the pszElement name with an equal
 * sign.  This function does an in-order traversal of the document tree.
 * So it will first match against the current node, then it's first child,
 * that childs first child, and so on. 
 *
 * Use XGISGetXMLNode() to find a specific child, or along a specific
 * node path. 
 *
 * @param psRoot the subtree to search.  This should be a node of type
 * XXT_Element.  NULL is safe. 
 *
 * @param pszElement the name of the element or attribute to search for.
 *
 * @return The matching node or NULL on failure. 
 */

XGISXMLNode *XGISSearchXMLNode( XGISXMLNode *psRoot, const char *pszElement )

{
    int         bSideSearch = false;
    XGISXMLNode *psChild, *psResult;

    if( psRoot == NULL || pszElement == NULL )
        return NULL;

    if( *pszElement == '=' )
    {
        bSideSearch = true;
        pszElement++;
    }

/* -------------------------------------------------------------------- */
/*      Does this node match?                                           */
/* -------------------------------------------------------------------- */
    if( (psRoot->eType == XXT_Element 
         || psRoot->eType == XXT_Attribute)
        && EQUAL_S(pszElement,psRoot->pszValue) )
        return psRoot;

/* -------------------------------------------------------------------- */
/*      Search children.                                                */
/* -------------------------------------------------------------------- */
    for( psChild = psRoot->psChild; psChild != NULL; psChild = psChild->psNext)
    {
        if( (psChild->eType == XXT_Element 
             || psChild->eType == XXT_Attribute)
            && EQUAL_S(pszElement,psChild->pszValue) )
            return psChild;

        if( psChild->psChild != NULL )
        {
            psResult = XGISSearchXMLNode( psChild, pszElement );
            if( psResult != NULL )
                return psResult;
        }
    }

/* -------------------------------------------------------------------- */
/*      Search siblings if we are in side search mode.                  */
/* -------------------------------------------------------------------- */
    if( bSideSearch )
    {
        for( psRoot = psRoot->psNext; psRoot != NULL; psRoot = psRoot->psNext )
        {
            psResult = XGISSearchXMLNode( psRoot, pszElement );
            if( psResult != NULL )
                return psResult;
        }
    }
    
    return NULL;
}

/************************************************************************/
/*                           XGISGetXMLNode()                            */
/************************************************************************/

/**
 * \brief Find node by path.
 *
 * Searches the document or subdocument indicated by psRoot for an element 
 * (or attribute) with the given path.  The path should consist of a set of
 * element names separated by dots, not including the name of the root 
 * element (psRoot).  If the requested element is not found NULL is returned.
 *
 * Attribute names may only appear as the last item in the path. 
 *
 * The search is done from the root nodes children, but all intermediate
 * nodes in the path must be specified.  Seaching for "name" would only find
 * a name element or attribute if it is a direct child of the root, not at any
 * level in the subdocument. 
 *
 * If the pszPath is prefixed by "=" then the search will begin with the
 * root node, and it's siblings, instead of the root nodes children.  This
 * is particularly useful when searching within a whole document which is
 * often prefixed by one or more "junk" nodes like the <?xml> declaration.
 *
 * @param psRoot the subtree in which to search.  This should be a node of 
 * type XXT_Element.  NULL is safe. 
 *
 * @param pszPath the list of element names in the path (dot separated). 
 *
 * @return the requested element node, or NULL if not found. 
 */

XGISXMLNode *XGISGetXMLNode( XGISXMLNode *psRoot, const char *pszPath )

{
    char        **papszTokens;
    int         iToken = 0;
    int         bSideSearch = false;

    if( psRoot == NULL )
        return NULL;

    if( *pszPath == '=' )
    {
        bSideSearch = true;
        pszPath++;
    }

    papszTokens = XGISTokenizeStringComplex( pszPath, ".", false, false );

    while( papszTokens[iToken] != NULL && psRoot != NULL )
    {
        XGISXMLNode *psChild;

        if( bSideSearch )
        {
            psChild = psRoot;
            bSideSearch = false;
        }
        else
            psChild = psRoot->psChild;

        for( ; psChild != NULL; psChild = psChild->psNext ) 
        {
            if( psChild->eType != XXT_Text 
                && EQUAL_S(papszTokens[iToken],psChild->pszValue) )
                break;
        }

        if( psChild == NULL )
        {
            psRoot = NULL;
            break;
        }

        psRoot = psChild;
        iToken++;
    }

    XGISDestroy( papszTokens );
    return psRoot;
}

/************************************************************************/
/*                           XGISGetXMLValue()                           */
/************************************************************************/

/**
 * \brief Fetch element/attribute value. 
 *
 * Searches the document for the element/attribute value associated with
 * the path.  The corresponding node is internally found with XGISGetXMLNode()
 * (see there for details on path handling).  Once found, the value is 
 * considered to be the first XXT_Text child of the node.
 *
 * If the attribute/element search fails, or if the found node has not
 * value then the passed default value is returned. 
 *
 * The returned value points to memory within the document tree, and should
 * not be altered or freed. 
 *
 * @param psRoot the subtree in which to search.  This should be a node of 
 * type XXT_Element.  NULL is safe. 
 *
 * @param pszPath the list of element names in the path (dot separated).  An
 * empty path means get the value of the psRoot node.
 *
 * @param pszDefault the value to return if a corresponding value is not
 * found, may be NULL.
 *
 * @return the requested value or pszDefault if not found.
 */

const char *XGISGetXMLValue( XGISXMLNode *psRoot, const char *pszPath, 
                            const char *pszDefault )

{
    XGISXMLNode  *psTarget;

    if( pszPath == NULL || *pszPath == '\0' )
        psTarget  = psRoot;
    else
        psTarget = XGISGetXMLNode( psRoot, pszPath );

    if( psTarget == NULL )
        return pszDefault;

    if( psTarget->eType == XXT_Attribute )
    {
        XGISAssert( psTarget->psChild != NULL 
                   && psTarget->psChild->eType == XXT_Text );

        return psTarget->psChild->pszValue;
    }

    if( psTarget->eType == XXT_Element )
    {
        // Find first non-attribute child, and verify it is a single text 
        // with no siblings

        psTarget = psTarget->psChild;

        while( psTarget != NULL && psTarget->eType == XXT_Attribute )
            psTarget = psTarget->psNext;

        if( psTarget != NULL 
            && psTarget->eType == XXT_Text
            && psTarget->psNext == NULL )
            return psTarget->pszValue;
    }

    return pszDefault;
}

/************************************************************************/
/*                           XGISAddXMLChild()                           */
/************************************************************************/

/**
 * \brief Add child node to parent. 
 *
 * The passed child is added to the list of children of the indicated
 * parent.  Normally the child is added at the end of the parents child
 * list, but attributes (XXT_Attribute) will be inserted after any other
 * attributes but before any other element type.  Ownership of the child
 * node is effectively assumed by the parent node.   If the child has
 * siblings (it's psNext is not NULL) they will be trimmed, but if the child
 * has children they are carried with it. 
 *
 * @param psParent the node to attach the child to.  May not be NULL.
 *
 * @param psChild the child to add to the parent.  May not be NULL.  Should 
 * not be a child of any other parent. 
 */

void XGISAddXMLChild( XGISXMLNode *psParent, XGISXMLNode *psChild )

{
    XGISXMLNode *psSib;

    if( psParent->psChild == NULL )
    {
        psParent->psChild = psChild;
        return;
    }

    // Insert at head of list if first child is not attribute.
    if( psChild->eType == XXT_Attribute 
        && psParent->psChild->eType != XXT_Attribute )
    {
        psChild->psNext = psParent->psChild;
        psParent->psChild = psChild;
        return;
    }

    // Search for end of list.
    for( psSib = psParent->psChild; 
         psSib->psNext != NULL; 
         psSib = psSib->psNext ) 
    {
        // Insert attributes if the next node is not an attribute.
        if( psChild->eType == XXT_Attribute 
            && psSib->psNext != NULL 
            && psSib->psNext->eType != XXT_Attribute )
        {
            psChild->psNext = psSib->psNext;
            psSib->psNext = psChild;
            return;
        }
    }

    psSib->psNext = psChild;
}

/************************************************************************/
/*                           XGISAddXMLChild()                           */
/************************************************************************/

/**
 * \brief Remove child node from parent. 
 *
 * The passed child is removed from the child list of the passed parent,
 * but the child is not destroyed.  The child retains ownership of it's
 * own children, but is cleanly removed from the child list of the parent.
 *
 * @param psParent the node to the child is attached to.
 *
 * @param psChild the child to remove.
 *
 * @return true on success or false if the child was not found.
 */

int XGISRemoveXMLChild( XGISXMLNode *psParent, XGISXMLNode *psChild )

{
    XGISXMLNode *psLast = NULL, *psThis;

    if( psParent == NULL )
        return false;

    for( psThis = psParent->psChild; 
         psThis != NULL; 
         psLast = psThis, psThis = psThis->psNext )
    {
        if( psThis == psChild )
        {
            if( psLast == NULL )
                psParent->psChild = psThis->psNext;
            else 
                psLast->psNext = psThis->psNext;

            psThis->psNext = NULL;
            return true;
        }
    }

    return false;
}

/************************************************************************/
/*                          XGISAddXMLSibling()                          */
/************************************************************************/

/**
 * \brief Add new sibling.
 *
 * The passed psNewSibling is added to the end of siblings of the 
 * psOlderSibling node.  That is, it is added to the end of the psNext
 * chain.  There is no special handling if psNewSibling is an attribute. 
 * If this is required, use XGISAddXMLChild(). 
 *
 * @param psOlderSibling the node to attach the sibling after.
 *
 * @param psNewSibling the node to add at the end of psOlderSiblings psNext 
 * chain.
 */

void XGISAddXMLSibling( XGISXMLNode *psOlderSibling, XGISXMLNode *psNewSibling )

{
    if( psOlderSibling == NULL )
        return;

    while( psOlderSibling->psNext != NULL )
        psOlderSibling = psOlderSibling->psNext;

    psOlderSibling->psNext = psNewSibling;
}

/************************************************************************/
/*                    XGISCreateXMLElementAndValue()                     */
/************************************************************************/

/**
 * \brief Create an element and text value.
 *
 * This is function is a convenient short form for:
 *
 * \code
 *     XGISXMLNode *psTextNode;
 *     XGISXMLNode *psElementNode;
 *
 *     psElementNode = XGISCreateXMLNode( psParent, XXT_Element, pszName );
 *     psTextNode = XGISCreateXMLNode( psElementNode, XXT_Text, pszValue );
 * 
 *     return psElementNode;
 * \endcode
 *
 * It creates a XXT_Element node, with a XXT_Text child, and
 * attaches the element to the passed parent.
 *
 * @param psParent the parent node to which the resulting node should
 * be attached.  May be NULL to keep as freestanding. 
 *
 * @param pszName the element name to create.
 * @param pszValue the text to attach to the element. Must not be NULL. 
 *
 * @return the pointer to the new element node.
 */

XGISXMLNode *XGISCreateXMLElementAndValue( XGISXMLNode *psParent, 
                                         const char *pszName, 
                                         const char *pszValue )

{
    XGISXMLNode *psElementNode;

    psElementNode = XGISCreateXMLNode( psParent, XXT_Element, pszName );
    XGISCreateXMLNode( psElementNode, XXT_Text, pszValue );

    return psElementNode;
}

/************************************************************************/
/*                          XGISCloneXMLTree()                           */
/************************************************************************/

/**
 * \brief Copy tree.
 *
 * Creates a deep copy of a XGISXMLNode tree.  
 *
 * @param psTree the tree to duplicate. 
 *
 * @return a copy of the whole tree. 
 */

XGISXMLNode *XGISCloneXMLTree( XGISXMLNode *psTree )

{
    XGISXMLNode *psPrevious = NULL;
    XGISXMLNode *psReturn = NULL;

    while( psTree != NULL )
    {
        XGISXMLNode *psCopy;

        psCopy = XGISCreateXMLNode( NULL, psTree->eType, psTree->pszValue );
        if( psReturn == NULL )
            psReturn = psCopy;
        if( psPrevious != NULL )
            psPrevious->psNext = psCopy;

        if( psTree->psChild != NULL )
            psCopy->psChild = XGISCloneXMLTree( psTree->psChild );

        psPrevious = psCopy;
        psTree = psTree->psNext;
    }

    return psReturn;
}

/************************************************************************/
/*                           XGISSetXMLValue()                           */
/************************************************************************/

/**
 * \brief Set element value by path. 
 *
 * Find (or create) the target element or attribute specified in the
 * path, and assign it the indicated value. 
 *
 * Any path elements that do not already exist will be created.  The target
 * nodes value (the first XXT_Text child) will be replaced with the provided
 * value.  
 *
 * If the target node is an attribute instead of an element, the last separator
 * should be a "#" instead of the normal period path separator. 
 *
 * Example:
 *   XGISSetXMLValue( "Citation.Id.Description", "DOQ dataset" );
 *   XGISSetXMLValue( "Citation.Id.Description#name", "doq" );
 *
 * @param psRoot the subdocument to be updated. 
 *
 * @param pszPath the dot seperated path to the target element/attribute.
 *
 * @param pszValue the text value to assign. 
 *
 * @return true on success.
 */

int XGISSetXMLValue( XGISXMLNode *psRoot,  const char *pszPath,
                    const char *pszValue )

{
    char        **papszTokens;
    int         iToken = 0;

    papszTokens = XGISTokenizeStringComplex( pszPath, ".", false, false );

    while( papszTokens[iToken] != NULL && psRoot != NULL )
    {
        XGISXMLNode *psChild;
        int        bIsAttribute = false;
        const char *pszName = papszTokens[iToken];

        if( pszName[0] == '#' )
        {
            bIsAttribute = true;
            pszName++;
        }

        if( psRoot->eType != XXT_Element )
            return false;

        for( psChild = psRoot->psChild; psChild != NULL; 
             psChild = psChild->psNext ) 
        {
            if( psChild->eType != XXT_Text 
                && EQUAL_S(pszName,psChild->pszValue) )
                break;
        }

        if( psChild == NULL )
        {
            if( bIsAttribute )
                psChild = XGISCreateXMLNode( psRoot, XXT_Attribute, pszName );
            else
                psChild = XGISCreateXMLNode( psRoot, XXT_Element, pszName );
        }

        psRoot = psChild;
        iToken++;
    }

    XGISDestroy( papszTokens );

/* -------------------------------------------------------------------- */
/*      Find the "text" child if there is one.                          */
/* -------------------------------------------------------------------- */
    XGISXMLNode *psTextChild = psRoot->psChild;

    while( psTextChild != NULL && psTextChild->eType != XXT_Text )
        psTextChild = psTextChild->psNext;

/* -------------------------------------------------------------------- */
/*      Now set a value node under this node.                           */
/* -------------------------------------------------------------------- */
    
    if( psTextChild == NULL )
        XGISCreateXMLNode( psRoot, XXT_Text, pszValue );
    else 
    {
        XGISFree( psTextChild->pszValue );
        psTextChild->pszValue = XGISStrdup( pszValue );
    }

    return true;
}

/************************************************************************/
/*                        XGISStripXMLNamespace()                        */
/************************************************************************/

/**
 * \brief Strip indicated namespaces. 
 *
 * The subdocument (psRoot) is recursively examined, and any elements
 * with the indicated namespace prefix will have the namespace prefix
 * stripped from the element names.  If the passed namespace is NULL, then
 * all namespace prefixes will be stripped. 
 *
 * Nodes other than elements should remain unaffected.  The changes are
 * made "in place", and should not alter any node locations, only the 
 * pszValue field of affected nodes. 
 *
 * @param psRoot the document to operate on.
 * @param pszNamespace the name space prefix (not including colon), or NULL.
 * @param bRecurse true to recurse over whole document, or false to only
 * operate on the passed node.
 */

void XGISStripXMLNamespace( XGISXMLNode *psRoot, 
                           const char *pszNamespace, 
                           int bRecurse )

{
    if( psRoot == NULL )
        return;

    if( psRoot->eType == XXT_Element || psRoot->eType == XXT_Attribute )
    {
        if( pszNamespace != NULL )
        {
            if( EQUAL_SN(pszNamespace,psRoot->pszValue,strlen(pszNamespace)) 
                && psRoot->pszValue[strlen(pszNamespace)] == ':' )
            {
                char *pszNewValue = 
                    XGISStrdup(psRoot->pszValue+strlen(pszNamespace)+1);
                
                XGISFree( psRoot->pszValue );
                psRoot->pszValue = pszNewValue;
            }
        }
        else
        {
            const char *pszCheck;
            
            for( pszCheck = psRoot->pszValue; *pszCheck != '\0'; pszCheck++ )
            {
                if( *pszCheck == ':' )
                {
                    char *pszNewValue = XGISStrdup( pszCheck+1 );
                    
                    XGISFree( psRoot->pszValue );
                    psRoot->pszValue = pszNewValue;
                    break;
                }
            }
        }
    }

    if( bRecurse )
    {
        if( psRoot->psChild != NULL )
            XGISStripXMLNamespace( psRoot->psChild, pszNamespace, 1 );
        if( psRoot->psNext != NULL )
            XGISStripXMLNamespace( psRoot->psNext, pszNamespace, 1 );
    }
}


/************************************************************************/
/*                       XGISCleanXMLElementName()                       */
/************************************************************************/

/**
 * \brief Make string into safe XML token.
 *
 * Modififies a string in place to try and make it into a legal
 * XML token that can be used as an element name.   This is accomplished
 * by changing any characters not legal in a token into an underscore. 
 * 
 * NOTE: This function should implement the rules in section 2.3 of 
 * http://www.w3.org/TR/xml11/ but it doesn't yet do that properly.  We
 * only do a rough approximation of that.
 *
 * @param pszTarget the string to be adjusted.  It is altered in place. 
 */

void XGISCleanXMLElementName( char *pszTarget )
{
    if( pszTarget == NULL )
        return;

    for( ; *pszTarget != '\0'; pszTarget++ )
    {
        if( (*((unsigned char *) pszTarget) & 0x80) || isalnum( *pszTarget )
            || *pszTarget == '_' || *pszTarget == '.' )
        {
            /* ok */
        }
        else
        {
            *pszTarget = '_';
        }
    }
}
