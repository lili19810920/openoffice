/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/

// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_xmloff.hxx"
#include "unointerfacetouniqueidentifiermapper.hxx"
#include <tools/debug.hxx>
#ifndef _SVSTDARR_LONGS_DECL
#define _SVSTDARR_LONGS
#include <svl/svstdarr.hxx>
#endif
#include <svl/svarray.hxx>
#include <rtl/ustrbuf.hxx>
#include <sal/types.h>
#include <vector>
#include <list>
#include <hash_map>
#include <com/sun/star/lang/XServiceInfo.hpp>
#include <com/sun/star/container/XEnumerationAccess.hpp>
#include <com/sun/star/container/XEnumeration.hpp>
#include <com/sun/star/container/XIndexReplace.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/beans/XMultiPropertySet.hpp>
#include <com/sun/star/beans/XPropertyState.hpp>
#include <com/sun/star/text/XTextDocument.hpp>
#include <com/sun/star/text/XTextSectionsSupplier.hpp>
#include <com/sun/star/text/XTextTablesSupplier.hpp>
#include <com/sun/star/text/XNumberingRulesSupplier.hpp>
#include <com/sun/star/text/XChapterNumberingSupplier.hpp>//#outline level,add by zhaojianwei
#include <com/sun/star/text/XTextTable.hpp>
#include <com/sun/star/text/XText.hpp>
#include <com/sun/star/text/XTextContent.hpp>
#include <com/sun/star/text/XTextRange.hpp>
#include <com/sun/star/text/XTextField.hpp>
#include <com/sun/star/text/XFootnote.hpp>
#include <com/sun/star/container/XNamed.hpp>
#include <com/sun/star/container/XContentEnumerationAccess.hpp>
#include <com/sun/star/text/XTextFrame.hpp>
#include <com/sun/star/container/XNameAccess.hpp>
#include <com/sun/star/text/SizeType.hpp>
#include <com/sun/star/text/HoriOrientation.hpp>
#include <com/sun/star/text/VertOrientation.hpp>
#include <com/sun/star/text/TextContentAnchorType.hpp>
#include <com/sun/star/text/XTextFramesSupplier.hpp>
#include <com/sun/star/text/XTextGraphicObjectsSupplier.hpp>
#include <com/sun/star/text/XTextEmbeddedObjectsSupplier.hpp>
#include <com/sun/star/drawing/XDrawPageSupplier.hpp>
#include <com/sun/star/document/XEmbeddedObjectSupplier.hpp>
#include <com/sun/star/document/XEventsSupplier.hpp>
#include <com/sun/star/document/XRedlinesSupplier.hpp>
#include <com/sun/star/text/XBookmarksSupplier.hpp>
#include <com/sun/star/text/XFormField.hpp>
#include <com/sun/star/text/XTextSection.hpp>
#include <com/sun/star/text/SectionFileLink.hpp>
#include <com/sun/star/drawing/XShape.hpp>
#include <com/sun/star/text/XTextShapesSupplier.hpp>
#include <com/sun/star/style/XAutoStylesSupplier.hpp>
#include <com/sun/star/style/XAutoStyleFamily.hpp>
#include <com/sun/star/text/XTextFieldsSupplier.hpp>
#include <com/sun/star/text/XFootnotesSupplier.hpp>
#include <com/sun/star/text/XEndnotesSupplier.hpp>
#include <com/sun/star/drawing/XControlShape.hpp>
#include <com/sun/star/util/DateTime.hpp>
#include "xmloff/xmlnmspe.hxx"
#include <xmloff/xmlaustp.hxx>
#include <xmloff/families.hxx>
#include "txtexppr.hxx"
#include <xmloff/xmlnumfe.hxx>
#include <xmloff/xmlnume.hxx>
#include <xmloff/xmluconv.hxx>
#include "XMLAnchorTypePropHdl.hxx"
#include "xexptran.hxx"
#include <xmloff/ProgressBarHelper.hxx>
#include <xmloff/nmspmap.hxx>
#include <xmloff/xmlexp.hxx>
#include "txtflde.hxx"
#include <xmloff/txtprmap.hxx>
#include "XMLImageMapExport.hxx"
#include "XMLTextNumRuleInfo.hxx"
#include "xmloff/XMLTextListAutoStylePool.hxx"
#include <xmloff/txtparae.hxx>
#include "XMLSectionExport.hxx"
#include "XMLIndexMarkExport.hxx"
#include <xmloff/XMLEventExport.hxx>
#include "XMLRedlineExport.hxx"
#include "MultiPropertySetHelper.hxx"
#include <xmloff/formlayerexport.hxx>
#include "XMLTextCharStyleNamesElementExport.hxx"
#include <comphelper/stlunosequence.hxx>
#include <txtlists.hxx>
#include <com/sun/star/rdf/XMetadatable.hpp>
#include <basegfx/polygon/b2dpolypolygon.hxx>
#include <basegfx/polygon/b2dpolypolygontools.hxx>
#include <basegfx/polygon/b2dpolygontools.hxx>

using ::rtl::OUString;
using ::rtl::OUStringBuffer;

using namespace ::std;
using namespace ::com::sun::star;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::lang;
using namespace ::com::sun::star::beans;
using namespace ::com::sun::star::container;
using namespace ::com::sun::star::text;
using namespace ::com::sun::star::style;
using namespace ::com::sun::star::util;
using namespace ::com::sun::star::drawing;
using namespace ::com::sun::star::document;
using namespace ::com::sun::star::frame;
using namespace ::xmloff;
using namespace ::xmloff::token;

namespace
{
    class TextContentSet
    {
        public:
            typedef Reference<XTextContent> text_content_ref_t;
            typedef list<text_content_ref_t> contents_t;
            typedef back_insert_iterator<contents_t> inserter_t;
            typedef contents_t::const_iterator const_iterator_t;

            inserter_t getInserter()
                { return back_insert_iterator<contents_t>(m_vTextContents); };
            const_iterator_t getBegin() const
                { return m_vTextContents.begin(); };
            const_iterator_t getEnd() const
                { return m_vTextContents.end(); };

        private:
            contents_t m_vTextContents;
    };

    struct FrameRefHash
        : public unary_function<Reference<XTextFrame>, size_t>
    {
        size_t operator()(const Reference<XTextFrame> xFrame) const
            { return sal::static_int_cast<size_t>(reinterpret_cast<sal_uIntPtr>(xFrame.get())); }
    };

    static bool lcl_TextContentsUnfiltered(const Reference<XTextContent>&)
        { return true; };

    static bool lcl_ShapeFilter(const Reference<XTextContent>& xTxtContent)
    {
        static const OUString sTextFrameService = OUString::createFromAscii("com.sun.star.text.TextFrame");
        static const OUString sTextGraphicService = OUString::createFromAscii("com.sun.star.text.TextGraphicObject");
        static const OUString sTextEmbeddedService = OUString::createFromAscii("com.sun.star.text.TextEmbeddedObject");
        Reference<XShape> xShape(xTxtContent, UNO_QUERY);
        if(!xShape.is())
            return false;
        Reference<XServiceInfo> xServiceInfo(xTxtContent, UNO_QUERY);
        if(xServiceInfo->supportsService(sTextFrameService) ||
            xServiceInfo->supportsService(sTextGraphicService) ||
            xServiceInfo->supportsService(sTextEmbeddedService) )
            return false;
        return true;
    };

    class BoundFrames
    {
        public:
            typedef bool (*filter_t)(const Reference<XTextContent>&);
            BoundFrames(
                const Reference<XEnumerationAccess> xEnumAccess,
                const filter_t& rFilter)
                : m_xEnumAccess(xEnumAccess)
            {
                Fill(rFilter);
            };
            BoundFrames()
                {};
            const TextContentSet* GetPageBoundContents() const
                { return &m_vPageBounds; };
            const TextContentSet* GetFrameBoundContents(const Reference<XTextFrame>& rParentFrame) const
            {
                framebound_map_t::const_iterator it = m_vFrameBoundsOf.find(rParentFrame);
                if(it == m_vFrameBoundsOf.end())
                    return NULL;
                return &(it->second);
            };
            Reference<XEnumeration> createEnumeration() const
            {
                if(!m_xEnumAccess.is())
                    return Reference<XEnumeration>();
                return m_xEnumAccess->createEnumeration();
            };

        private:
            typedef hash_map<
                Reference<XTextFrame>,
                TextContentSet,
                FrameRefHash> framebound_map_t;
            TextContentSet m_vPageBounds;
            framebound_map_t m_vFrameBoundsOf;
            const Reference<XEnumerationAccess> m_xEnumAccess;
            void Fill(const filter_t& rFilter);
            static const OUString our_sAnchorType;
            static const OUString our_sAnchorFrame;
    };
    const OUString BoundFrames::our_sAnchorType = OUString::createFromAscii("AnchorType");
    const OUString BoundFrames::our_sAnchorFrame = OUString::createFromAscii("AnchorFrame");

    class FieldParamExporter
    {
        public:
            FieldParamExporter(SvXMLExport* const pExport, Reference<XNameContainer> xFieldParams)
                : m_pExport(pExport)
                , m_xFieldParams(xFieldParams)
                { };
            void Export();

        private:
            SvXMLExport* const m_pExport;
            const Reference<XNameContainer> m_xFieldParams;

            void ExportParameter(const OUString& sKey, const OUString& sValue);
    };
}

namespace xmloff
{
    class BoundFrameSets
    {
        public:
            BoundFrameSets(const Reference<XInterface> xModel);
            const BoundFrames* GetTexts() const
                { return m_pTexts.get(); };
            const BoundFrames* GetGraphics() const
                { return m_pGraphics.get(); };
            const BoundFrames* GetEmbeddeds() const
                { return m_pEmbeddeds.get(); };
            const BoundFrames* GetShapes() const
                { return m_pShapes.get(); };
        private:
            auto_ptr<BoundFrames> m_pTexts;
            auto_ptr<BoundFrames> m_pGraphics;
            auto_ptr<BoundFrames> m_pEmbeddeds;
            auto_ptr<BoundFrames> m_pShapes;
    };
}

typedef OUString *OUStringPtr;
SV_DECL_PTRARR_DEL( OUStrings_Impl, OUStringPtr, 20, 10 )
SV_IMPL_PTRARR( OUStrings_Impl, OUStringPtr )

SV_DECL_PTRARR_SORT_DEL( OUStringsSort_Impl, OUStringPtr, 20, 10 )
SV_IMPL_OP_PTRARR_SORT( OUStringsSort_Impl, OUStringPtr )

#ifdef DBG_UTIL
static int txtparae_bContainsIllegalCharacters = sal_False;
#endif

// The following map shows which property values are required:
//
// property						auto style pass		export
// --------------------------------------------------------
// ParaStyleName				if style exists		always
// ParaConditionalStyleName		if style exists		always
// NumberingRules				if style exists		always
// TextSection					always				always
// ParaChapterNumberingLevel	never				always
// NumberingIsNumber            never               always

// The conclusion is that for auto styles the first three properties
// should be queried using a multi property set if, and only if, an
// auto style needs to be exported. TextSection should be queried by
// an individual call to getPropertyvalue, because this seems to be
// less expensive than querying the first three properties if they aren't
// required.

// For the export pass all properties can be queried using a multi property
// set.

static const sal_Char* aParagraphPropertyNamesAuto[] =
{
	"NumberingRules",
	"ParaConditionalStyleName",
	"ParaStyleName",
	NULL
};

enum eParagraphPropertyNamesEnumAuto
{
	NUMBERING_RULES_AUTO = 0,
	PARA_CONDITIONAL_STYLE_NAME_AUTO = 1,
	PARA_STYLE_NAME_AUTO = 2
};

static const sal_Char* aParagraphPropertyNames[] =
{
    "NumberingIsNumber",
	"NumberingStyleName",			//#outline level,add by zhaojianwei

	//"ParaChapterNumberingLevel",	//#outline level,remove by zhaojianwei
	"OutlineLevel",					//<-end,add by zhaojianwei
	"ParaConditionalStyleName",
	"ParaStyleName",
	"TextSection",
    NULL
};

enum eParagraphPropertyNamesEnum
{
	NUMBERING_IS_NUMBER = 0,
	PARA_NUMBERING_STYLENAME = 1,		//#outline level,add by zhaojianwei
	//PARA_CHAPTER_NUMERBING_LEVEL = 1,	//#outline level,remove by zhaojianwei
	PARA_OUTLINE_LEVEL=2,				//<-end.add by zhaojianwei
	PARA_CONDITIONAL_STYLE_NAME = 3,
	PARA_STYLE_NAME = 4,
	TEXT_SECTION = 5
};

void BoundFrames::Fill(const filter_t& rFilter)
{
    if(!m_xEnumAccess.is())
        return;
    const Reference< XEnumeration > xEnum = m_xEnumAccess->createEnumeration();
    if(!xEnum.is())
        return;
    while(xEnum->hasMoreElements())
    {
        Reference<XPropertySet> xPropSet(xEnum->nextElement(), UNO_QUERY);
        Reference<XTextContent> xTextContent(xPropSet, UNO_QUERY);
        if(!xPropSet.is() || !xTextContent.is())
            continue;
        TextContentAnchorType eAnchor;
        xPropSet->getPropertyValue(our_sAnchorType) >>= eAnchor;
        if(TextContentAnchorType_AT_PAGE != eAnchor && TextContentAnchorType_AT_FRAME != eAnchor)
            continue;
        if(!rFilter(xTextContent))
            continue;

        TextContentSet::inserter_t pInserter = m_vPageBounds.getInserter();
        if(TextContentAnchorType_AT_FRAME == eAnchor)
        {
            Reference<XTextFrame> xAnchorTxtFrame(
                xPropSet->getPropertyValue(our_sAnchorFrame),
                uno::UNO_QUERY);
            pInserter = m_vFrameBoundsOf[xAnchorTxtFrame].getInserter();
        }
        *pInserter++ = xTextContent;
    }
}

BoundFrameSets::BoundFrameSets(const Reference<XInterface> xModel)
    : m_pTexts(new BoundFrames())
    , m_pGraphics(new BoundFrames())
    , m_pEmbeddeds(new BoundFrames())
    , m_pShapes(new BoundFrames())
{
    const Reference<XTextFramesSupplier> xTFS(xModel, UNO_QUERY);
    const Reference<XTextGraphicObjectsSupplier> xGOS(xModel, UNO_QUERY);
    const Reference<XTextEmbeddedObjectsSupplier> xEOS(xModel, UNO_QUERY);
    const Reference<XDrawPageSupplier> xDPS(xModel, UNO_QUERY);
    if(xTFS.is())
        m_pTexts = auto_ptr<BoundFrames>(new BoundFrames(
            Reference<XEnumerationAccess>(xTFS->getTextFrames(), UNO_QUERY),
            &lcl_TextContentsUnfiltered));
    if(xGOS.is())
        m_pGraphics = auto_ptr<BoundFrames>(new BoundFrames(
            Reference<XEnumerationAccess>(xGOS->getGraphicObjects(), UNO_QUERY),
            &lcl_TextContentsUnfiltered));
    if(xEOS.is())
        m_pEmbeddeds = auto_ptr<BoundFrames>(new BoundFrames(
            Reference<XEnumerationAccess>(xEOS->getEmbeddedObjects(), UNO_QUERY),
            &lcl_TextContentsUnfiltered));
    if(xDPS.is())
        m_pShapes = auto_ptr<BoundFrames>(new BoundFrames(
            Reference<XEnumerationAccess>(xDPS->getDrawPage(), UNO_QUERY),
            &lcl_ShapeFilter));
};

void FieldParamExporter::Export()
{
    static const Type aStringType = ::getCppuType((OUString*)0);
    static const Type aBoolType = ::getCppuType((sal_Bool*)0);
    static const Type aSeqType = ::getCppuType((Sequence<OUString>*)0);
    static const Type aIntType = ::getCppuType((sal_Int32*)0);
    Sequence<OUString> vParameters(m_xFieldParams->getElementNames());
    for(const OUString* pCurrent=::comphelper::stl_begin(vParameters); pCurrent!=::comphelper::stl_end(vParameters); ++pCurrent)
    {
        const Any aValue = m_xFieldParams->getByName(*pCurrent);
        const Type aValueType = aValue.getValueType();
        if(aValueType == aStringType)
        {
            OUString sValue;
            aValue >>= sValue;
            ExportParameter(*pCurrent,sValue);
        }
        else if(aValueType == aBoolType)
        {
            sal_Bool bValue = false;
            aValue >>= bValue;
            ExportParameter(*pCurrent,OUString::createFromAscii(bValue ? "true" : "false"));
        }
        else if(aValueType == aSeqType)
        {
            Sequence<OUString> vValue;
            aValue >>= vValue;
            for(OUString* pSeqCurrent = ::comphelper::stl_begin(vValue); pSeqCurrent != ::comphelper::stl_end(vValue); ++pSeqCurrent)
            {
                ExportParameter(*pCurrent, *pSeqCurrent);
            }
        }
        else if(aValueType == aIntType)
        {
            sal_Int32 nValue = 0;
            aValue >>= nValue;
            ExportParameter(*pCurrent, OUStringBuffer().append(nValue).makeStringAndClear());
        }
    }
}

void FieldParamExporter::ExportParameter(const OUString& sKey, const OUString& sValue)
{
    m_pExport->AddAttribute(XML_NAMESPACE_FIELD, XML_NAME, sKey);
    m_pExport->AddAttribute(XML_NAMESPACE_FIELD, XML_VALUE, sValue);
    m_pExport->StartElement(XML_NAMESPACE_FIELD, XML_PARAM, sal_False);
    m_pExport->EndElement(XML_NAMESPACE_FIELD, XML_PARAM, sal_False);
}

void XMLTextParagraphExport::Add( sal_uInt16 nFamily,
								  const Reference < XPropertySet > & rPropSet,
								  const XMLPropertyState** ppAddStates, bool bDontSeek )
{
	UniReference < SvXMLExportPropertyMapper > xPropMapper;
	switch( nFamily )
	{
	case XML_STYLE_FAMILY_TEXT_PARAGRAPH:
		xPropMapper = GetParaPropMapper();
		break;
	case XML_STYLE_FAMILY_TEXT_TEXT:
		xPropMapper = GetTextPropMapper();
		break;
	case XML_STYLE_FAMILY_TEXT_FRAME:
		xPropMapper = GetAutoFramePropMapper();
		break;
	case XML_STYLE_FAMILY_TEXT_SECTION:
		xPropMapper = GetSectionPropMapper();
		break;
	case XML_STYLE_FAMILY_TEXT_RUBY:
		xPropMapper = GetRubyPropMapper();
		break;
	}
	DBG_ASSERT( xPropMapper.is(), "There is the property mapper?" );

	vector< XMLPropertyState > xPropStates =
			xPropMapper->Filter( rPropSet );

	if( ppAddStates )
	{
		while( *ppAddStates )
		{
			xPropStates.push_back( **ppAddStates );
			ppAddStates++;
		}
	}

	if( !xPropStates.empty() )
	{
		Reference< XPropertySetInfo > xPropSetInfo(rPropSet->getPropertySetInfo());
		OUString sParent, sCondParent;
		sal_uInt16 nIgnoreProps = 0;
		switch( nFamily )
		{
		case XML_STYLE_FAMILY_TEXT_PARAGRAPH:
			if( xPropSetInfo->hasPropertyByName( sParaStyleName ) )
			{
				rPropSet->getPropertyValue( sParaStyleName ) >>= sParent;
			}
			if( xPropSetInfo->hasPropertyByName( sParaConditionalStyleName ) )
			{
				rPropSet->getPropertyValue( sParaConditionalStyleName ) >>= sCondParent;
			}
			if( xPropSetInfo->hasPropertyByName( sNumberingRules ) )
			{
                Reference < XIndexReplace > xNumRule(rPropSet->getPropertyValue( sNumberingRules ), uno::UNO_QUERY);
				if( xNumRule.is() && xNumRule->getCount() )
				{
					Reference < XNamed > xNamed( xNumRule, UNO_QUERY );
					OUString sName;
					if( xNamed.is() )
						sName = xNamed->getName();
					sal_Bool bAdd = !sName.getLength();
					if( !bAdd )
					{
						Reference < XPropertySet > xNumPropSet( xNumRule,
																UNO_QUERY );
                        const OUString sIsAutomatic( RTL_CONSTASCII_USTRINGPARAM( "IsAutomatic" ) );
						if( xNumPropSet.is() &&
							xNumPropSet->getPropertySetInfo()
									   ->hasPropertyByName( sIsAutomatic ) )
						{
							bAdd = *(sal_Bool *)xNumPropSet->getPropertyValue( sIsAutomatic ).getValue();
                            // --> OD 2007-01-12 #i73361# - check on outline style
                            const OUString sNumberingIsOutline( RTL_CONSTASCII_USTRINGPARAM( "NumberingIsOutline" ) );
                            if ( bAdd &&
                                 xNumPropSet->getPropertySetInfo()
                                           ->hasPropertyByName( sNumberingIsOutline ) )
                            {
                                bAdd = !(*(sal_Bool *)xNumPropSet->getPropertyValue( sNumberingIsOutline ).getValue());
                            }
                            // <--
						}
						else
						{
							bAdd = sal_True;
						}
					}
					if( bAdd )
						pListAutoPool->Add( xNumRule );
				}
			}
			break;
		case XML_STYLE_FAMILY_TEXT_TEXT:
			{
				// Get parent and remove hyperlinks (they aren't of interest)
				UniReference< XMLPropertySetMapper > xPM(xPropMapper->getPropertySetMapper());
				for( ::std::vector< XMLPropertyState >::iterator i(xPropStates.begin());
		 			 nIgnoreProps < 2 && i != xPropStates.end(); )
				{
					if( i->mnIndex == -1 )
                    {
                        ++i;
						continue;
                    }

					switch( xPM->GetEntryContextId(i->mnIndex) )
					{
					case CTF_CHAR_STYLE_NAME:
					case CTF_HYPERLINK_URL:
						i->mnIndex = -1;
						nIgnoreProps++;
                        i = xPropStates.erase( i );
						break;
                    default:
                        ++i;
                        break;
					}
				}
			}
			break;
		case XML_STYLE_FAMILY_TEXT_FRAME:
			if( xPropSetInfo->hasPropertyByName( sFrameStyleName ) )
			{
				rPropSet->getPropertyValue( sFrameStyleName ) >>= sParent;
			}
			break;
		case XML_STYLE_FAMILY_TEXT_SECTION:
		case XML_STYLE_FAMILY_TEXT_RUBY:
			; // section styles have no parents
			break;
		}
		if( (xPropStates.size() - nIgnoreProps) > 0 )
		{
			GetAutoStylePool().Add( nFamily, sParent, xPropStates, bDontSeek );
			if( sCondParent.getLength() && sParent != sCondParent )
				GetAutoStylePool().Add( nFamily, sCondParent, xPropStates );
		}
	}
}

bool lcl_validPropState( const XMLPropertyState& rState )
{
    return rState.mnIndex != -1;
}

void XMLTextParagraphExport::Add( sal_uInt16 nFamily,
								  MultiPropertySetHelper& rPropSetHelper,
								  const Reference < XPropertySet > & rPropSet,
								  const XMLPropertyState** ppAddStates)
{
	UniReference < SvXMLExportPropertyMapper > xPropMapper;
	switch( nFamily )
	{
	case XML_STYLE_FAMILY_TEXT_PARAGRAPH:
		xPropMapper = GetParaPropMapper();
		break;
	}
	DBG_ASSERT( xPropMapper.is(), "There is the property mapper?" );

	vector< XMLPropertyState > xPropStates(xPropMapper->Filter( rPropSet ));
	if( ppAddStates )
	{
		while( *ppAddStates )
		{
			xPropStates.push_back( **ppAddStates );
			++ppAddStates;
		}
	}

	if( rPropSetHelper.hasProperty( NUMBERING_RULES_AUTO ) )
	{
		Reference < XIndexReplace > xNumRule(rPropSetHelper.getValue( NUMBERING_RULES_AUTO,
            rPropSet, sal_True ), uno::UNO_QUERY);
		if( xNumRule.is() && xNumRule->getCount() )
		{
			Reference < XNamed > xNamed( xNumRule, UNO_QUERY );
			OUString sName;
			if( xNamed.is() )
				sName = xNamed->getName();
			sal_Bool bAdd = !sName.getLength();
			if( !bAdd )
			{
				Reference < XPropertySet > xNumPropSet( xNumRule,
														UNO_QUERY );
                const OUString sIsAutomatic( RTL_CONSTASCII_USTRINGPARAM( "IsAutomatic" ) );
				if( xNumPropSet.is() &&
					xNumPropSet->getPropertySetInfo()
							   ->hasPropertyByName( sIsAutomatic ) )
				{
					bAdd = *(sal_Bool *)xNumPropSet->getPropertyValue( sIsAutomatic ).getValue();
                    // --> OD 2007-01-12 #i73361# - check on outline style
                    const OUString sNumberingIsOutline( RTL_CONSTASCII_USTRINGPARAM( "NumberingIsOutline" ) );
                    if ( bAdd &&
                         xNumPropSet->getPropertySetInfo()
                                   ->hasPropertyByName( sNumberingIsOutline ) )
                    {
                        bAdd = !(*(sal_Bool *)xNumPropSet->getPropertyValue( sNumberingIsOutline ).getValue());
                    }
                    // <--
				}
				else
				{
					bAdd = sal_True;
				}
			}
			if( bAdd )
				pListAutoPool->Add( xNumRule );
		}
	}

	if( !xPropStates.empty() )
	{
		OUString sParent, sCondParent;
		switch( nFamily )
		{
		case XML_STYLE_FAMILY_TEXT_PARAGRAPH:
			if( rPropSetHelper.hasProperty( PARA_STYLE_NAME_AUTO ) )
			{
				rPropSetHelper.getValue( PARA_STYLE_NAME_AUTO, rPropSet,
												sal_True ) >>= sParent;
			}
			if( rPropSetHelper.hasProperty( PARA_CONDITIONAL_STYLE_NAME_AUTO ) )
			{
				rPropSetHelper.getValue( PARA_CONDITIONAL_STYLE_NAME_AUTO,
											 	rPropSet, sal_True ) >>= sCondParent;
			}

			break;
		}

        if( find_if( xPropStates.begin(), xPropStates.end(), lcl_validPropState ) != xPropStates.end() )
		{
			GetAutoStylePool().Add( nFamily, sParent, xPropStates );
			if( sCondParent.getLength() && sParent != sCondParent )
				GetAutoStylePool().Add( nFamily, sCondParent, xPropStates );
		}
	}
}

OUString XMLTextParagraphExport::Find(
		sal_uInt16 nFamily,
	   	const Reference < XPropertySet > & rPropSet,
		const OUString& rParent,
		const XMLPropertyState** ppAddStates) const
{
	OUString sName( rParent );
	UniReference < SvXMLExportPropertyMapper > xPropMapper;
	switch( nFamily )
	{
	case XML_STYLE_FAMILY_TEXT_PARAGRAPH:
		xPropMapper = GetParaPropMapper();
		break;
	case XML_STYLE_FAMILY_TEXT_FRAME:
		xPropMapper = GetAutoFramePropMapper();
		break;
	case XML_STYLE_FAMILY_TEXT_SECTION:
		xPropMapper = GetSectionPropMapper();
		break;
	case XML_STYLE_FAMILY_TEXT_RUBY:
		xPropMapper = GetRubyPropMapper();
		break;
	}
	DBG_ASSERT( xPropMapper.is(), "There is the property mapper?" );
	if( !xPropMapper.is() )
		return sName;
	vector< XMLPropertyState > xPropStates(xPropMapper->Filter( rPropSet ));
	if( ppAddStates )
	{
		while( *ppAddStates )
		{
			xPropStates.push_back( **ppAddStates );
			++ppAddStates;
		}
	}
    if( find_if( xPropStates.begin(), xPropStates.end(), lcl_validPropState ) != xPropStates.end() )
		sName = GetAutoStylePool().Find( nFamily, sName, xPropStates );

	return sName;
}

OUString XMLTextParagraphExport::FindTextStyleAndHyperlink(
	   	const Reference < XPropertySet > & rPropSet,
        sal_Bool& rbHyperlink,
        sal_Bool& rbHasCharStyle,
        sal_Bool& rbHasAutoStyle,
		const XMLPropertyState** ppAddStates ) const
{
	UniReference < SvXMLExportPropertyMapper > xPropMapper(GetTextPropMapper());
	vector< XMLPropertyState > xPropStates(xPropMapper->Filter( rPropSet ));

	// Get parent and remove hyperlinks (they aren't of interest)
	OUString sName;
    rbHyperlink = rbHasCharStyle = rbHasAutoStyle = sal_False;
	sal_uInt16 nIgnoreProps = 0;
	UniReference< XMLPropertySetMapper > xPM(xPropMapper->getPropertySetMapper());
    ::std::vector< XMLPropertyState >::iterator aFirstDel = xPropStates.end();
    ::std::vector< XMLPropertyState >::iterator aSecondDel = xPropStates.end();

	for( ::std::vector< XMLPropertyState >::iterator
			i = xPropStates.begin();
		 nIgnoreProps < 2 && i != xPropStates.end();
		 i++ )
	{
		if( i->mnIndex == -1 )
			continue;

		switch( xPM->GetEntryContextId(i->mnIndex) )
		{
		case CTF_CHAR_STYLE_NAME:
			i->maValue >>= sName;
			i->mnIndex = -1;
            rbHasCharStyle = sName.getLength() > 0;
            if( nIgnoreProps )
                aSecondDel = i;
            else
                aFirstDel = i;
			nIgnoreProps++;
			break;
		case CTF_HYPERLINK_URL:
            rbHyperlink = sal_True;
			i->mnIndex = -1;
            if( nIgnoreProps )
                aSecondDel = i;
            else
                aFirstDel = i;
			nIgnoreProps++;
			break;
		}
	}
	if( ppAddStates )
	{
		while( *ppAddStates )
		{
			xPropStates.push_back( **ppAddStates );
			ppAddStates++;
		}
	}
	if( (xPropStates.size() - nIgnoreProps) > 0L )
    {
        // erase the character style, otherwise the autostyle cannot be found!
        // erase the hyperlink, otherwise the autostyle cannot be found!
        if ( nIgnoreProps )
        {
            // If two elements of a vector have to be deleted,
            // we should delete the second one first.
            if( --nIgnoreProps )
                xPropStates.erase( aSecondDel );
            xPropStates.erase( aFirstDel );
        }
        OUString sParent; // AutoStyles should not have parents!
		sName = GetAutoStylePool().Find( XML_STYLE_FAMILY_TEXT_TEXT, sParent, xPropStates );
        DBG_ASSERT( sName.getLength(), "AutoStyle could not be found" );
        rbHasAutoStyle = sal_True;
    }

	return sName;
}

OUString XMLTextParagraphExport::FindTextStyle(
	   	const Reference < XPropertySet > & rPropSet,
		sal_Bool& rHasCharStyle ) const
{
    sal_Bool bDummy;
    sal_Bool bDummy2;
    return FindTextStyleAndHyperlink( rPropSet, bDummy, rHasCharStyle, bDummy2 );
}


// --> OD 2008-04-25 #refactorlists#
// adjustments to support lists independent from list style
void XMLTextParagraphExport::exportListChange(
		const XMLTextNumRuleInfo& rPrevInfo,
	    const XMLTextNumRuleInfo& rNextInfo )
{
	// end a list
    if ( rPrevInfo.GetLevel() > 0 )
    {
        bool bRootListToBeClosed = false;
        sal_Int16 nListLevelsToBeClosed = 0;
        if ( !rNextInfo.BelongsToSameList( rPrevInfo ) ||
             rNextInfo.GetLevel() <= 0 )
        {
            // close complete previous list
            bRootListToBeClosed = true;
            nListLevelsToBeClosed = rPrevInfo.GetLevel();
        }
        else if ( rPrevInfo.GetLevel() > rNextInfo.GetLevel() )
        {
            // close corresponding sub lists
            DBG_ASSERT( rNextInfo.GetLevel() > 0,
                        "<rPrevInfo.GetLevel() > 0> not hold. Serious defect -> please inform OD." );
            nListLevelsToBeClosed = rPrevInfo.GetLevel() - rNextInfo.GetLevel();
        }

        if ( nListLevelsToBeClosed > 0 &&
             pListElements &&
             pListElements->Count() >= ( 2 * nListLevelsToBeClosed ) )
        {
            do {
                for( sal_uInt16 j = 0; j < 2; ++j )
                {
                    OUString *pElem = (*pListElements)[pListElements->Count()-1];
                    pListElements->Remove( pListElements->Count()-1 );

                    GetExport().EndElement( *pElem, sal_True );

                    delete pElem;
                }

                // remove closed list from list stack
                mpTextListsHelper->PopListFromStack();

                --nListLevelsToBeClosed;
            } while ( nListLevelsToBeClosed > 0 );
        }
    }

    const bool bExportODF =
                ( GetExport().getExportFlags() & EXPORT_OASIS ) != 0;
    const SvtSaveOptions::ODFDefaultVersion eODFDefaultVersion =
                                    GetExport().getDefaultVersion();

    // start a new list
    if ( rNextInfo.GetLevel() > 0 )
    {
        bool bRootListToBeStarted = false;
        sal_Int16 nListLevelsToBeOpened = 0;
        if ( !rPrevInfo.BelongsToSameList( rNextInfo ) ||
             rPrevInfo.GetLevel() <= 0 )
        {
            // new root list
            bRootListToBeStarted = true;
            nListLevelsToBeOpened = rNextInfo.GetLevel();
        }
        else if ( rNextInfo.GetLevel() > rPrevInfo.GetLevel() )
        {
            // open corresponding sub lists
            DBG_ASSERT( rPrevInfo.GetLevel() > 0,
                        "<rPrevInfo.GetLevel() > 0> not hold. Serious defect -> please inform OD." );
            nListLevelsToBeOpened = rNextInfo.GetLevel() - rPrevInfo.GetLevel();
        }

        if ( nListLevelsToBeOpened > 0 )
        {
            const ::rtl::OUString sListStyleName( rNextInfo.GetNumRulesName() );
            // Currently only the text documents support <ListId>.
            // Thus, for other document types <sListId> is empty.
            const ::rtl::OUString sListId( rNextInfo.GetListId() );
            bool bExportListStyle( true );
            bool bRestartNumberingAtContinuedRootList( false );
            sal_Int16 nRestartValueForContinuedRootList( -1 );
            // --> OD 2008-11-26 #158694#
            bool bContinueingPreviousSubList = !bRootListToBeStarted &&
                                               rNextInfo.IsContinueingPreviousSubTree();
            // <--
            do {
                GetExport().CheckAttrList();

                if ( bRootListToBeStarted )
                {
                    if ( !mpTextListsHelper->IsListProcessed( sListId ) )
                    {
                        if ( bExportODF &&
                             eODFDefaultVersion >= SvtSaveOptions::ODFVER_012 &&
                             sListId.getLength() > 0 )
                        {
                            // --> OD 2008-07-31 #i92221#
                            GetExport().AddAttribute( XML_NAMESPACE_XML,
                                                      XML_ID,
                                                      sListId );
                            // <--
                        }
                        mpTextListsHelper->KeepListAsProcessed( sListId,
                                                                sListStyleName,
                                                                ::rtl::OUString() );
                    }
                    else
                    {
                        const ::rtl::OUString sNewListId(
                                        mpTextListsHelper->GenerateNewListId() );
                        if ( bExportODF &&
                             eODFDefaultVersion >= SvtSaveOptions::ODFVER_012 &&
                             sListId.getLength() > 0 )
                        {
                            // --> OD 2008-07-31 #i92221#
                            GetExport().AddAttribute( XML_NAMESPACE_XML,
                                                      XML_ID,
                                                      sNewListId );
                            // <--
                        }

                        const ::rtl::OUString sContinueListId =
                            mpTextListsHelper->GetLastContinuingListId( sListId );
                        // store that list with list id <sNewListId> is last list,
                        // which has continued list with list id <sListId>
                        mpTextListsHelper->StoreLastContinuingList( sListId,
                                                                    sNewListId );
                        if ( sListStyleName ==
                                mpTextListsHelper->GetListStyleOfLastProcessedList() &&
                             // --> OD 2008-08-15 #i92811#
                             sContinueListId ==
                                mpTextListsHelper->GetLastProcessedListId() &&
                             // <--
                             !rNextInfo.IsRestart() )
                        {
                            GetExport().AddAttribute( XML_NAMESPACE_TEXT,
                                                      XML_CONTINUE_NUMBERING,
                                                      XML_TRUE );
                        }
                        else
                        {
                            if ( bExportODF &&
                                 eODFDefaultVersion >= SvtSaveOptions::ODFVER_012 &&
                                 sListId.getLength() > 0 )
                            {
                                GetExport().AddAttribute( XML_NAMESPACE_TEXT,
                                                          XML_CONTINUE_LIST,
                                                          sContinueListId );
                            }

                            if ( rNextInfo.IsRestart() &&
                                 ( nListLevelsToBeOpened != 1 ||
                                   !rNextInfo.HasStartValue() ) )
                            {
                                bRestartNumberingAtContinuedRootList = true;
                                nRestartValueForContinuedRootList =
                                                rNextInfo.GetListLevelStartValue();
                            }
                        }

                        mpTextListsHelper->KeepListAsProcessed( sNewListId,
                                                                sListStyleName,
                                                                sContinueListId );
                    }

                    GetExport().AddAttribute( XML_NAMESPACE_TEXT, XML_STYLE_NAME,
                            GetExport().EncodeStyleName( sListStyleName ) );
                    bExportListStyle = false;

                    bRootListToBeStarted = false;
                }
                else if ( bExportListStyle &&
                          !mpTextListsHelper->EqualsToTopListStyleOnStack( sListStyleName ) )
                {
                    GetExport().AddAttribute( XML_NAMESPACE_TEXT, XML_STYLE_NAME,
                            GetExport().EncodeStyleName( sListStyleName ) );
                    bExportListStyle = false;
                }

                // --> OD 2008-11-26 #158694#
                if ( bContinueingPreviousSubList )
                {
                    GetExport().AddAttribute( XML_NAMESPACE_TEXT,
                                              XML_CONTINUE_NUMBERING, XML_TRUE );
                    bContinueingPreviousSubList = false;
                }
                // <--

                enum XMLTokenEnum eLName = XML_LIST;

                OUString *pElem = new OUString(
                        GetExport().GetNamespaceMap().GetQNameByKey(
                                            XML_NAMESPACE_TEXT,
                                            GetXMLToken(eLName) ) );
                GetExport().IgnorableWhitespace();
                GetExport().StartElement( *pElem, sal_False );

                if( !pListElements )
                    pListElements = new OUStrings_Impl;
                pListElements->Insert( pElem, pListElements->Count() );

                mpTextListsHelper->PushListOnStack( sListId,
                                                    sListStyleName );

                // <text:list-header> or <text:list-item>
                GetExport().CheckAttrList();

                // --> OD 2009-06-24 #i97309#
                // export start value in case of <bRestartNumberingAtContinuedRootList>
                // at correct list item
                if ( nListLevelsToBeOpened == 1 )
                {
                    if ( rNextInfo.HasStartValue() )
                    {
                        OUStringBuffer aBuffer;
                        aBuffer.append( (sal_Int32)rNextInfo.GetStartValue() );
                        GetExport().AddAttribute( XML_NAMESPACE_TEXT, XML_START_VALUE,
                                      aBuffer.makeStringAndClear() );
                    }
                    else if ( bRestartNumberingAtContinuedRootList )
                    {
                        OUStringBuffer aBuffer;
                        aBuffer.append( (sal_Int32)nRestartValueForContinuedRootList );
                        GetExport().AddAttribute( XML_NAMESPACE_TEXT,
                                                  XML_START_VALUE,
                                                  aBuffer.makeStringAndClear() );
                        bRestartNumberingAtContinuedRootList = false;
                    }
                }
                // <--

                eLName = ( rNextInfo.IsNumbered() || nListLevelsToBeOpened > 1 )
                         ? XML_LIST_ITEM
                         : XML_LIST_HEADER;
                pElem = new OUString(  GetExport().GetNamespaceMap().GetQNameByKey(
                                            XML_NAMESPACE_TEXT,
                                            GetXMLToken(eLName) ) );
                GetExport().IgnorableWhitespace();
                GetExport().StartElement( *pElem, sal_False );

                pListElements->Insert( pElem, pListElements->Count() );

                // --> OD 2008-11-26 #158694#
                // export of <text:number> element for last opened <text:list-item>, if requested
                if ( GetExport().exportTextNumberElement() &&
                     eLName == XML_LIST_ITEM && nListLevelsToBeOpened == 1 && // last iteration --> last opened <text:list-item>
                     rNextInfo.ListLabelString().getLength() > 0 )
                {
                    const ::rtl::OUString aTextNumberElem =
                            OUString( GetExport().GetNamespaceMap().GetQNameByKey(
                                      XML_NAMESPACE_TEXT,
                                      GetXMLToken(XML_NUMBER) ) );
                    GetExport().IgnorableWhitespace();
                    GetExport().StartElement( aTextNumberElem, sal_False );
                    GetExport().Characters( rNextInfo.ListLabelString() );
                    GetExport().EndElement( aTextNumberElem, sal_True );
                }
                // <--

                --nListLevelsToBeOpened;
            } while ( nListLevelsToBeOpened > 0 );
        }
	}

    if ( rNextInfo.GetLevel() > 0 &&
         rNextInfo.IsNumbered() &&
         rPrevInfo.BelongsToSameList( rNextInfo ) &&
         rPrevInfo.GetLevel() >= rNextInfo.GetLevel() )
	{
        // close previous list-item
        DBG_ASSERT( pListElements && pListElements->Count() >= 2,
				"SwXMLExport::ExportListChange: list elements missing" );

		OUString *pElem = (*pListElements)[pListElements->Count()-1];
		GetExport().EndElement( *pElem, sal_True );

		pListElements->Remove( pListElements->Count()-1 );
		delete pElem;

        // --> OD 2009-11-12 #i103745# - only for sub lists
        if ( rNextInfo.IsRestart() && !rNextInfo.HasStartValue() &&
             rNextInfo.GetLevel() != 1 )
        // <--
        {
            // start new sub list respectively list on same list level
            pElem = (*pListElements)[pListElements->Count()-1];
            GetExport().EndElement( *pElem, sal_True );
            GetExport().IgnorableWhitespace();
            GetExport().StartElement( *pElem, sal_False );
        }

        // open new list-item
		GetExport().CheckAttrList();
		if( rNextInfo.HasStartValue() )
		{
			OUStringBuffer aBuffer;
			aBuffer.append( (sal_Int32)rNextInfo.GetStartValue() );
			GetExport().AddAttribute( XML_NAMESPACE_TEXT, XML_START_VALUE,
                                      aBuffer.makeStringAndClear() );
		}
        // --> OD 2009-11-12 #i103745# - handle restart without start value on list level 1
        else if ( rNextInfo.IsRestart() && /*!rNextInfo.HasStartValue() &&*/
                  rNextInfo.GetLevel() == 1 )
        {
            OUStringBuffer aBuffer;
            aBuffer.append( (sal_Int32)rNextInfo.GetListLevelStartValue() );
            GetExport().AddAttribute( XML_NAMESPACE_TEXT, XML_START_VALUE,
                                      aBuffer.makeStringAndClear() );
        }
        // <--
        if ( ( GetExport().getExportFlags() & EXPORT_OASIS ) != 0 &&
             GetExport().getDefaultVersion() >= SvtSaveOptions::ODFVER_012 )
        {
            const ::rtl::OUString sListStyleName( rNextInfo.GetNumRulesName() );
            if ( !mpTextListsHelper->EqualsToTopListStyleOnStack( sListStyleName ) )
            {
                GetExport().AddAttribute( XML_NAMESPACE_TEXT,
                                          XML_STYLE_OVERRIDE,
                                          GetExport().EncodeStyleName( sListStyleName ) );
            }
        }
		pElem = new OUString( GetExport().GetNamespaceMap().GetQNameByKey(
								XML_NAMESPACE_TEXT,
								GetXMLToken(XML_LIST_ITEM) ) );
		GetExport().IgnorableWhitespace();
		GetExport().StartElement( *pElem, sal_False );

		pListElements->Insert( pElem, pListElements->Count() );

        // --> OD 2008-11-26 #158694#
        // export of <text:number> element for <text:list-item>, if requested
        if ( GetExport().exportTextNumberElement() &&
             rNextInfo.ListLabelString().getLength() > 0 )
        {
            const ::rtl::OUString aTextNumberElem =
                    OUString( GetExport().GetNamespaceMap().GetQNameByKey(
                              XML_NAMESPACE_TEXT,
                              GetXMLToken(XML_NUMBER) ) );
            GetExport().IgnorableWhitespace();
            GetExport().StartElement( aTextNumberElem, sal_False );
            GetExport().Characters( rNextInfo.ListLabelString() );
            GetExport().EndElement( aTextNumberElem, sal_True );
        }
        // <--
    }
}
// <--

XMLTextParagraphExport::XMLTextParagraphExport(
		SvXMLExport& rExp,
		SvXMLAutoStylePoolP & rASP
	    ) :
	XMLStyleExport( rExp, OUString(), &rASP ),
	rAutoStylePool( rASP ),
    pBoundFrameSets(new BoundFrameSets(GetExport().GetModel())),
	pFieldExport( 0 ),
	pListElements( 0 ),
    // --> OD 2008-05-07 #refactorlists# - no longer needed
//    pExportedLists( 0 ),
    // <--
	pListAutoPool( new XMLTextListAutoStylePool( this->GetExport() ) ),
	pSectionExport( NULL ),
	pIndexMarkExport( NULL ),

	pRedlineExport( NULL ),
	pHeadingStyles( NULL ),

	bProgress( sal_False ),
	bBlock( sal_False ),

	bOpenRuby( sal_False ),
    // --> OD 2008-04-25 #refactorlists#
    mpTextListsHelper( 0 ),
    maTextListsHelperStack(),
    // <--

	sActualSize(RTL_CONSTASCII_USTRINGPARAM("ActualSize")),
    // --> OD 2009-07-22 #i73249#
//    sAlternativeText(RTL_CONSTASCII_USTRINGPARAM("AlternativeText")),
    sTitle(RTL_CONSTASCII_USTRINGPARAM("Title")),
    sDescription(RTL_CONSTASCII_USTRINGPARAM("Description")),
    // <--
	sAnchorCharStyleName(RTL_CONSTASCII_USTRINGPARAM("AnchorCharStyleName")),
	sAnchorPageNo(RTL_CONSTASCII_USTRINGPARAM("AnchorPageNo")),
	sAnchorType(RTL_CONSTASCII_USTRINGPARAM("AnchorType")),
	sBeginNotice(RTL_CONSTASCII_USTRINGPARAM("BeginNotice")),
	sBookmark(RTL_CONSTASCII_USTRINGPARAM("Bookmark")),
	sCategory(RTL_CONSTASCII_USTRINGPARAM("Category")),
	sChainNextName(RTL_CONSTASCII_USTRINGPARAM("ChainNextName")),
	sCharStyleName(RTL_CONSTASCII_USTRINGPARAM("CharStyleName")),
	sCharStyleNames(RTL_CONSTASCII_USTRINGPARAM("CharStyleNames")),
	sContourPolyPolygon(RTL_CONSTASCII_USTRINGPARAM("ContourPolyPolygon")),
	sDocumentIndex(RTL_CONSTASCII_USTRINGPARAM("DocumentIndex")),
	sDocumentIndexMark(RTL_CONSTASCII_USTRINGPARAM("DocumentIndexMark")),
	sEndNotice(RTL_CONSTASCII_USTRINGPARAM("EndNotice")),
	sFootnote(RTL_CONSTASCII_USTRINGPARAM("Footnote")),
	sFootnoteCounting(RTL_CONSTASCII_USTRINGPARAM("FootnoteCounting")),
	sFrame(RTL_CONSTASCII_USTRINGPARAM("Frame")),
	sFrameHeightAbsolute(RTL_CONSTASCII_USTRINGPARAM("FrameHeightAbsolute")),
	sFrameHeightPercent(RTL_CONSTASCII_USTRINGPARAM("FrameHeightPercent")),
	sFrameStyleName(RTL_CONSTASCII_USTRINGPARAM("FrameStyleName")),
	sFrameWidthAbsolute(RTL_CONSTASCII_USTRINGPARAM("FrameWidthAbsolute")),
	sFrameWidthPercent(RTL_CONSTASCII_USTRINGPARAM("FrameWidthPercent")),
	sGraphicFilter(RTL_CONSTASCII_USTRINGPARAM("GraphicFilter")),
	sGraphicRotation(RTL_CONSTASCII_USTRINGPARAM("GraphicRotation")),
	sGraphicURL(RTL_CONSTASCII_USTRINGPARAM("GraphicURL")),
	sReplacementGraphicURL(RTL_CONSTASCII_USTRINGPARAM("ReplacementGraphicURL")),
	sHeight(RTL_CONSTASCII_USTRINGPARAM("Height")),
	sHoriOrient(RTL_CONSTASCII_USTRINGPARAM("HoriOrient")),
	sHoriOrientPosition(RTL_CONSTASCII_USTRINGPARAM("HoriOrientPosition")),
	sHyperLinkName(RTL_CONSTASCII_USTRINGPARAM("HyperLinkName")),
	sHyperLinkTarget(RTL_CONSTASCII_USTRINGPARAM("HyperLinkTarget")),
	sHyperLinkURL(RTL_CONSTASCII_USTRINGPARAM("HyperLinkURL")),
	sIsAutomaticContour(RTL_CONSTASCII_USTRINGPARAM("IsAutomaticContour")),
	sIsCollapsed(RTL_CONSTASCII_USTRINGPARAM("IsCollapsed")),
	sIsPixelContour(RTL_CONSTASCII_USTRINGPARAM("IsPixelContour")),
	sIsStart(RTL_CONSTASCII_USTRINGPARAM("IsStart")),
	sIsSyncHeightToWidth(RTL_CONSTASCII_USTRINGPARAM("IsSyncHeightToWidth")),
	sIsSyncWidthToHeight(RTL_CONSTASCII_USTRINGPARAM("IsSyncWidthToHeight")),
	sNumberingRules(RTL_CONSTASCII_USTRINGPARAM("NumberingRules")),
	sNumberingType(RTL_CONSTASCII_USTRINGPARAM("NumberingType")),
	sPageDescName(RTL_CONSTASCII_USTRINGPARAM("PageDescName")),
	sPageStyleName(RTL_CONSTASCII_USTRINGPARAM("PageStyleName")),
	sParaChapterNumberingLevel(RTL_CONSTASCII_USTRINGPARAM("ParaChapterNumberingLevel")),
	sParaConditionalStyleName(RTL_CONSTASCII_USTRINGPARAM("ParaConditionalStyleName")),
	sParagraphService(RTL_CONSTASCII_USTRINGPARAM("com.sun.star.text.Paragraph")),
	sParaStyleName(RTL_CONSTASCII_USTRINGPARAM("ParaStyleName")),
	sPositionEndOfDoc(RTL_CONSTASCII_USTRINGPARAM("PositionEndOfDoc")),
	sPrefix(RTL_CONSTASCII_USTRINGPARAM("Prefix")),
	sRedline(RTL_CONSTASCII_USTRINGPARAM("Redline")),
	sReferenceId(RTL_CONSTASCII_USTRINGPARAM("ReferenceId")),
	sReferenceMark(RTL_CONSTASCII_USTRINGPARAM("ReferenceMark")),
	sRelativeHeight(RTL_CONSTASCII_USTRINGPARAM("RelativeHeight")),
	sRelativeWidth(RTL_CONSTASCII_USTRINGPARAM("RelativeWidth")),
	sRuby(RTL_CONSTASCII_USTRINGPARAM("Ruby")),
	sRubyAdjust(RTL_CONSTASCII_USTRINGPARAM("RubyAdjust")),
	sRubyCharStyleName(RTL_CONSTASCII_USTRINGPARAM("RubyCharStyleName")),
	sRubyText(RTL_CONSTASCII_USTRINGPARAM("RubyText")),
	sServerMap(RTL_CONSTASCII_USTRINGPARAM("ServerMap")),
	sShapeService(RTL_CONSTASCII_USTRINGPARAM("com.sun.star.drawing.Shape")),
	sSizeType(RTL_CONSTASCII_USTRINGPARAM("SizeType")),
	sSoftPageBreak( RTL_CONSTASCII_USTRINGPARAM( "SoftPageBreak" ) ),
	sStartAt(RTL_CONSTASCII_USTRINGPARAM("StartAt")),
	sSuffix(RTL_CONSTASCII_USTRINGPARAM("Suffix")),
	sTableService(RTL_CONSTASCII_USTRINGPARAM("com.sun.star.text.TextTable")),
	sText(RTL_CONSTASCII_USTRINGPARAM("Text")),
	sTextContentService(RTL_CONSTASCII_USTRINGPARAM("com.sun.star.text.TextContent")),
	sTextEmbeddedService(RTL_CONSTASCII_USTRINGPARAM("com.sun.star.text.TextEmbeddedObject")),
	sTextEndnoteService(RTL_CONSTASCII_USTRINGPARAM("com.sun.star.text.Endnote")),
	sTextField(RTL_CONSTASCII_USTRINGPARAM("TextField")),
	sTextFieldService(RTL_CONSTASCII_USTRINGPARAM("com.sun.star.text.TextField")),
	sTextFrameService(RTL_CONSTASCII_USTRINGPARAM("com.sun.star.text.TextFrame")),
	sTextGraphicService(RTL_CONSTASCII_USTRINGPARAM("com.sun.star.text.TextGraphicObject")),
	sTextPortionType(RTL_CONSTASCII_USTRINGPARAM("TextPortionType")),
	sTextSection(RTL_CONSTASCII_USTRINGPARAM("TextSection")),
	sUnvisitedCharStyleName(RTL_CONSTASCII_USTRINGPARAM("UnvisitedCharStyleName")),
	sVertOrient(RTL_CONSTASCII_USTRINGPARAM("VertOrient")),
	sVertOrientPosition(RTL_CONSTASCII_USTRINGPARAM("VertOrientPosition")),
	sVisitedCharStyleName(RTL_CONSTASCII_USTRINGPARAM("VisitedCharStyleName")),
	sWidth(RTL_CONSTASCII_USTRINGPARAM("Width")),
	sWidthType( RTL_CONSTASCII_USTRINGPARAM( "WidthType" ) ),
	sTextFieldStart( RTL_CONSTASCII_USTRINGPARAM( "TextFieldStart" ) ),
	sTextFieldEnd( RTL_CONSTASCII_USTRINGPARAM( "TextFieldEnd" ) ),
	sTextFieldStartEnd( RTL_CONSTASCII_USTRINGPARAM( "TextFieldStartEnd" ) ),
	aCharStyleNamesPropInfoCache( sCharStyleNames )
{
	UniReference < XMLPropertySetMapper > xPropMapper(new XMLTextPropertySetMapper( TEXT_PROP_MAP_PARA ));
	xParaPropMapper = new XMLTextExportPropertySetMapper( xPropMapper,
								   						  GetExport() );

	OUString sFamily( GetXMLToken(XML_PARAGRAPH) );
	OUString aPrefix( String( 'P' ) );
	rAutoStylePool.AddFamily( XML_STYLE_FAMILY_TEXT_PARAGRAPH, sFamily,
							  xParaPropMapper, aPrefix );

	xPropMapper = new XMLTextPropertySetMapper( TEXT_PROP_MAP_TEXT );
	xTextPropMapper = new XMLTextExportPropertySetMapper( xPropMapper,
								   						  GetExport() );
	sFamily = OUString( GetXMLToken(XML_TEXT) );
	aPrefix = OUString( String( 'T' ) );
	rAutoStylePool.AddFamily( XML_STYLE_FAMILY_TEXT_TEXT, sFamily,
							  xTextPropMapper, aPrefix );

	xPropMapper = new XMLTextPropertySetMapper( TEXT_PROP_MAP_AUTO_FRAME );
	xAutoFramePropMapper = new XMLTextExportPropertySetMapper( xPropMapper,
								   							   GetExport() );
	sFamily = OUString( RTL_CONSTASCII_USTRINGPARAM(XML_STYLE_FAMILY_SD_GRAPHICS_NAME) );
	aPrefix = OUString( RTL_CONSTASCII_USTRINGPARAM( "fr" ) );
	rAutoStylePool.AddFamily( XML_STYLE_FAMILY_TEXT_FRAME, sFamily,
							  xAutoFramePropMapper, aPrefix );

	xPropMapper = new XMLTextPropertySetMapper( TEXT_PROP_MAP_SECTION );
	xSectionPropMapper = new XMLTextExportPropertySetMapper( xPropMapper,
															 GetExport() );
	sFamily = OUString( GetXMLToken( XML_SECTION ) );
	aPrefix = OUString( RTL_CONSTASCII_USTRINGPARAM( "Sect" ) );
	rAutoStylePool.AddFamily( XML_STYLE_FAMILY_TEXT_SECTION, sFamily,
							  xSectionPropMapper, aPrefix );

	xPropMapper = new XMLTextPropertySetMapper( TEXT_PROP_MAP_RUBY );
	xRubyPropMapper = new SvXMLExportPropertyMapper( xPropMapper );
	sFamily = OUString( GetXMLToken( XML_RUBY ) );
	aPrefix = OUString( RTL_CONSTASCII_USTRINGPARAM( "Ru" ) );
	rAutoStylePool.AddFamily( XML_STYLE_FAMILY_TEXT_RUBY, sFamily,
							  xRubyPropMapper, aPrefix );

	xPropMapper = new XMLTextPropertySetMapper( TEXT_PROP_MAP_FRAME );
	xFramePropMapper = new XMLTextExportPropertySetMapper( xPropMapper,
								   						   GetExport() );

	pSectionExport = new XMLSectionExport( rExp, *this );
	pIndexMarkExport = new XMLIndexMarkExport( rExp, *this );

    if( ! IsBlockMode() &&
        Reference<XRedlinesSupplier>( GetExport().GetModel(), UNO_QUERY ).is())
        pRedlineExport = new XMLRedlineExport( rExp );

	// The text field helper needs a pre-constructed XMLPropertyState
	// to export the combined characters field. We construct that
	// here, because we need the text property mapper to do it.

	// construct Any value, then find index
	sal_Int32 nIndex = xTextPropMapper->getPropertySetMapper()->FindEntryIndex(
								"", XML_NAMESPACE_STYLE,
                                GetXMLToken(XML_TEXT_COMBINE));
    pFieldExport = new XMLTextFieldExport( rExp, new XMLPropertyState( nIndex, uno::makeAny(sal_True) ) );

    // --> OD 2008-05-08 #refactorlists#
    PushNewTextListsHelper();
    // <--
}

XMLTextParagraphExport::~XMLTextParagraphExport()
{
	delete pHeadingStyles;
	delete pRedlineExport;
	delete pIndexMarkExport;
	delete pSectionExport;
	delete pFieldExport;
	delete pListElements;
    // --> OD 2008-05-07 #refactorlists# - no longer needed
//    delete pExportedLists;
    // <--
	delete pListAutoPool;
#ifdef DBG_UTIL
	txtparae_bContainsIllegalCharacters = sal_False;
#endif
    // --> OD 2008-04-25 #refactorlists#
    // also deletes <mpTextListsHelper>
    PopTextListsHelper();
    DBG_ASSERT( maTextListsHelperStack.size() == 0,
                "misusage of text lists helper stack - it is not empty. Serious defect - please inform OD" );
    // <--
}

SvXMLExportPropertyMapper *XMLTextParagraphExport::CreateShapeExtPropMapper(
		SvXMLExport& rExport )
{
	UniReference < XMLPropertySetMapper > xPropMapper =
		new XMLTextPropertySetMapper( TEXT_PROP_MAP_SHAPE );
	return new XMLTextExportPropertySetMapper( xPropMapper, rExport );
}

SvXMLExportPropertyMapper *XMLTextParagraphExport::CreateCharExtPropMapper(
		SvXMLExport& rExport)
{
	XMLPropertySetMapper *pPropMapper =
		new XMLTextPropertySetMapper( TEXT_PROP_MAP_TEXT );
	return new XMLTextExportPropertySetMapper( pPropMapper, rExport );
}

SvXMLExportPropertyMapper *XMLTextParagraphExport::CreateParaExtPropMapper(
		SvXMLExport& rExport)
{
	XMLPropertySetMapper *pPropMapper =
		new XMLTextPropertySetMapper( TEXT_PROP_MAP_SHAPE_PARA );
	return new XMLTextExportPropertySetMapper( pPropMapper, rExport );
}

SvXMLExportPropertyMapper *XMLTextParagraphExport::CreateParaDefaultExtPropMapper(
		SvXMLExport& rExport)
{
	XMLPropertySetMapper *pPropMapper =
		new XMLTextPropertySetMapper( TEXT_PROP_MAP_TEXT_ADDITIONAL_DEFAULTS );
	return new XMLTextExportPropertySetMapper( pPropMapper, rExport );
}

void XMLTextParagraphExport::exportPageFrames( sal_Bool bAutoStyles,
											   sal_Bool bIsProgress )
{
    const TextContentSet* const pTexts = pBoundFrameSets->GetTexts()->GetPageBoundContents();
    const TextContentSet* const pGraphics = pBoundFrameSets->GetGraphics()->GetPageBoundContents();
    const TextContentSet* const pEmbeddeds = pBoundFrameSets->GetEmbeddeds()->GetPageBoundContents();
    const TextContentSet* const pShapes = pBoundFrameSets->GetShapes()->GetPageBoundContents();
    for(TextContentSet::const_iterator_t it = pTexts->getBegin();
        it != pTexts->getEnd();
        ++it)
        exportTextFrame(*it, bAutoStyles, bIsProgress, sal_True);
    for(TextContentSet::const_iterator_t it = pGraphics->getBegin();
        it != pGraphics->getEnd();
        ++it)
        exportTextGraphic(*it, bAutoStyles);
    for(TextContentSet::const_iterator_t it = pEmbeddeds->getBegin();
        it != pEmbeddeds->getEnd();
        ++it)
        exportTextEmbedded(*it, bAutoStyles);
    for(TextContentSet::const_iterator_t it = pShapes->getBegin();
        it != pShapes->getEnd();
        ++it)
        exportShape(*it, bAutoStyles);
}

void XMLTextParagraphExport::exportFrameFrames(
		sal_Bool bAutoStyles,
		sal_Bool bIsProgress,
		const Reference < XTextFrame > *pParentTxtFrame )
{
    const TextContentSet* const pTexts = pBoundFrameSets->GetTexts()->GetFrameBoundContents(*pParentTxtFrame);
    if(pTexts)
        for(TextContentSet::const_iterator_t it = pTexts->getBegin();
            it != pTexts->getEnd();
            ++it)
            exportTextFrame(*it, bAutoStyles, bIsProgress, sal_True);
    const TextContentSet* const pGraphics = pBoundFrameSets->GetGraphics()->GetFrameBoundContents(*pParentTxtFrame);
    if(pGraphics)
        for(TextContentSet::const_iterator_t it = pGraphics->getBegin();
            it != pGraphics->getEnd();
            ++it)
            exportTextGraphic(*it, bAutoStyles);
    const TextContentSet* const pEmbeddeds = pBoundFrameSets->GetEmbeddeds()->GetFrameBoundContents(*pParentTxtFrame);
    if(pEmbeddeds)
        for(TextContentSet::const_iterator_t it = pEmbeddeds->getBegin();
            it != pEmbeddeds->getEnd();
            ++it)
            exportTextEmbedded(*it, bAutoStyles);
    const TextContentSet* const pShapes = pBoundFrameSets->GetShapes()->GetFrameBoundContents(*pParentTxtFrame);
    if(pShapes)
        for(TextContentSet::const_iterator_t it = pShapes->getBegin();
            it != pShapes->getEnd();
            ++it)
            exportShape(*it, bAutoStyles);
}

// bookmarks, reference marks (and TOC marks) are the same except for the
// element names. We use the same method for export and it an array with
// the proper element names
static const enum XMLTokenEnum lcl_XmlReferenceElements[] = {
    XML_REFERENCE_MARK, XML_REFERENCE_MARK_START, XML_REFERENCE_MARK_END };
static const enum XMLTokenEnum lcl_XmlBookmarkElements[] = {
    XML_BOOKMARK, XML_BOOKMARK_START, XML_BOOKMARK_END };

// This function replaces the text portion iteration during auto style
// collection.
bool XMLTextParagraphExport::collectTextAutoStylesOptimized( sal_Bool bIsProgress )
{
	GetExport().GetShapeExport(); // make sure the graphics styles family is added

    const sal_Bool bAutoStyles = sal_True;
    const sal_Bool bExportContent = sal_False;

    // Export AutoStyles:
    Reference< XAutoStylesSupplier > xAutoStylesSupp( GetExport().GetModel(), UNO_QUERY );
    if ( xAutoStylesSupp.is() )
    {
        Reference< XAutoStyles > xAutoStyleFamilies = xAutoStylesSupp->getAutoStyles();
        OUString sName;
        sal_uInt16 nFamily;

        for ( int i = 0; i < 3; ++i )
        {
            if ( 0 == i )
            {
                sName = OUString( RTL_CONSTASCII_USTRINGPARAM( "CharacterStyles" ) );
                nFamily = XML_STYLE_FAMILY_TEXT_TEXT;
            }
            else if ( 1 == i )
            {
                sName = OUString( RTL_CONSTASCII_USTRINGPARAM( "RubyStyles" ) );
                nFamily = XML_STYLE_FAMILY_TEXT_RUBY;
            }
            else
            {
                sName = OUString( RTL_CONSTASCII_USTRINGPARAM( "ParagraphStyles" ) );
                nFamily = XML_STYLE_FAMILY_TEXT_PARAGRAPH;
            }

            Any aAny = xAutoStyleFamilies->getByName( sName );
            Reference< XAutoStyleFamily > xAutoStyles = *(Reference<XAutoStyleFamily>*)aAny.getValue();
            Reference < XEnumeration > xAutoStylesEnum( xAutoStyles->createEnumeration() );

            while ( xAutoStylesEnum->hasMoreElements() )
            {
                aAny = xAutoStylesEnum->nextElement();
                Reference< XAutoStyle > xAutoStyle = *(Reference<XAutoStyle>*)aAny.getValue();
                Reference < XPropertySet > xPSet( xAutoStyle, uno::UNO_QUERY );
                Add( nFamily, xPSet, 0, true );
            }
        }
    }

    // Export Field AutoStyles:
    Reference< XTextFieldsSupplier > xTextFieldsSupp( GetExport().GetModel(), UNO_QUERY );
    if ( xTextFieldsSupp.is() )
    {
        Reference< XEnumerationAccess > xTextFields = xTextFieldsSupp->getTextFields();
        Reference < XEnumeration > xTextFieldsEnum( xTextFields->createEnumeration() );

        while ( xTextFieldsEnum->hasMoreElements() )
        {
            Any aAny = xTextFieldsEnum->nextElement();
            Reference< XTextField > xTextField = *(Reference<XTextField>*)aAny.getValue();
            exportTextField( xTextField, bAutoStyles, bIsProgress,
                !xAutoStylesSupp.is() );
			try
			{
				Reference < XPropertySet > xSet( xTextField, UNO_QUERY );
				Reference < XText > xText;
				Any a = xSet->getPropertyValue( ::rtl::OUString::createFromAscii("TextRange") );
				a >>= xText;
				if ( xText.is() )
                {
					exportText( xText, sal_True, bIsProgress, bExportContent );
                    GetExport().GetTextParagraphExport()
                        ->collectTextAutoStyles( xText );
                }
			}
			catch (Exception&)
			{
			}
        }
    }

    // Export text frames:
    Reference<XEnumeration> xTextFramesEnum = pBoundFrameSets->GetTexts()->createEnumeration();
    if(xTextFramesEnum.is())
        while(xTextFramesEnum->hasMoreElements())
        {
            Reference<XTextContent> xTxtCntnt(xTextFramesEnum->nextElement(), UNO_QUERY);
            if(xTxtCntnt.is())
                exportTextFrame(xTxtCntnt, bAutoStyles, bIsProgress, bExportContent, 0);
        }

    // Export graphic objects:
    Reference<XEnumeration> xGraphicsEnum = pBoundFrameSets->GetGraphics()->createEnumeration();
    if(xGraphicsEnum.is())
        while(xGraphicsEnum->hasMoreElements())
        {
            Reference<XTextContent> xTxtCntnt(xGraphicsEnum->nextElement(), UNO_QUERY);
            if(xTxtCntnt.is())
                exportTextGraphic(xTxtCntnt, true, 0);
        }

    // Export embedded objects:
    Reference<XEnumeration> xEmbeddedsEnum = pBoundFrameSets->GetEmbeddeds()->createEnumeration();
    if(xEmbeddedsEnum.is())
        while(xEmbeddedsEnum->hasMoreElements())
        {
            Reference<XTextContent> xTxtCntnt(xEmbeddedsEnum->nextElement(), UNO_QUERY);
            if(xTxtCntnt.is())
                exportTextEmbedded(xTxtCntnt, true, 0);
        }

    // Export shapes:
    Reference<XEnumeration> xShapesEnum = pBoundFrameSets->GetShapes()->createEnumeration();
    if(xShapesEnum.is())
        while(xShapesEnum->hasMoreElements())
        {
            Reference<XTextContent> xTxtCntnt(xShapesEnum->nextElement(), UNO_QUERY);
            if(xTxtCntnt.is())
            {
                Reference<XServiceInfo> xServiceInfo(xTxtCntnt, UNO_QUERY);
                if( xServiceInfo->supportsService(sShapeService))
                    exportShape(xTxtCntnt, true, 0);
            }
        }

    sal_Int32 nCount;
    // AutoStyles for sections
    Reference< XTextSectionsSupplier > xSectionsSupp( GetExport().GetModel(), UNO_QUERY );
    if ( xSectionsSupp.is() )
    {
        Reference< XIndexAccess > xSections( xSectionsSupp->getTextSections(), UNO_QUERY );
        if ( xSections.is() )
        {
            nCount = xSections->getCount();
            for( sal_Int32 i = 0; i < nCount; ++i )
            {
                Any aAny = xSections->getByIndex( i );
                Reference< XTextSection > xSection = *(Reference<XTextSection>*)aAny.getValue();
                Reference < XPropertySet > xPSet( xSection, uno::UNO_QUERY );
                Add( XML_STYLE_FAMILY_TEXT_SECTION, xPSet );
            }
        }
    }

    // AutoStyles for tables (Note: suppress autostyle collection for paragraphs in exportTable)
    Reference< XTextTablesSupplier > xTablesSupp( GetExport().GetModel(), UNO_QUERY );
    if ( xTablesSupp.is() )
    {
        Reference< XIndexAccess > xTables( xTablesSupp->getTextTables(), UNO_QUERY );
        if ( xTables.is() )
        {
            nCount = xTables->getCount();
            for( sal_Int32 i = 0; i < nCount; ++i )
            {
                Any aAny = xTables->getByIndex( i );
                Reference< XTextTable > xTable = *(Reference<XTextTable>*)aAny.getValue();
                Reference < XTextContent > xTextContent( xTable, uno::UNO_QUERY );
                exportTable( xTextContent, sal_True, sal_True );
            }
        }
    }

    Reference< XNumberingRulesSupplier > xNumberingRulesSupp( GetExport().GetModel(), UNO_QUERY );
    if ( xNumberingRulesSupp.is() )
    {
        Reference< XIndexAccess > xNumberingRules = xNumberingRulesSupp->getNumberingRules();
        nCount = xNumberingRules->getCount();
        // --> OD 2007-01-12 #i73361#
        const OUString sNumberingIsOutline( RTL_CONSTASCII_USTRINGPARAM( "NumberingIsOutline" ) );
        // <--
        for( sal_Int32 i = 0; i < nCount; ++i )
        {
            Reference< XIndexReplace > xNumRule( xNumberingRules->getByIndex( i ), UNO_QUERY );
            if( xNumRule.is() && xNumRule->getCount() )
            {
                Reference < XNamed > xNamed( xNumRule, UNO_QUERY );
                OUString sName;
                if( xNamed.is() )
                    sName = xNamed->getName();
                sal_Bool bAdd = !sName.getLength();
                if( !bAdd )
                {
                    Reference < XPropertySet > xNumPropSet( xNumRule,
                                                            UNO_QUERY );
                    const OUString sIsAutomatic( RTL_CONSTASCII_USTRINGPARAM( "IsAutomatic" ) );
                    if( xNumPropSet.is() &&
                        xNumPropSet->getPropertySetInfo()
                                   ->hasPropertyByName( sIsAutomatic ) )
                    {
                        bAdd = *(sal_Bool *)xNumPropSet->getPropertyValue( sIsAutomatic ).getValue();
                        // --> OD 2007-01-12 #i73361# - check on outline style
                        if ( bAdd &&
                             xNumPropSet->getPropertySetInfo()
                                       ->hasPropertyByName( sNumberingIsOutline ) )
                        {
                            bAdd = !(*(sal_Bool *)xNumPropSet->getPropertyValue( sNumberingIsOutline ).getValue());
                        }
                        // <--
                    }
                    else
                    {
                        bAdd = sal_True;
                    }
                }
                if( bAdd )
                    pListAutoPool->Add( xNumRule );
            }
        }
    }

    return true;
}

void XMLTextParagraphExport::exportText(
		const Reference < XText > & rText,
		sal_Bool bAutoStyles,
        sal_Bool bIsProgress,
		sal_Bool bExportParagraph )
{
	if( bAutoStyles )
		GetExport().GetShapeExport(); // make sure the graphics styles family
									  // is added
	Reference < XEnumerationAccess > xEA( rText, UNO_QUERY );
	Reference < XEnumeration > xParaEnum(xEA->createEnumeration());
	Reference < XPropertySet > xPropertySet( rText, UNO_QUERY );
	Reference < XTextSection > xBaseSection;

	// #97718# footnotes don't supply paragraph enumerations in some cases
	// This is always a bug, but at least we don't want to crash.
	DBG_ASSERT( xParaEnum.is(), "We need a paragraph enumeration" );
	if( ! xParaEnum.is() )
		return;

	sal_Bool bExportLevels = sal_True;

	if (xPropertySet.is())
	{
		Reference < XPropertySetInfo > xInfo ( xPropertySet->getPropertySetInfo() );

		if( xInfo.is() )
		{
			if (xInfo->hasPropertyByName( sTextSection ))
			{
				xPropertySet->getPropertyValue(sTextSection) >>= xBaseSection ;
			}

/* #i35937#
			// for applications that use the outliner we need to check if
			// the current text object needs the level information exported
			if( !bAutoStyles )
			{
				// fixme: move string to class member, couldn't do now because
				//		  of no incompatible build
				OUString sHasLevels( RTL_CONSTASCII_USTRINGPARAM("HasLevels") );
				if (xInfo->hasPropertyByName( sHasLevels ) )
				{
					xPropertySet->getPropertyValue(sHasLevels) >>= bExportLevels;
				}
			}
*/
		}
	}

    // #96530# Export redlines at start & end of XText before & after
    // exporting the text content enumeration
    if( !bAutoStyles && (pRedlineExport != NULL) )
        pRedlineExport->ExportStartOrEndRedline( xPropertySet, sal_True );
	exportTextContentEnumeration( xParaEnum, bAutoStyles, xBaseSection,
                                  bIsProgress, bExportParagraph, 0, bExportLevels );
    if( !bAutoStyles && (pRedlineExport != NULL) )
        pRedlineExport->ExportStartOrEndRedline( xPropertySet, sal_False );
}

void XMLTextParagraphExport::exportText(
		const Reference < XText > & rText,
		const Reference < XTextSection > & rBaseSection,
		sal_Bool bAutoStyles,
		sal_Bool bIsProgress,
		sal_Bool bExportParagraph )
{
	if( bAutoStyles )
		GetExport().GetShapeExport(); // make sure the graphics styles family
									  // is added
	Reference < XEnumerationAccess > xEA( rText, UNO_QUERY );
	Reference < XEnumeration > xParaEnum(xEA->createEnumeration());

    // #98165# don't continue without a paragraph enumeration
    if( ! xParaEnum.is() )
        return;

    // #96530# Export redlines at start & end of XText before & after
    // exporting the text content enumeration
    Reference<XPropertySet> xPropertySet;
    if( !bAutoStyles && (pRedlineExport != NULL) )
    {
        xPropertySet.set(rText, uno::UNO_QUERY );
        pRedlineExport->ExportStartOrEndRedline( xPropertySet, sal_True );
    }
	exportTextContentEnumeration( xParaEnum, bAutoStyles, rBaseSection,
							      bIsProgress, bExportParagraph );
    if( !bAutoStyles && (pRedlineExport != NULL) )
        pRedlineExport->ExportStartOrEndRedline( xPropertySet, sal_False );
}

sal_Bool XMLTextParagraphExport::exportTextContentEnumeration(
		const Reference < XEnumeration > & rContEnum,
		sal_Bool bAutoStyles,
		const Reference < XTextSection > & rBaseSection,
		sal_Bool bIsProgress,
		sal_Bool bExportParagraph,
		const Reference < XPropertySet > *pRangePropSet,
        sal_Bool bExportLevels )
{
	DBG_ASSERT( rContEnum.is(), "No enumeration to export!" );
	sal_Bool bHasMoreElements = rContEnum->hasMoreElements();
	if( !bHasMoreElements )
		return sal_False;

	XMLTextNumRuleInfo aPrevNumInfo;
	XMLTextNumRuleInfo aNextNumInfo;

    sal_Bool bHasContent = sal_False;
	Reference<XTextSection> xCurrentTextSection(rBaseSection);

    MultiPropertySetHelper aPropSetHelper(
		   					bAutoStyles ? aParagraphPropertyNamesAuto :
										  aParagraphPropertyNames );

	sal_Bool bHoldElement = sal_False;
	Reference < XTextContent > xTxtCntnt;
	while( bHoldElement || bHasMoreElements )
	{
		if (bHoldElement)
		{
			bHoldElement = sal_False;
		}
		else
		{
            xTxtCntnt.set(rContEnum->nextElement(), uno::UNO_QUERY);

			aPropSetHelper.resetValues();

		}

		Reference<XServiceInfo> xServiceInfo( xTxtCntnt, UNO_QUERY );
		if( xServiceInfo->supportsService( sParagraphService ) )
		{
			if( bExportLevels )
			{
				if( bAutoStyles )
				{
					exportListAndSectionChange( xCurrentTextSection, xTxtCntnt,
												aPrevNumInfo, aNextNumInfo,
												bAutoStyles );
				}
				else
				{
                    // --> OD 2006-09-27 #i69627#
                    // --> OD 2008-04-24 #refactorlists#
                    // pass list auto style pool to <XMLTextNumRuleInfo> instance
                    // --> OD 2008-11-26 #158694#
                    // pass info about request to export <text:number> element
                    // to <XMLTextNumRuleInfo> instance
                    aNextNumInfo.Set( xTxtCntnt,
                                      GetExport().writeOutlineStyleAsNormalListStyle(),
                                      GetListAutoStylePool(),
                                      GetExport().exportTextNumberElement() );
                    // <--

					exportListAndSectionChange( xCurrentTextSection, aPropSetHelper,
												TEXT_SECTION, xTxtCntnt,
												aPrevNumInfo, aNextNumInfo,
												bAutoStyles );
				}
			}

			// if we found a mute section: skip all section content
			if (pSectionExport->IsMuteSection(xCurrentTextSection))
			{
				// Make sure headings are exported anyway.
				if( !bAutoStyles )
					pSectionExport->ExportMasterDocHeadingDummies();

				while (rContEnum->hasMoreElements() &&
					   pSectionExport->IsInSection( xCurrentTextSection,
													xTxtCntnt, sal_True ))
				{
                    xTxtCntnt.set(rContEnum->nextElement(), uno::UNO_QUERY);
					aPropSetHelper.resetValues();
					aNextNumInfo.Reset();
				}
				// the first non-mute element still needs to be processed
				bHoldElement =
					! pSectionExport->IsInSection( xCurrentTextSection,
												   xTxtCntnt, sal_False );
			}
			else
                exportParagraph( xTxtCntnt, bAutoStyles, bIsProgress,
                                 bExportParagraph, aPropSetHelper );
            bHasContent = sal_True;
		}
		else if( xServiceInfo->supportsService( sTableService ) )
		{
			if( !bAutoStyles )
			{
				aNextNumInfo.Reset();
			}

			exportListAndSectionChange( xCurrentTextSection, xTxtCntnt,
										aPrevNumInfo, aNextNumInfo,
										bAutoStyles );

			if (! pSectionExport->IsMuteSection(xCurrentTextSection))
			{
				// export start + end redlines (for wholly redlined tables)
				if ((! bAutoStyles) && (NULL != pRedlineExport))
					pRedlineExport->ExportStartOrEndRedline(xTxtCntnt, sal_True);

				exportTable( xTxtCntnt, bAutoStyles, bIsProgress  );

				if ((! bAutoStyles) && (NULL != pRedlineExport))
					pRedlineExport->ExportStartOrEndRedline(xTxtCntnt, sal_False);
			}
			else if( !bAutoStyles )
			{
				// Make sure headings are exported anyway.
				pSectionExport->ExportMasterDocHeadingDummies();
			}

			bHasContent = sal_True;
		}
		else if( xServiceInfo->supportsService( sTextFrameService ) )
		{
            exportTextFrame( xTxtCntnt, bAutoStyles, bIsProgress, sal_True, pRangePropSet );
        }
		else if( xServiceInfo->supportsService( sTextGraphicService ) )
		{
			exportTextGraphic( xTxtCntnt, bAutoStyles, pRangePropSet );
		}
		else if( xServiceInfo->supportsService( sTextEmbeddedService ) )
		{
			exportTextEmbedded( xTxtCntnt, bAutoStyles, pRangePropSet );
		}
		else if( xServiceInfo->supportsService( sShapeService ) )
		{
			exportShape( xTxtCntnt, bAutoStyles, pRangePropSet );
		}
		else
		{
			DBG_ASSERT( !xTxtCntnt.is(), "unknown text content" );
		}

		if( !bAutoStyles )
		{
			aPrevNumInfo = aNextNumInfo;
		}

		bHasMoreElements = rContEnum->hasMoreElements();
	}

	if( bExportLevels && bHasContent && !bAutoStyles )
	{
		aNextNumInfo.Reset();

		// close open lists and sections; no new styles
		exportListAndSectionChange( xCurrentTextSection, rBaseSection,
									aPrevNumInfo, aNextNumInfo,
									bAutoStyles );
	}

	return sal_True;
}

void XMLTextParagraphExport::exportParagraph(
		const Reference < XTextContent > & rTextContent,
        sal_Bool bAutoStyles, sal_Bool bIsProgress, sal_Bool bExportParagraph,
        MultiPropertySetHelper& rPropSetHelper)
{
	sal_Int16 nOutlineLevel = -1;

	if( bIsProgress )
	{
		ProgressBarHelper *pProgress = GetExport().GetProgressBarHelper();
		pProgress->SetValue( pProgress->GetValue()+1 );
	}

	// get property set or multi property set and initialize helper
	Reference<XMultiPropertySet> xMultiPropSet( rTextContent, UNO_QUERY );
	Reference<XPropertySet> xPropSet( rTextContent, UNO_QUERY );

	// check for supported properties
    if( !rPropSetHelper.checkedProperties() )
		rPropSetHelper.hasProperties( xPropSet->getPropertySetInfo() );

//	if( xMultiPropSet.is() )
//		rPropSetHelper.getValues( xMultiPropSet );
//	else
//		rPropSetHelper.getValues( xPropSet );

	if( bExportParagraph )
	{
		if( bAutoStyles )
		{
			Add( XML_STYLE_FAMILY_TEXT_PARAGRAPH, rPropSetHelper, xPropSet );
		}
		else
		{
            // xml:id for RDF metadata
            GetExport().AddAttributeXmlId(rTextContent);
            GetExport().AddAttributesRDFa(rTextContent);

			OUString sStyle;
			if( rPropSetHelper.hasProperty( PARA_STYLE_NAME ) )
			{
				if( xMultiPropSet.is() )
					rPropSetHelper.getValue( PARA_STYLE_NAME,
													xMultiPropSet ) >>= sStyle;
				else
					rPropSetHelper.getValue( PARA_STYLE_NAME,
													xPropSet ) >>= sStyle;
			}

			Reference< XInterface > xRef( rTextContent, UNO_QUERY );
			if( xRef.is() )
			{
				const OUString& rIdentifier = GetExport().getInterfaceToIdentifierMapper().getIdentifier( xRef );
				if( rIdentifier.getLength() )
                {
                    // FIXME: this is just temporary until EditEngine
                    // paragraphs implement XMetadatable.
                    // then that must be used and not the mapper, because
                    // when both can be used we get two xml:id!
                    uno::Reference<rdf::XMetadatable> const xMeta(xRef,
                        uno::UNO_QUERY);
                    OSL_ENSURE(!xMeta.is(), "paragraph that implements "
                        "XMetadatable used in interfaceToIdentifierMapper?");
                    GetExport().AddAttributeIdLegacy(XML_NAMESPACE_TEXT,
                        rIdentifier);
                }
			}

			OUString sAutoStyle( sStyle );
			sAutoStyle = Find( XML_STYLE_FAMILY_TEXT_PARAGRAPH, xPropSet, sStyle );
			if( sAutoStyle.getLength() )
				GetExport().AddAttribute( XML_NAMESPACE_TEXT, XML_STYLE_NAME,
							  GetExport().EncodeStyleName( sAutoStyle ) );

			if( rPropSetHelper.hasProperty( PARA_CONDITIONAL_STYLE_NAME ) )
			{
				OUString sCondStyle;
				if( xMultiPropSet.is() )
					rPropSetHelper.getValue( PARA_CONDITIONAL_STYLE_NAME,
												 	xMultiPropSet ) >>= sCondStyle;
				else
					rPropSetHelper.getValue( PARA_CONDITIONAL_STYLE_NAME,
												 	xPropSet ) >>= sCondStyle;
				if( sCondStyle != sStyle )
				{
					sCondStyle = Find( XML_STYLE_FAMILY_TEXT_PARAGRAPH, xPropSet,
								   	   sCondStyle );
					if( sCondStyle.getLength() )
						GetExport().AddAttribute( XML_NAMESPACE_TEXT,
												  XML_COND_STYLE_NAME,
							  GetExport().EncodeStyleName( sCondStyle ) );
				}
			}

			//if( rPropSetHelper.hasProperty( PARA_CHAPTER_NUMERBING_LEVEL ) )	//#outline level,zhaojianwei
			if( rPropSetHelper.hasProperty( PARA_OUTLINE_LEVEL ) )				//<-end
			{
				if( xMultiPropSet.is() )
					//rPropSetHelper.getValue( PARA_CHAPTER_NUMERBING_LEVEL,	//#outline level,zhaojianwei
					rPropSetHelper.getValue( PARA_OUTLINE_LEVEL,				//<-end
												 	xMultiPropSet ) >>= nOutlineLevel;
				else
					//rPropSetHelper.getValue( PARA_CHAPTER_NUMERBING_LEVEL,	//#outline level,zhaojianwei
					rPropSetHelper.getValue( PARA_OUTLINE_LEVEL,				//<-end
												 	xPropSet ) >>= nOutlineLevel;

				//if( -1 != nOutlineLevel )	//#outline level,zhaojianwei
				if( 0 < nOutlineLevel )	//<-end,zhaojianwei
				{
					OUStringBuffer sTmp;
					//sTmp.append( sal_Int32( nOutlineLevel + 1 ) );	//#outline level,zhaojianwei
					sTmp.append( sal_Int32( nOutlineLevel) );		//<-end,zhaojianwei
					GetExport().AddAttribute( XML_NAMESPACE_TEXT,
											  XML_OUTLINE_LEVEL,
								  sTmp.makeStringAndClear() );

                    if( rPropSetHelper.hasProperty( NUMBERING_IS_NUMBER ) )
                    {
                        bool bIsNumber = false;
                        if( xMultiPropSet.is() )
                            rPropSetHelper.getValue(
                                       NUMBERING_IS_NUMBER, xMultiPropSet ) >>= bIsNumber;
                        else
                            rPropSetHelper.getValue(
                                       NUMBERING_IS_NUMBER, xPropSet ) >>= bIsNumber;

                        OUString sListStyleName;
                        if( xMultiPropSet.is() )
                            rPropSetHelper.getValue(
                                       PARA_NUMBERING_STYLENAME, xMultiPropSet ) >>= sListStyleName;
                        else
                            rPropSetHelper.getValue(
                                       PARA_NUMBERING_STYLENAME, xPropSet ) >>= sListStyleName;



				        bool bAssignedtoOutlineStyle = false;//#outline level,add by zhaojianwei
						{
                            Reference< XChapterNumberingSupplier > xCNSupplier( GetExport().GetModel(), UNO_QUERY );

                            OUString sOutlineName;
                            if (xCNSupplier.is())
                            {
                                Reference< XIndexReplace > xNumRule ( xCNSupplier->getChapterNumberingRules() );
                                DBG_ASSERT( xNumRule.is(), "no chapter numbering rules" );

                                if (xNumRule.is())
                                {
                                    Reference< XPropertySet > xNumRulePropSet( xNumRule, UNO_QUERY );
                                    xNumRulePropSet->getPropertyValue(
										OUString(RTL_CONSTASCII_USTRINGPARAM("Name")) ) >>= sOutlineName;
                                    bAssignedtoOutlineStyle = ( sListStyleName == sOutlineName );
                                }
							}

						}	//<end,zhaojianwei

						//if( ! bIsNumber )			//#outline level,removed by zhaojianwei
						if( ! bIsNumber && bAssignedtoOutlineStyle )	//#outline level,add by zhaojianwei
                            GetExport().AddAttribute( XML_NAMESPACE_TEXT,
                                                      XML_IS_LIST_HEADER,
                                                      XML_TRUE );
                    }

                    {
                        String sParaIsNumberingRestart
                            (RTL_CONSTASCII_USTRINGPARAM
                             ("ParaIsNumberingRestart"));
                        bool bIsRestartNumbering = false;

                        Reference< XPropertySetInfo >
                        xPropSetInfo(xMultiPropSet.is() ?
                                     xMultiPropSet->getPropertySetInfo():
                                     xPropSet->getPropertySetInfo());

                        if (xPropSetInfo->
                            hasPropertyByName(sParaIsNumberingRestart))
                        {
                            xPropSet->getPropertyValue(sParaIsNumberingRestart)
                                >>= bIsRestartNumbering;
                        }

                        if (bIsRestartNumbering)
                        {
                            GetExport().AddAttribute(XML_NAMESPACE_TEXT,
                                                     XML_RESTART_NUMBERING,
                                                     XML_TRUE);

                            String sNumberingStartValue
                                (RTL_CONSTASCII_USTRINGPARAM
                                 ("NumberingStartValue"));


                            sal_Int32 nStartValue = 0;

                            if (xPropSetInfo->
                                hasPropertyByName(sNumberingStartValue))
                            {
                                xPropSet->getPropertyValue(sNumberingStartValue)
                                    >>= nStartValue;

                                OUStringBuffer sTmpStartValue;

                                sTmpStartValue.append(nStartValue);

                                GetExport().
                                    AddAttribute(XML_NAMESPACE_TEXT,
                                                 XML_START_VALUE,
                                                 sTmpStartValue.
                                                 makeStringAndClear());
                            }
                        }
                    }
				}
			}
		}
	}

	Reference < XEnumerationAccess > xEA( rTextContent, UNO_QUERY );
	Reference < XEnumeration > xTextEnum;
    xTextEnum = xEA->createEnumeration();
    const sal_Bool bHasPortions = xTextEnum.is();

	Reference < XEnumeration> xContentEnum;
	Reference < XContentEnumerationAccess > xCEA( rTextContent, UNO_QUERY );
	if( xCEA.is() )
		xContentEnum.set(xCEA->createContentEnumeration( sTextContentService ));
	const sal_Bool bHasContentEnum = xContentEnum.is() &&
	   	                             xContentEnum->hasMoreElements();

	Reference < XTextSection > xSection;
	if( bHasContentEnum )
	{
		// For the auto styles, the multi property set helper is only used
		// if hard attributes are existing. Therfor, it seems to be a better
		// strategy to have the TextSection property seperate, because otherwise
		// we always retrieve the style names even if they are not required.
		if( bAutoStyles )
		{
			if( xPropSet->getPropertySetInfo()->hasPropertyByName( sTextSection ) )
			{
                xSection.set(xPropSet->getPropertyValue( sTextSection ), uno::UNO_QUERY);
			}
		}
		else
		{
			if( rPropSetHelper.hasProperty( TEXT_SECTION ) )
			{
                xSection.set(rPropSetHelper.getValue( TEXT_SECTION ), uno::UNO_QUERY);
			}
		}
	}

	if( bAutoStyles )
	{
		sal_Bool bPrevCharIsSpace = sal_True;
		if( bHasContentEnum )
			bPrevCharIsSpace = !exportTextContentEnumeration(
									xContentEnum, bAutoStyles, xSection,
                                    bIsProgress, sal_True, 0, sal_True );
		if ( bHasPortions )
            exportTextRangeEnumeration( xTextEnum, bAutoStyles, bIsProgress );
    }
	else
	{
		sal_Bool bPrevCharIsSpace = sal_True;
		enum XMLTokenEnum eElem =
			//-1 == nOutlineLevel ? XML_P : XML_H;	//#outline level,zhaojianwei
			0 < nOutlineLevel ? XML_H : XML_P;	//<-end,zhaojianwei
		SvXMLElementExport aElem( GetExport(), XML_NAMESPACE_TEXT, eElem,
								  sal_True, sal_False );
		if( bHasContentEnum )
			bPrevCharIsSpace = !exportTextContentEnumeration(
									xContentEnum, bAutoStyles, xSection,
									bIsProgress );
		exportTextRangeEnumeration( xTextEnum, bAutoStyles, bIsProgress,
								 	bPrevCharIsSpace );
	}
}

void XMLTextParagraphExport::exportTextRangeEnumeration(
    const Reference < XEnumeration > & rTextEnum,
    sal_Bool bAutoStyles,
    sal_Bool bIsProgress,
    sal_Bool bPrvChrIsSpc )
{
    static OUString sMeta(RTL_CONSTASCII_USTRINGPARAM("InContentMetadata"));
    static OUString sAnnotation(RTL_CONSTASCII_USTRINGPARAM("Annotation"));
    static OUString sAnnotationEnd(RTL_CONSTASCII_USTRINGPARAM("AnnotationEnd"));

    sal_Bool bPrevCharIsSpace = bPrvChrIsSpc;

    while( rTextEnum->hasMoreElements() )
    {
        Reference<XPropertySet> xPropSet(rTextEnum->nextElement(), UNO_QUERY);
        Reference < XTextRange > xTxtRange(xPropSet, uno::UNO_QUERY);
        Reference<XPropertySetInfo> xPropInfo(xPropSet->getPropertySetInfo());

        if (xPropInfo->hasPropertyByName(sTextPortionType))
        {
            rtl::OUString sType;
            xPropSet->getPropertyValue(sTextPortionType) >>= sType;

            if( sType.equals(sText))
            {
                exportTextRange( xTxtRange, bAutoStyles, bPrevCharIsSpace );
            }
            else if( sType.equals(sTextField))
            {
                exportTextField( xTxtRange, bAutoStyles, bIsProgress );
                bPrevCharIsSpace = false;
            }
            else if ( sType.equals( sAnnotation ) )
            {
                exportTextField( xTxtRange, bAutoStyles, bIsProgress );
                bPrevCharIsSpace = false;
            }
            else if ( sType.equals( sAnnotationEnd ) )
            {
                Reference<XNamed> xBookmark(xPropSet->getPropertyValue(sBookmark), UNO_QUERY);
                const OUString& rName = xBookmark->getName();
                if ( rName.getLength() > 0 )
                {
                    GetExport().AddAttribute(XML_NAMESPACE_OFFICE, XML_NAME, rName);
                }
                SvXMLElementExport aElem( GetExport(), !bAutoStyles, XML_NAMESPACE_OFFICE, XML_ANNOTATION_END, sal_False, sal_False );
            }
            else if( sType.equals( sFrame ) )
            {
                Reference < XEnumeration> xContentEnum;
                Reference < XContentEnumerationAccess > xCEA( xTxtRange, UNO_QUERY );
                if( xCEA.is() )
                    xContentEnum.set(xCEA->createContentEnumeration( sTextContentService ));
                // frames are never in sections
                Reference<XTextSection> xSection;
                if( xContentEnum.is() )
                    exportTextContentEnumeration(
                    xContentEnum,
                    bAutoStyles,
                    xSection,
                    bIsProgress,
                    sal_True,
                    &xPropSet );

                bPrevCharIsSpace = sal_False;
            }
            else if (sType.equals(sFootnote))
            {
                exportTextFootnote( xPropSet, xTxtRange->getString(), bAutoStyles, bIsProgress );
                bPrevCharIsSpace = sal_False;
            }
            else if (sType.equals(sBookmark))
            {
                exportTextMark( xPropSet, sBookmark, lcl_XmlBookmarkElements, bAutoStyles );
            }
            else if (sType.equals(sReferenceMark))
            {
                exportTextMark( xPropSet, sReferenceMark, lcl_XmlReferenceElements, bAutoStyles);
            }
            else if (sType.equals(sDocumentIndexMark))
            {
                pIndexMarkExport->ExportIndexMark( xPropSet, bAutoStyles);
            }
            else if (sType.equals(sRedline))
            {
                if (NULL != pRedlineExport)
                    pRedlineExport->ExportChange( xPropSet, bAutoStyles );
            }
            else if (sType.equals(sRuby))
            {
                exportRuby( xPropSet, bAutoStyles );
            }
            else if (sType.equals(sMeta))
            {
                exportMeta( xPropSet, bAutoStyles, bIsProgress );
            }
            else if (sType.equals(sTextFieldStart))
            {
                Reference<XNamed> xBookmark(xPropSet->getPropertyValue(sBookmark), UNO_QUERY);
                if (xBookmark.is())
                {
                    GetExport().AddAttribute(XML_NAMESPACE_TEXT, XML_NAME, xBookmark->getName());
                }

                Reference< ::com::sun::star::text::XFormField > xFormField(xPropSet->getPropertyValue(sBookmark), UNO_QUERY);
                if (xFormField.is())
                {
                    GetExport().AddAttribute(XML_NAMESPACE_FIELD, XML_TYPE, xFormField->getFieldType());
                }

                GetExport().StartElement(XML_NAMESPACE_FIELD, XML_FIELDMARK_START, sal_False);
                if (xFormField.is())
                {
                    FieldParamExporter(&GetExport(), xFormField->getParameters()).Export();
                }
                GetExport().EndElement(XML_NAMESPACE_FIELD, XML_FIELDMARK_START, sal_False);
            }
            else if (sType.equals(sTextFieldEnd))
            {
                GetExport().StartElement(XML_NAMESPACE_FIELD, XML_FIELDMARK_END, sal_False);
                GetExport().EndElement(XML_NAMESPACE_FIELD, XML_FIELDMARK_END, sal_False);
            }
            else if (sType.equals(sTextFieldStartEnd))
            {
                Reference<XNamed> xBookmark(xPropSet->getPropertyValue(sBookmark), UNO_QUERY);
                if (xBookmark.is())
                {
                    GetExport().AddAttribute(XML_NAMESPACE_TEXT, XML_NAME, xBookmark->getName());
                }
                Reference< ::com::sun::star::text::XFormField > xFormField(xPropSet->getPropertyValue(sBookmark), UNO_QUERY);
                if (xFormField.is())
                {
                    GetExport().AddAttribute(XML_NAMESPACE_FIELD, XML_TYPE, xFormField->getFieldType());
                }
                GetExport().StartElement(XML_NAMESPACE_FIELD, XML_FIELDMARK, sal_False);
                if (xFormField.is())
                {
                    FieldParamExporter(&GetExport(), xFormField->getParameters()).Export();
                }
                GetExport().EndElement(XML_NAMESPACE_FIELD, XML_FIELDMARK, sal_False);
            }
            else if (sType.equals(sSoftPageBreak))
            {
                exportSoftPageBreak(xPropSet,	bAutoStyles);
            }
            else {
                DBG_ERROR("unknown text portion type");
            }
        }
        else
        {
            Reference<XServiceInfo> xServiceInfo( xTxtRange, UNO_QUERY );
            if( xServiceInfo->supportsService( sTextFieldService ) )
            {
                exportTextField( xTxtRange, bAutoStyles, bIsProgress );
                bPrevCharIsSpace = sal_False;
            }
            else
            {
                // no TextPortionType property -> non-Writer app -> text
                exportTextRange( xTxtRange, bAutoStyles, bPrevCharIsSpace );
            }
        }
    }

// now that there are nested enumerations for meta(-field), this may be valid!
//	DBG_ASSERT( !bOpenRuby, "Red Alert: Ruby still open!" );
}

void XMLTextParagraphExport::exportTable(
		const Reference < XTextContent > &,
		sal_Bool /*bAutoStyles*/, sal_Bool /*bIsProgress*/ )
{
}

void XMLTextParagraphExport::exportTextField(
		const Reference < XTextRange > & rTextRange,
        sal_Bool bAutoStyles, sal_Bool bIsProgress )
{
	Reference < XPropertySet > xPropSet( rTextRange, UNO_QUERY );
	// non-Writer apps need not support Property TextField, so test first
	if (xPropSet->getPropertySetInfo()->hasPropertyByName( sTextField ))
	{
        Reference < XTextField > xTxtFld(xPropSet->getPropertyValue( sTextField ), uno::UNO_QUERY);
		DBG_ASSERT( xTxtFld.is(), "text field missing" );
		if( xTxtFld.is() )
		{
            exportTextField(xTxtFld, bAutoStyles, bIsProgress, sal_True);
		}
		else
		{
			// write only characters
			GetExport().Characters(rTextRange->getString());
		}
	}
}

void XMLTextParagraphExport::exportTextField(
        const Reference < XTextField > & xTextField,
        const sal_Bool bAutoStyles, const sal_Bool bIsProgress,
        const sal_Bool bRecursive )
{
    if ( bAutoStyles )
    {
        pFieldExport->ExportFieldAutoStyle( xTextField, bIsProgress,
                bRecursive );
    }
    else
    {
        pFieldExport->ExportField( xTextField, bIsProgress );
    }
}

void XMLTextParagraphExport::exportSoftPageBreak(
	const Reference<XPropertySet> & ,
	sal_Bool )
{
    SvXMLElementExport aElem( GetExport(), XML_NAMESPACE_TEXT,
                              XML_SOFT_PAGE_BREAK, sal_False,
                              sal_False );
}

void XMLTextParagraphExport::exportTextMark(
	const Reference<XPropertySet> & rPropSet,
	const OUString sProperty,
	const enum XMLTokenEnum pElements[],
	sal_Bool bAutoStyles)
{
	// mib said: "Hau wech!"
	//
	// (Originally, I'd export a span element in case the (book|reference)mark
	//	was formatted. This actually makes a difference in case some pervert
	//	sets a point reference mark in the document and, say, formats it bold.
	//  This basically meaningless formatting will now been thrown away
	//	(aka cleaned up), since mib said: ...					dvo

 	if (!bAutoStyles)
	{
		// name element
		Reference<XNamed> xName(rPropSet->getPropertyValue(sProperty), UNO_QUERY);
		GetExport().AddAttribute(XML_NAMESPACE_TEXT, XML_NAME,
								 xName->getName());

		// start, end, or point-reference?
		sal_Int8 nElement;
		if( *(sal_Bool *)rPropSet->getPropertyValue(sIsCollapsed).getValue() )
		{
			nElement = 0;
		}
		else
		{
			nElement = *(sal_Bool *)rPropSet->getPropertyValue(sIsStart).getValue() ? 1 : 2;
		}

        // bookmark, bookmark-start: xml:id and RDFa for RDF metadata
        if( nElement < 2 ) {
            GetExport().AddAttributeXmlId(xName);
            const uno::Reference<text::XTextContent> xTextContent(
                    xName, uno::UNO_QUERY_THROW);
            GetExport().AddAttributesRDFa(xTextContent);
        }

		// export element
		DBG_ASSERT(pElements != NULL, "illegal element array");
		DBG_ASSERT(nElement >= 0, "illegal element number");
		DBG_ASSERT(nElement <= 2, "illegal element number");
		SvXMLElementExport aElem(GetExport(),
								 XML_NAMESPACE_TEXT, pElements[nElement],
								 sal_False, sal_False);
	}
	// else: no styles. (see above)
}

sal_Bool lcl_txtpara_isBoundAsChar(
		const Reference < XPropertySet > & rPropSet,
		const Reference < XPropertySetInfo > & rPropSetInfo )
{
	sal_Bool bIsBoundAsChar = sal_False;
	OUString sAnchorType( RTL_CONSTASCII_USTRINGPARAM( "AnchorType" ) );
	if( rPropSetInfo->hasPropertyByName( sAnchorType ) )
	{
		TextContentAnchorType eAnchor;
		rPropSet->getPropertyValue( sAnchorType ) >>= eAnchor;
		bIsBoundAsChar = TextContentAnchorType_AS_CHARACTER == eAnchor;
	}

	return bIsBoundAsChar;
}

sal_Int32 XMLTextParagraphExport::addTextFrameAttributes(
	const Reference < XPropertySet >& rPropSet,
	sal_Bool bShape,
	OUString *pMinHeightValue )
{
	sal_Int32 nShapeFeatures = SEF_DEFAULT;

	// draw:name (#97662#: not for shapes, since those names will be
	// treated in the shape export)
    if( !bShape )
    {
        Reference < XNamed > xNamed( rPropSet, UNO_QUERY );
        if( xNamed.is() )
        {
            OUString sName( xNamed->getName() );
            if( sName.getLength() )
                GetExport().AddAttribute( XML_NAMESPACE_DRAW, XML_NAME,
                                          xNamed->getName() );
        }
    }

	OUStringBuffer sValue;

	// text:anchor-type
	TextContentAnchorType eAnchor = TextContentAnchorType_AT_PARAGRAPH;
	rPropSet->getPropertyValue( sAnchorType ) >>= eAnchor;
	{
		XMLAnchorTypePropHdl aAnchorTypeHdl;
		OUString sTmp;
        aAnchorTypeHdl.exportXML( sTmp, uno::makeAny(eAnchor),
								  GetExport().GetMM100UnitConverter() );
		GetExport().AddAttribute( XML_NAMESPACE_TEXT, XML_ANCHOR_TYPE, sTmp );
	}

	// text:anchor-page-number
	if( TextContentAnchorType_AT_PAGE == eAnchor )
	{
		sal_Int16 nPage = 0;
		rPropSet->getPropertyValue( sAnchorPageNo ) >>= nPage;
		GetExport().GetMM100UnitConverter().convertNumber( sValue,
														   (sal_Int32)nPage );
		GetExport().AddAttribute( XML_NAMESPACE_TEXT, XML_ANCHOR_PAGE_NUMBER,
								  sValue.makeStringAndClear() );
	}
	else
	{
		// #92210#
		nShapeFeatures |= SEF_EXPORT_NO_WS;
	}

    // OD 2004-06-01 #i27691# - correction: no export of svg:x, if object
    // is anchored as-character.
    if ( !bShape &&
         eAnchor != TextContentAnchorType_AS_CHARACTER )
	{
		// svg:x
		sal_Int16 nHoriOrient =  HoriOrientation::NONE;
		rPropSet->getPropertyValue( sHoriOrient ) >>= nHoriOrient;
		if( HoriOrientation::NONE == nHoriOrient )
		{
			sal_Int32 nPos = 0;
			rPropSet->getPropertyValue( sHoriOrientPosition ) >>= nPos;
			GetExport().GetMM100UnitConverter().convertMeasure( sValue, nPos );
			GetExport().AddAttribute( XML_NAMESPACE_SVG, XML_X,
									  sValue.makeStringAndClear() );
		}
	}
	else if( TextContentAnchorType_AS_CHARACTER == eAnchor )
		nShapeFeatures = (nShapeFeatures & ~SEF_EXPORT_X);

	if( !bShape || TextContentAnchorType_AS_CHARACTER == eAnchor  )
	{
		// svg:y
		sal_Int16 nVertOrient =  VertOrientation::NONE;
		rPropSet->getPropertyValue( sVertOrient ) >>= nVertOrient;
		if( VertOrientation::NONE == nVertOrient )
		{
			sal_Int32 nPos = 0;
			rPropSet->getPropertyValue( sVertOrientPosition ) >>= nPos;
			GetExport().GetMM100UnitConverter().convertMeasure( sValue, nPos );
			GetExport().AddAttribute( XML_NAMESPACE_SVG, XML_Y,
									  sValue.makeStringAndClear() );
		}
		if( bShape )
			nShapeFeatures = (nShapeFeatures & ~SEF_EXPORT_Y);
	}


	Reference< XPropertySetInfo > xPropSetInfo(rPropSet->getPropertySetInfo());

	// svg:width
	sal_Int16 nWidthType = SizeType::FIX;
	if( xPropSetInfo->hasPropertyByName( sWidthType ) )
	{
		rPropSet->getPropertyValue( sWidthType ) >>= nWidthType;
	}
	if( xPropSetInfo->hasPropertyByName( sWidth ) )
	{
		sal_Int32 nWidth =  0;
        // VAR size will be written as zero min-size
        if( SizeType::VARIABLE != nWidthType )
        {
    		rPropSet->getPropertyValue( sWidth ) >>= nWidth;
        }
		GetExport().GetMM100UnitConverter().convertMeasure( sValue, nWidth );
		if( SizeType::FIX != nWidthType )
			GetExport().AddAttribute( XML_NAMESPACE_FO, XML_MIN_WIDTH,
									  sValue.makeStringAndClear() );
		else
            GetExport().AddAttribute( XML_NAMESPACE_SVG, XML_WIDTH,
                                      sValue.makeStringAndClear() );
	}
	sal_Bool bSyncWidth = sal_False;
	if( xPropSetInfo->hasPropertyByName( sIsSyncWidthToHeight ) )
	{
		bSyncWidth = *(sal_Bool *)rPropSet->getPropertyValue( sIsSyncWidthToHeight ).getValue();
		if( bSyncWidth )
			GetExport().AddAttribute( XML_NAMESPACE_STYLE, XML_REL_WIDTH,
                                      XML_SCALE );
	}
	if( !bSyncWidth && xPropSetInfo->hasPropertyByName( sRelativeWidth ) )
	{
		sal_Int16 nRelWidth =  0;
		rPropSet->getPropertyValue( sRelativeWidth ) >>= nRelWidth;
		DBG_ASSERT( nRelWidth >= 0 && nRelWidth <= 254,
					"Got illegal relative width from API" );
		if( nRelWidth > 0 )
		{
			GetExport().GetMM100UnitConverter().convertPercent( sValue,
																nRelWidth );
			GetExport().AddAttribute( XML_NAMESPACE_STYLE, XML_REL_WIDTH,
									  sValue.makeStringAndClear() );
		}
	}

	// svg:height, fo:min-height or style:rel-height
	sal_Int16 nSizeType = SizeType::FIX;
	if( xPropSetInfo->hasPropertyByName( sSizeType ) )
	{
		rPropSet->getPropertyValue( sSizeType ) >>= nSizeType;
	}
	sal_Bool bSyncHeight = sal_False;
	if( xPropSetInfo->hasPropertyByName( sIsSyncHeightToWidth ) )
	{
		bSyncHeight = *(sal_Bool *)rPropSet->getPropertyValue( sIsSyncHeightToWidth ).getValue();
	}
	sal_Int16 nRelHeight =  0;
	if( !bSyncHeight && xPropSetInfo->hasPropertyByName( sRelativeHeight ) )
	{
		rPropSet->getPropertyValue( sRelativeHeight ) >>= nRelHeight;
	}
	if( xPropSetInfo->hasPropertyByName( sHeight ) )
	{
		sal_Int32 nHeight =  0;
        if( SizeType::VARIABLE != nSizeType )
        {
    		rPropSet->getPropertyValue( sHeight ) >>= nHeight;
        }
		GetExport().GetMM100UnitConverter().convertMeasure( sValue,
															nHeight );
		if( SizeType::FIX != nSizeType && 0==nRelHeight && !bSyncHeight &&
		 	pMinHeightValue )
			*pMinHeightValue = sValue.makeStringAndClear();
		else
			GetExport().AddAttribute( XML_NAMESPACE_SVG, XML_HEIGHT,
									  sValue.makeStringAndClear() );
	}
	if( bSyncHeight )
	{
		GetExport().AddAttribute( XML_NAMESPACE_STYLE, XML_REL_HEIGHT,
				SizeType::MIN == nSizeType ? XML_SCALE_MIN : XML_SCALE );

	}
	else if( nRelHeight > 0 )
	{
		GetExport().GetMM100UnitConverter().convertPercent( sValue,
															nRelHeight );
		if( SizeType::MIN == nSizeType )
			GetExport().AddAttribute( XML_NAMESPACE_FO, XML_MIN_HEIGHT,
									  sValue.makeStringAndClear() );
		else
			GetExport().AddAttribute( XML_NAMESPACE_STYLE, XML_REL_HEIGHT,
									  sValue.makeStringAndClear() );
	}

	OUString sZOrder( RTL_CONSTASCII_USTRINGPARAM( "ZOrder" ) );
	if( xPropSetInfo->hasPropertyByName( sZOrder ) )
	{
		sal_Int32 nZIndex = 0;
		rPropSet->getPropertyValue( sZOrder ) >>= nZIndex;
		if( -1 != nZIndex )
		{
			GetExport().GetMM100UnitConverter().convertNumber( sValue,
																nZIndex );
			GetExport().AddAttribute( XML_NAMESPACE_DRAW, XML_ZINDEX,
									  sValue.makeStringAndClear() );
		}
	}

	return nShapeFeatures;
}

void XMLTextParagraphExport::exportAnyTextFrame(
		const Reference < XTextContent > & rTxtCntnt,
		FrameType eType,
		sal_Bool bAutoStyles,
        sal_Bool bIsProgress,
        sal_Bool bExportContent,
        const Reference < XPropertySet > *pRangePropSet)
{
	Reference < XPropertySet > xPropSet( rTxtCntnt, UNO_QUERY );

	if( bAutoStyles )
	{
		if( FT_EMBEDDED == eType )
			_collectTextEmbeddedAutoStyles( xPropSet );
        // --> OD 2004-08-09 #i28745# - no text frame style for shapes
        else if ( FT_SHAPE != eType )
			Add( XML_STYLE_FAMILY_TEXT_FRAME, xPropSet );
        // <--

		if( pRangePropSet && lcl_txtpara_isBoundAsChar( xPropSet,
											xPropSet->getPropertySetInfo() ) )
			Add( XML_STYLE_FAMILY_TEXT_TEXT, *pRangePropSet );

		switch( eType )
		{
		case FT_TEXT:
			{
				// frame bound frames
                if ( bExportContent )
                {
    				Reference < XTextFrame > xTxtFrame( rTxtCntnt, UNO_QUERY );
	    			Reference < XText > xTxt(xTxtFrame->getText());
                    exportFrameFrames( sal_True, bIsProgress, &xTxtFrame );
                    exportText( xTxt, bAutoStyles, bIsProgress, sal_True );
                }
            }
			break;
		case FT_SHAPE:
			{
				Reference < XShape > xShape( rTxtCntnt, UNO_QUERY );
				GetExport().GetShapeExport()->collectShapeAutoStyles( xShape );
			}
			break;
		default:
			break;
		}
	}
	else
	{
		Reference< XPropertySetInfo > xPropSetInfo(xPropSet->getPropertySetInfo());
		Reference< XPropertyState > xPropState( xPropSet, UNO_QUERY );
		{
			sal_Bool bAddCharStyles = pRangePropSet &&
				lcl_txtpara_isBoundAsChar( xPropSet, xPropSetInfo );

            sal_Bool bIsUICharStyle;
            sal_Bool bHasAutoStyle = sal_False;
            sal_Bool bDummy;

			OUString sStyle;

			if( bAddCharStyles )
		   		sStyle = FindTextStyleAndHyperlink( *pRangePropSet, bDummy, bIsUICharStyle, bHasAutoStyle );
			else
				bIsUICharStyle = sal_False;

			XMLTextCharStyleNamesElementExport aCharStylesExport(
				GetExport(), bIsUICharStyle &&
							 aCharStyleNamesPropInfoCache.hasProperty(
                                            *pRangePropSet ), bHasAutoStyle,
				*pRangePropSet, sCharStyleNames );

			if( sStyle.getLength() )
				GetExport().AddAttribute( XML_NAMESPACE_TEXT, XML_STYLE_NAME,
								  GetExport().EncodeStyleName( sStyle ) );
			{
				SvXMLElementExport aElem( GetExport(), sStyle.getLength() > 0,
					XML_NAMESPACE_TEXT, XML_SPAN, sal_False, sal_False );
				{
					SvXMLElementExport aElement( GetExport(),
						FT_SHAPE != eType &&
						addHyperlinkAttributes( xPropSet,
												xPropState,xPropSetInfo ),
						XML_NAMESPACE_DRAW, XML_A, sal_False, sal_False );
					switch( eType )
					{
					case FT_TEXT:
						_exportTextFrame( xPropSet, xPropSetInfo, bIsProgress );
						break;
					case FT_GRAPHIC:
						_exportTextGraphic( xPropSet, xPropSetInfo );
						break;
					case FT_EMBEDDED:
						_exportTextEmbedded( xPropSet, xPropSetInfo );
						break;
					case FT_SHAPE:
						{
							Reference < XShape > xShape( rTxtCntnt, UNO_QUERY );
							sal_Int32 nFeatures =
								addTextFrameAttributes( xPropSet, sal_True );
							GetExport().GetShapeExport()
								->exportShape( xShape, nFeatures );
						}
						break;
					}
				}
			}
		}
	}
}

void XMLTextParagraphExport::_exportTextFrame(
		const Reference < XPropertySet > & rPropSet,
		const Reference < XPropertySetInfo > & rPropSetInfo,
		sal_Bool bIsProgress )
{
	Reference < XTextFrame > xTxtFrame( rPropSet, UNO_QUERY );
	Reference < XText > xTxt(xTxtFrame->getText());

	OUString sStyle;
	if( rPropSetInfo->hasPropertyByName( sFrameStyleName ) )
	{
		rPropSet->getPropertyValue( sFrameStyleName ) >>= sStyle;
	}

	OUString sAutoStyle( sStyle );
	OUString aMinHeightValue;
	sAutoStyle = Find( XML_STYLE_FAMILY_TEXT_FRAME, rPropSet, sStyle );
	if( sAutoStyle.getLength() )
		GetExport().AddAttribute( XML_NAMESPACE_DRAW, XML_STYLE_NAME,
							  GetExport().EncodeStyleName( sAutoStyle ) );
	addTextFrameAttributes( rPropSet, sal_False, &aMinHeightValue );

	SvXMLElementExport aElem( GetExport(), XML_NAMESPACE_DRAW,
							  XML_FRAME, sal_False, sal_True );

	if( aMinHeightValue.getLength() )
		GetExport().AddAttribute( XML_NAMESPACE_FO, XML_MIN_HEIGHT,
								  aMinHeightValue );

	// draw:chain-next-name
	if( rPropSetInfo->hasPropertyByName( sChainNextName ) )
	{
		OUString sNext;
		if( (rPropSet->getPropertyValue( sChainNextName ) >>= sNext) && sNext.getLength() > 0 )
			GetExport().AddAttribute( XML_NAMESPACE_DRAW,
									  XML_CHAIN_NEXT_NAME,
									  sNext );
	}

	{
		SvXMLElementExport aElement( GetExport(), XML_NAMESPACE_DRAW,
								  XML_TEXT_BOX, sal_True, sal_True );

		// frame bound frames
		exportFramesBoundToFrame( xTxtFrame, bIsProgress );

		exportText( xTxt, sal_False, bIsProgress, sal_True );
	}

	// script:events
	Reference<XEventsSupplier> xEventsSupp( xTxtFrame, UNO_QUERY );
	GetExport().GetEventExport().Export(xEventsSupp);

	// image map
	GetExport().GetImageMapExport().Export( rPropSet );

    // --> OD 2009-07-22 #i73249#
    // svg:title and svg:desc
    exportTitleAndDescription( rPropSet, rPropSetInfo );
    // <--
}

void XMLTextParagraphExport::exportContour(
    const Reference < XPropertySet > & rPropSet,
    const Reference < XPropertySetInfo > & rPropSetInfo )
{
    if( !rPropSetInfo->hasPropertyByName( sContourPolyPolygon ) )
    {
        return;
    }

    PointSequenceSequence aSourcePolyPolygon;
    rPropSet->getPropertyValue( sContourPolyPolygon ) >>= aSourcePolyPolygon;
    const basegfx::B2DPolyPolygon aPolyPolygon(
        basegfx::tools::UnoPointSequenceSequenceToB2DPolyPolygon(
            aSourcePolyPolygon));
    const sal_uInt32 nPolygonCount(aPolyPolygon.count());

    if(!nPolygonCount)
    {
        return;
    }

    const basegfx::B2DRange aPolyPolygonRange(aPolyPolygon.getB2DRange());
    bool bPixel(false);

    if( rPropSetInfo->hasPropertyByName( sIsPixelContour ) )
    {
        bPixel = *(sal_Bool *)rPropSet->getPropertyValue( sIsPixelContour ).getValue();
    }

    // svg: width
    OUStringBuffer aStringBuffer( 10 );

    if(bPixel)
    {
        GetExport().GetMM100UnitConverter().convertMeasurePx(aStringBuffer, basegfx::fround(aPolyPolygonRange.getWidth()));
    }
    else
    {
        GetExport().GetMM100UnitConverter().convertMeasure(aStringBuffer, basegfx::fround(aPolyPolygonRange.getWidth()));
    }

    GetExport().AddAttribute(XML_NAMESPACE_SVG, XML_WIDTH, aStringBuffer.makeStringAndClear());

    // svg: height
    if(bPixel)
    {
        GetExport().GetMM100UnitConverter().convertMeasurePx(aStringBuffer, basegfx::fround(aPolyPolygonRange.getHeight()));
    }
    else
    {
        GetExport().GetMM100UnitConverter().convertMeasure(aStringBuffer, basegfx::fround(aPolyPolygonRange.getHeight()));
    }

    GetExport().AddAttribute(XML_NAMESPACE_SVG, XML_HEIGHT, aStringBuffer.makeStringAndClear());

    // svg:viewbox
    SdXMLImExViewBox aViewBox(0.0, 0.0, aPolyPolygonRange.getWidth(), aPolyPolygonRange.getHeight());
    GetExport().AddAttribute(XML_NAMESPACE_SVG, XML_VIEWBOX, aViewBox.GetExportString());
    enum XMLTokenEnum eElem = XML_TOKEN_INVALID;

    if(1 == nPolygonCount )
    {
        // simple polygon shape, can be written as svg:points sequence
        const ::rtl::OUString aPointString(
            basegfx::tools::exportToSvgPoints(
                aPolyPolygon.getB2DPolygon(0)));

        // write point array
        GetExport().AddAttribute(XML_NAMESPACE_DRAW, XML_POINTS, aPointString);
        eElem = XML_CONTOUR_POLYGON;
    }
    else
    {
        // polypolygon, needs to be written as a svg:path sequence
        const ::rtl::OUString aPolygonString(
            basegfx::tools::exportToSvgD(
                aPolyPolygon,
                true,           // bUseRelativeCoordinates
                false,          // bDetectQuadraticBeziers: not used in old, but maybe activated now
                true));         // bHandleRelativeNextPointCompatible

        // write point array
        GetExport().AddAttribute( XML_NAMESPACE_SVG, XML_D, aPolygonString);
        eElem = XML_CONTOUR_PATH;
    }

    if( rPropSetInfo->hasPropertyByName( sIsAutomaticContour ) )
    {
        sal_Bool bTmp = *(sal_Bool *)rPropSet->getPropertyValue(
                                            sIsAutomaticContour ).getValue();
        GetExport().AddAttribute( XML_NAMESPACE_DRAW,
                      XML_RECREATE_ON_EDIT, bTmp ? XML_TRUE : XML_FALSE );
    }

    // write object now
    SvXMLElementExport aElem( GetExport(), XML_NAMESPACE_DRAW, eElem,
                              sal_True, sal_True );
}

void XMLTextParagraphExport::_exportTextGraphic(
		const Reference < XPropertySet > & rPropSet,
		const Reference < XPropertySetInfo > & rPropSetInfo )
{
	OUString sStyle;
	if( rPropSetInfo->hasPropertyByName( sFrameStyleName ) )
	{
		rPropSet->getPropertyValue( sFrameStyleName ) >>= sStyle;
	}

	OUString sAutoStyle( sStyle );
	sAutoStyle = Find( XML_STYLE_FAMILY_TEXT_FRAME, rPropSet, sStyle );
	if( sAutoStyle.getLength() )
		GetExport().AddAttribute( XML_NAMESPACE_DRAW, XML_STYLE_NAME,
								  GetExport().EncodeStyleName( sAutoStyle ) );
	addTextFrameAttributes( rPropSet, sal_False );

	// svg:transform
	sal_Int16 nVal = 0;
	rPropSet->getPropertyValue( sGraphicRotation ) >>= nVal;
	if( nVal != 0 )
	{
		OUStringBuffer sRet( GetXMLToken(XML_ROTATE).getLength()+4 );
		sRet.append( GetXMLToken(XML_ROTATE));
		sRet.append( (sal_Unicode)'(' );
		GetExport().GetMM100UnitConverter().convertNumber( sRet, (sal_Int32)nVal );
		sRet.append( (sal_Unicode)')' );
		GetExport().AddAttribute( XML_NAMESPACE_SVG, XML_TRANSFORM,
								  sRet.makeStringAndClear() );
	}

    // original content
    SvXMLElementExport aElem(GetExport(), XML_NAMESPACE_DRAW, XML_FRAME, sal_False, sal_True);

    // replacement graphic for backwards compatibility, but
    // only for SVG currently
    OUString sReplacementOrigURL;
    rPropSet->getPropertyValue( sReplacementGraphicURL ) >>= sReplacementOrigURL;

    if(sReplacementOrigURL.getLength())
    {
        const OUString sReplacementURL(GetExport().AddEmbeddedGraphicObject( sReplacementOrigURL ));

        // If there is no url, then then graphic is empty
        if(sReplacementURL.getLength())
        {
            GetExport().AddAttribute(XML_NAMESPACE_XLINK, XML_HREF, sReplacementURL);
            GetExport().AddAttribute(XML_NAMESPACE_XLINK, XML_TYPE, XML_SIMPLE);
            GetExport().AddAttribute(XML_NAMESPACE_XLINK, XML_SHOW, XML_EMBED);
            GetExport().AddAttribute(XML_NAMESPACE_XLINK, XML_ACTUATE, XML_ONLOAD);
            
            // xlink:href for replacement, only written for Svg content
            SvXMLElementExport aElement(GetExport(), XML_NAMESPACE_DRAW, XML_IMAGE, sal_False, sal_True);

            // optional office:binary-data
            GetExport().AddEmbeddedGraphicObjectAsBase64(sReplacementURL);
        }
    }

	// xlink:href
	OUString sOrigURL;
	rPropSet->getPropertyValue( sGraphicURL ) >>= sOrigURL;
	OUString sURL(GetExport().AddEmbeddedGraphicObject( sOrigURL ));
	setTextEmbeddedGraphicURL( rPropSet, sURL );

	// If there still is no url, then then graphic is empty
	if( sURL.getLength() )
	{
		GetExport().AddAttribute(XML_NAMESPACE_XLINK, XML_HREF, sURL );
		GetExport().AddAttribute( XML_NAMESPACE_XLINK, XML_TYPE, XML_SIMPLE );
		GetExport().AddAttribute( XML_NAMESPACE_XLINK, XML_SHOW, XML_EMBED );
		GetExport().AddAttribute( XML_NAMESPACE_XLINK, XML_ACTUATE,
                                                       XML_ONLOAD );
	}

	// draw:filter-name
	OUString sGrfFilter;
	rPropSet->getPropertyValue( sGraphicFilter ) >>= sGrfFilter;
	if( sGrfFilter.getLength() )
		GetExport().AddAttribute( XML_NAMESPACE_DRAW, XML_FILTER_NAME,
								  sGrfFilter );

	{
		SvXMLElementExport aElement( GetExport(), XML_NAMESPACE_DRAW,
								  XML_IMAGE, sal_False, sal_True );

		// optional office:binary-data
		GetExport().AddEmbeddedGraphicObjectAsBase64( sOrigURL );
	}

	// script:events
	Reference<XEventsSupplier> xEventsSupp( rPropSet, UNO_QUERY );
	GetExport().GetEventExport().Export(xEventsSupp);

	// image map
	GetExport().GetImageMapExport().Export( rPropSet );

    // --> OD 2009-07-22 #i73249#
    // svg:title and svg:desc
    exportTitleAndDescription( rPropSet, rPropSetInfo );
    // <--

	// draw:contour
	exportContour( rPropSet, rPropSetInfo );
}

void XMLTextParagraphExport::_collectTextEmbeddedAutoStyles(const Reference < XPropertySet > & )
{
	DBG_ASSERT( !this, "no API implementation avialable" );
}

void XMLTextParagraphExport::_exportTextEmbedded(
		const Reference < XPropertySet > &,
		const Reference < XPropertySetInfo > & )
{
	DBG_ASSERT( !this, "no API implementation avialable" );
}

void XMLTextParagraphExport::exportEvents( const Reference < XPropertySet > & rPropSet )
{
	// script:events
	Reference<XEventsSupplier> xEventsSupp( rPropSet, UNO_QUERY );
	GetExport().GetEventExport().Export(xEventsSupp);

	// image map
	OUString sImageMap(RTL_CONSTASCII_USTRINGPARAM("ImageMap"));
	if (rPropSet->getPropertySetInfo()->hasPropertyByName(sImageMap))
		GetExport().GetImageMapExport().Export( rPropSet );
}

// --> OD 2009-07-22 #i73249#
void XMLTextParagraphExport::exportTitleAndDescription(
		const Reference < XPropertySet > & rPropSet,
		const Reference < XPropertySetInfo > & rPropSetInfo )
{
	// svg:title
    if( rPropSetInfo->hasPropertyByName( sTitle ) )
	{
        OUString sObjTitle;
        rPropSet->getPropertyValue( sTitle ) >>= sObjTitle;
        if( sObjTitle.getLength() )
		{
			SvXMLElementExport aElem( GetExport(), XML_NAMESPACE_SVG,
									  XML_TITLE, sal_True, sal_False );
            GetExport().Characters( sObjTitle );
		}
	}

    // svg:description
    if( rPropSetInfo->hasPropertyByName( sDescription ) )
    {
        OUString sObjDesc;
        rPropSet->getPropertyValue( sDescription ) >>= sObjDesc;
        if( sObjDesc.getLength() )
        {
            SvXMLElementExport aElem( GetExport(), XML_NAMESPACE_SVG,
                                      XML_DESC, sal_True, sal_False );
            GetExport().Characters( sObjDesc );
        }
    }
}
// <--

void XMLTextParagraphExport::setTextEmbeddedGraphicURL(
	const Reference < XPropertySet >&,
	OUString& /*rStreamName*/ ) const
{
}

sal_Bool XMLTextParagraphExport::addHyperlinkAttributes(
		const Reference < XPropertySet > & rPropSet,
		const Reference < XPropertyState > & rPropState,
		const Reference < XPropertySetInfo > & rPropSetInfo )
{
	sal_Bool bExport = sal_False;
	OUString sHRef, sName, sTargetFrame, sUStyleName, sVStyleName;
	sal_Bool bServerMap = sal_False;

/*    bool bHyperLinkURL = false;
    bool bHyperLinkName = false;
    bool bHyperLinkTarget = false;
    bool bServer = false;
    bool bUnvisitedCharStyleName = false;
    bool bVisitedCharStyleName = false;

    const Reference< XMultiPropertySet > xMultiPropertySet( rPropSet, UNO_QUERY );
    if ( xMultiPropertySet.is() )
    {
    sal_uInt32 nCount = 0;
    Sequence< OUString > aPropertyNames( 6 );
  	OUString* pArray = aPropertyNames.getArray();

    if ( rPropSetInfo->hasPropertyByName( sServerMap ) )
    {
        bServer = true;
        pArray[ nCount++ ] = sServerMap;
    }
    if ( rPropSetInfo->hasPropertyByName( sHyperLinkName ) )
    {
        bHyperLinkName = true;
        pArray[ nCount++ ] = sHyperLinkName;
    }
    if ( rPropSetInfo->hasPropertyByName( sHyperLinkTarget ) )
    {
        bHyperLinkTarget = true;
        pArray[ nCount++ ] = sHyperLinkTarget;
    }
    if ( rPropSetInfo->hasPropertyByName( sHyperLinkURL ) )
    {
        bHyperLinkURL = true;
        pArray[ nCount++ ] = sHyperLinkURL;
    }
    if ( rPropSetInfo->hasPropertyByName( sUnvisitedCharStyleName ) )
    {
        bUnvisitedCharStyleName = true;
        pArray[ nCount++ ] = sUnvisitedCharStyleName;
    }
    if ( rPropSetInfo->hasPropertyByName( sVisitedCharStyleName ) )
    {
        bVisitedCharStyleName = true;
        pArray[ nCount++ ] = sVisitedCharStyleName;
    }

    aPropertyNames.realloc( nCount );

    if ( nCount )
    {
        Sequence< PropertyState > aPropertyStates( nCount );
      	PropertyState* pStateArray = aPropertyStates.getArray();

        if ( rPropState.is() )
            aPropertyStates = rPropState->getPropertyStates( aPropertyNames );

        Sequence< Any > aPropertyValues ( xMultiPropertySet->getPropertyValues( aPropertyNames ) );
        Any* pValueArray = aPropertyValues.getArray();

        sal_uInt32 nIdx = 0;

        if ( bServer )
        {
            if ( !rPropState.is() || PropertyState_DIRECT_VALUE == pStateArray[ nIdx ] )
            {
                bServerMap = *(sal_Bool *)pValueArray[ nIdx ].getValue();
        		if( bServerMap  )
			        bExport = sal_True;
            }
            ++nIdx;
        }
        if ( bHyperLinkName )
        {
            if ( !rPropState.is() || PropertyState_DIRECT_VALUE == pStateArray[ nIdx ] )
            {
                pValueArray[ nIdx ] >>= sName;
        		if( sName.getLength() > 0 )
			        bExport = sal_True;
            }
            ++nIdx;
        }
        if ( bHyperLinkTarget )
        {
            if ( !rPropState.is() || PropertyState_DIRECT_VALUE == pStateArray[ nIdx ] )
            {
                pValueArray[ nIdx ] >>= sTargetFrame;
        		if( sTargetFrame.getLength() )
			        bExport = sal_True;
            }
            ++nIdx;
        }
        if ( bHyperLinkURL )
        {
            if ( !rPropState.is() || PropertyState_DIRECT_VALUE == pStateArray[ nIdx ] )
            {
                pValueArray[ nIdx ] >>= sHRef;
        		if( sHRef.getLength() > 0 )
		        	bExport = sal_True;
            }
            ++nIdx;
        }
        if ( bUnvisitedCharStyleName )
        {
            if ( !rPropState.is() || PropertyState_DIRECT_VALUE == pStateArray[ nIdx ] )
            {
                pValueArray[ nIdx ] >>= sUStyleName;
        		if( sUStyleName.getLength() )
			        bExport = sal_True;
            }
            ++nIdx;
        }
        if ( bVisitedCharStyleName )
        {
            if ( !rPropState.is() || PropertyState_DIRECT_VALUE == pStateArray[ nIdx ] )
            {
                pValueArray[ nIdx ] >>= sVStyleName;
        		if( sVStyleName.getLength() )
			        bExport = sal_True;
            }
            ++nIdx;
        }
    }
    }
    else
    {*/
	if( rPropSetInfo->hasPropertyByName( sHyperLinkURL ) &&
		( !rPropState.is() || PropertyState_DIRECT_VALUE ==
					rPropState->getPropertyState( sHyperLinkURL ) ) )
	{
		rPropSet->getPropertyValue( sHyperLinkURL ) >>= sHRef;

		if( sHRef.getLength() > 0 )
			bExport = sal_True;
	}

	if( rPropSetInfo->hasPropertyByName( sHyperLinkName ) &&
		( !rPropState.is() || PropertyState_DIRECT_VALUE ==
					rPropState->getPropertyState( sHyperLinkName ) ) )
	{
		rPropSet->getPropertyValue( sHyperLinkName ) >>= sName;
		if( sName.getLength() > 0 )
			bExport = sal_True;
	}

	if( rPropSetInfo->hasPropertyByName( sHyperLinkTarget ) &&
		( !rPropState.is() || PropertyState_DIRECT_VALUE ==
					rPropState->getPropertyState( sHyperLinkTarget ) ) )
	{
		rPropSet->getPropertyValue( sHyperLinkTarget ) >>= sTargetFrame;
		if( sTargetFrame.getLength() )
			bExport = sal_True;
	}

	if( rPropSetInfo->hasPropertyByName( sServerMap ) &&
		( !rPropState.is() || PropertyState_DIRECT_VALUE ==
					rPropState->getPropertyState( sServerMap ) ) )
	{
		bServerMap = *(sal_Bool *)rPropSet->getPropertyValue( sServerMap ).getValue();
		if( bServerMap  )
			bExport = sal_True;
	}

	if( rPropSetInfo->hasPropertyByName( sUnvisitedCharStyleName ) &&
		( !rPropState.is() || PropertyState_DIRECT_VALUE ==
			rPropState->getPropertyState( sUnvisitedCharStyleName ) ) )
	{
		rPropSet->getPropertyValue( sUnvisitedCharStyleName ) >>= sUStyleName;
		if( sUStyleName.getLength() )
			bExport = sal_True;
	}

	if( rPropSetInfo->hasPropertyByName( sVisitedCharStyleName ) &&
		( !rPropState.is() || PropertyState_DIRECT_VALUE ==
			rPropState->getPropertyState( sVisitedCharStyleName ) ) )
	{
		rPropSet->getPropertyValue( sVisitedCharStyleName ) >>= sVStyleName;
		if( sVStyleName.getLength() )
			bExport = sal_True;
	}

	if( bExport )
	{
		GetExport().AddAttribute( XML_NAMESPACE_XLINK, XML_TYPE, XML_SIMPLE );
		GetExport().AddAttribute( XML_NAMESPACE_XLINK, XML_HREF, GetExport().GetRelativeReference( sHRef ) );

		if( sName.getLength() > 0 )
			GetExport().AddAttribute( XML_NAMESPACE_OFFICE, XML_NAME, sName );

		if( sTargetFrame.getLength() )
		{
			GetExport().AddAttribute( XML_NAMESPACE_OFFICE,
									  XML_TARGET_FRAME_NAME, sTargetFrame );
			enum XMLTokenEnum eTok =
				sTargetFrame.equalsAsciiL( "_blank", sizeof("_blank")-1 )
					? XML_NEW : XML_REPLACE;
			GetExport().AddAttribute( XML_NAMESPACE_XLINK, XML_SHOW, eTok );
		}

		if( bServerMap  )
			GetExport().AddAttribute( XML_NAMESPACE_OFFICE,
                                      XML_SERVER_MAP, XML_TRUE );

		if( sUStyleName.getLength() )
			GetExport().AddAttribute( XML_NAMESPACE_TEXT,
			  XML_STYLE_NAME, GetExport().EncodeStyleName( sUStyleName ) );

		if( sVStyleName.getLength() )
			GetExport().AddAttribute( XML_NAMESPACE_TEXT,
			  XML_VISITED_STYLE_NAME, GetExport().EncodeStyleName( sVStyleName ) );
	}

	return bExport;
}

void XMLTextParagraphExport::exportTextRange(
		const Reference < XTextRange > & rTextRange,
		sal_Bool bAutoStyles,
		sal_Bool& rPrevCharIsSpace )
{
	Reference < XPropertySet > xPropSet( rTextRange, UNO_QUERY );
	if( bAutoStyles )
	{
		Add( XML_STYLE_FAMILY_TEXT_TEXT, xPropSet );
	}
	else
	{
		sal_Bool bHyperlink = sal_False;
        sal_Bool bIsUICharStyle = sal_False;
        sal_Bool bHasAutoStyle = sal_False;

        OUString sStyle(FindTextStyleAndHyperlink( xPropSet, bHyperlink,
			   										 bIsUICharStyle, bHasAutoStyle ));

        Reference < XPropertySetInfo > xPropSetInfo;
		if( bHyperlink )
		{
			Reference< XPropertyState > xPropState( xPropSet, UNO_QUERY );
			xPropSetInfo.set(xPropSet->getPropertySetInfo());
			bHyperlink = addHyperlinkAttributes( xPropSet, xPropState, xPropSetInfo );
		}
		SvXMLElementExport aElem( GetExport(), bHyperlink, XML_NAMESPACE_TEXT,
								  XML_A, sal_False, sal_False );
		if( bHyperlink )
		{
			// export events (if supported)
			OUString sHyperLinkEvents(RTL_CONSTASCII_USTRINGPARAM(
				"HyperLinkEvents"));
			if (xPropSetInfo->hasPropertyByName(sHyperLinkEvents))
			{
                Reference<XNameReplace> xName(xPropSet->getPropertyValue(sHyperLinkEvents), uno::UNO_QUERY);
				GetExport().GetEventExport().Export(xName, sal_False);
			}
		}

		{
			XMLTextCharStyleNamesElementExport aCharStylesExport(
				GetExport(), bIsUICharStyle &&
							 aCharStyleNamesPropInfoCache.hasProperty(
                                                    xPropSet, xPropSetInfo ), bHasAutoStyle,
				xPropSet, sCharStyleNames );

			OUString aText(rTextRange->getString());
			if( sStyle.getLength() )
				GetExport().AddAttribute( XML_NAMESPACE_TEXT, XML_STYLE_NAME,
						  GetExport().EncodeStyleName( sStyle ) );
			{
				// in a block to make sure it is destroyed before the text:a element
				SvXMLElementExport aElement( GetExport(), sStyle.getLength() > 0,
										  XML_NAMESPACE_TEXT, XML_SPAN, sal_False,
										  sal_False );
				exportText( aText, rPrevCharIsSpace );
			}
		}
	}
}

void XMLTextParagraphExport::exportText( const OUString& rText,
									  	 sal_Bool& rPrevCharIsSpace )
{
    sal_Int32 nExpStartPos = 0;
	sal_Int32 nEndPos = rText.getLength();
	sal_Int32 nSpaceChars = 0;
	for( sal_Int32 nPos = 0; nPos < nEndPos; nPos++ )
	{
		sal_Unicode cChar = rText[nPos];
		sal_Bool bExpCharAsText = sal_True;
		sal_Bool bExpCharAsElement = sal_False;
		sal_Bool bCurrCharIsSpace = sal_False;
		switch( cChar )
		{
		case 0x0009:	// Tab
		case 0x000A:	// LF
			// These characters are exported as text.
			bExpCharAsElement = sal_True;
			bExpCharAsText = sal_False;
			break;
		case 0x000D:
			break;	// legal character
		case 0x0020:	// Blank
			if( rPrevCharIsSpace )
			{
				// If the previous character is a space character,
				// too, export a special space element.
				bExpCharAsText = sal_False;
			}
			bCurrCharIsSpace = sal_True;
			break;
		default:
			if( cChar < 0x0020 )
			{
#ifdef DBG_UTIL
				OSL_ENSURE( txtparae_bContainsIllegalCharacters ||
							cChar >= 0x0020,
							"illegal character in text content" );
				txtparae_bContainsIllegalCharacters = sal_True;
#endif
				bExpCharAsText = sal_False;
			}
			break;
		}

		// If the current character is not exported as text
	   	// the text that has not been exported by now has to be exported now.
		if( nPos > nExpStartPos && !bExpCharAsText )
		{
			DBG_ASSERT( 0==nSpaceChars, "pending spaces" );
			OUString sExp( rText.copy( nExpStartPos, nPos - nExpStartPos ) );
			GetExport().Characters( sExp );
			nExpStartPos = nPos;
		}

		// If there are spaces left that have not been exported and the
		// current chracter is not a space , the pending spaces have to be
		// exported now.
		if( nSpaceChars > 0 && !bCurrCharIsSpace )
		{
			DBG_ASSERT( nExpStartPos == nPos, " pending characters" );

			if( nSpaceChars > 1 )
			{
				OUStringBuffer sTmp;
				sTmp.append( (sal_Int32)nSpaceChars );
				GetExport().AddAttribute( XML_NAMESPACE_TEXT, XML_C,
							  sTmp.makeStringAndClear() );
			}

			SvXMLElementExport aElem( GetExport(), XML_NAMESPACE_TEXT,
									  XML_S, sal_False, sal_False );

			nSpaceChars = 0;
		}

		// If the current character has to be exported as a special
		// element, the elemnt will be exported now.
		if( bExpCharAsElement )
		{
			switch( cChar )
			{
			case 0x0009:	// Tab
				{
					SvXMLElementExport aElem( GetExport(), XML_NAMESPACE_TEXT,
											  XML_TAB, sal_False,
											  sal_False );
				}
				break;
			case 0x000A:	// LF
				{
					SvXMLElementExport aElem( GetExport(), XML_NAMESPACE_TEXT,
											  XML_LINE_BREAK, sal_False,
											  sal_False );
				}
				break;
			}
		}

		// If the current character is a space, and the previous one
		// is a space, too, the number of pending spaces is incremented
		// only.
		if( bCurrCharIsSpace && rPrevCharIsSpace )
			nSpaceChars++;
		rPrevCharIsSpace = bCurrCharIsSpace;

		// If the currect character is not exported as text, the start
		// position for text is the position behind the current position.
		if( !bExpCharAsText )
		{
			DBG_ASSERT( nExpStartPos == nPos, "wrong export start pos" );
			nExpStartPos = nPos+1;
		}
	}

	if( nExpStartPos < nEndPos )
	{
		DBG_ASSERT( 0==nSpaceChars, " pending spaces " );
		OUString sExp( rText.copy( nExpStartPos, nEndPos - nExpStartPos ) );
		GetExport().Characters( sExp );
	}

	// If there are some spaces left, they have to be exported now.
	if( nSpaceChars > 0 )
	{
		if( nSpaceChars > 1 )
		{
			OUStringBuffer sTmp;
			sTmp.append( (sal_Int32)nSpaceChars );
			GetExport().AddAttribute( XML_NAMESPACE_TEXT, XML_C,
						  sTmp.makeStringAndClear() );
		}

		SvXMLElementExport aElem( GetExport(), XML_NAMESPACE_TEXT, XML_S,
								  sal_False, sal_False );
	}
}

void XMLTextParagraphExport::exportTextDeclarations()
{
	pFieldExport->ExportFieldDeclarations();

	// get XPropertySet from the document and ask for AutoMarkFileURL.
	// If it exists, export the auto-mark-file element.
	Reference<XPropertySet> xPropertySet( GetExport().GetModel(), UNO_QUERY );
	if (xPropertySet.is())
	{
		OUString sUrl;
		OUString sIndexAutoMarkFileURL(
			RTL_CONSTASCII_USTRINGPARAM("IndexAutoMarkFileURL"));
		if (xPropertySet->getPropertySetInfo()->hasPropertyByName(
			sIndexAutoMarkFileURL))
		{
			xPropertySet->getPropertyValue(sIndexAutoMarkFileURL) >>= sUrl;
			if (sUrl.getLength() > 0)
			{
				GetExport().AddAttribute( XML_NAMESPACE_XLINK, XML_HREF,
										  GetExport().GetRelativeReference(sUrl) );
				SvXMLElementExport aAutoMarkElement(
					GetExport(), XML_NAMESPACE_TEXT,
					XML_ALPHABETICAL_INDEX_AUTO_MARK_FILE,
					sal_True, sal_True );
			}
		}
	}
}

void XMLTextParagraphExport::exportTextDeclarations(
	const Reference<XText> & rText )
{
	pFieldExport->ExportFieldDeclarations(rText);
}

void XMLTextParagraphExport::exportUsedDeclarations( sal_Bool bOnlyUsed )
{
	pFieldExport->SetExportOnlyUsedFieldDeclarations( bOnlyUsed );
}


void XMLTextParagraphExport::exportTrackedChanges(sal_Bool bAutoStyles)
{
	if (NULL != pRedlineExport)
		pRedlineExport->ExportChangesList( bAutoStyles );
}

void XMLTextParagraphExport::exportTrackedChanges(
	const Reference<XText> & rText,
	sal_Bool bAutoStyle)
{
	if (NULL != pRedlineExport)
		pRedlineExport->ExportChangesList(rText, bAutoStyle);
}

void XMLTextParagraphExport::recordTrackedChangesForXText(
	const Reference<XText> & rText )
{
	if (NULL != pRedlineExport)
		pRedlineExport->SetCurrentXText(rText);
}

void XMLTextParagraphExport::recordTrackedChangesNoXText()
{
	if (NULL != pRedlineExport)
		pRedlineExport->SetCurrentXText();
}


void XMLTextParagraphExport::exportTextAutoStyles()
{
	GetAutoStylePool().exportXML( XML_STYLE_FAMILY_TEXT_PARAGRAPH,
								   GetExport().GetDocHandler(),
								   GetExport().GetMM100UnitConverter(),
								   GetExport().GetNamespaceMap() );

	GetAutoStylePool().exportXML( XML_STYLE_FAMILY_TEXT_TEXT,
								   GetExport().GetDocHandler(),
								   GetExport().GetMM100UnitConverter(),
								   GetExport().GetNamespaceMap() );

	GetAutoStylePool().exportXML( XML_STYLE_FAMILY_TEXT_FRAME,
								   GetExport().GetDocHandler(),
								   GetExport().GetMM100UnitConverter(),
								   GetExport().GetNamespaceMap() );

	GetAutoStylePool().exportXML( XML_STYLE_FAMILY_TEXT_SECTION,
								  GetExport().GetDocHandler(),
								  GetExport().GetMM100UnitConverter(),
								  GetExport().GetNamespaceMap() );

	GetAutoStylePool().exportXML( XML_STYLE_FAMILY_TEXT_RUBY,
								  GetExport().GetDocHandler(),
								  GetExport().GetMM100UnitConverter(),
								  GetExport().GetNamespaceMap() );

	pListAutoPool->exportXML();
}

void XMLTextParagraphExport::exportRuby(
	const Reference<XPropertySet> & rPropSet,
	sal_Bool bAutoStyles )
{
	// early out: a collapsed ruby makes no sense
	if (*(sal_Bool*)rPropSet->getPropertyValue(sIsCollapsed).getValue())
		return;

	// start value ?
	sal_Bool bStart = (*(sal_Bool*)rPropSet->getPropertyValue(sIsStart).getValue());

	if (bAutoStyles)
	{
		// ruby auto styles
		if (bStart)
			Add( XML_STYLE_FAMILY_TEXT_RUBY, rPropSet );
	}
	else
	{
		// prepare element names
		OUString aRuby(GetXMLToken(XML_RUBY));
		OUString sTextRuby(GetExport().GetNamespaceMap().
						   GetQNameByKey(XML_NAMESPACE_TEXT, aRuby));
		OUString sRubyBase(GetXMLToken(XML_RUBY_BASE));
		OUString sTextRubyBase(GetExport().GetNamespaceMap().
						   GetQNameByKey(XML_NAMESPACE_TEXT, sRubyBase));

		if (bStart)
		{
			// ruby start

			// we can only start a ruby if none is open
			DBG_ASSERT(! bOpenRuby, "Can't open a ruby inside of ruby!");
			if( bOpenRuby )
				return;

			// save ruby text + ruby char style
			rPropSet->getPropertyValue(sRubyText) >>= sOpenRubyText;
			rPropSet->getPropertyValue(sRubyCharStyleName) >>= sOpenRubyCharStyle;

			// ruby style
			GetExport().CheckAttrList();
			OUString sEmpty;
			OUString sStyleName(Find( XML_STYLE_FAMILY_TEXT_RUBY, rPropSet,
										sEmpty ));
			DBG_ASSERT(sStyleName.getLength() > 0, "I can't find the style!");
			GetExport().AddAttribute(XML_NAMESPACE_TEXT,
									 XML_STYLE_NAME, sStyleName);

			// export <text:ruby> and <text:ruby-base> start elements
			GetExport().StartElement( XML_NAMESPACE_TEXT, XML_RUBY, sal_False);
			GetExport().ClearAttrList();
			GetExport().StartElement( XML_NAMESPACE_TEXT, XML_RUBY_BASE,
                                      sal_False );
			bOpenRuby = sal_True;
		}
		else
		{
			// ruby end

			// check for an open ruby
			DBG_ASSERT(bOpenRuby, "Can't close a ruby if none is open!");
			if( !bOpenRuby )
				return;

			// close <text:ruby-base>
			GetExport().EndElement(XML_NAMESPACE_TEXT, XML_RUBY_BASE,
                                   sal_False);

			// write the ruby text (with char style)
			{
				if (sOpenRubyCharStyle.getLength() > 0)
					GetExport().AddAttribute(
						XML_NAMESPACE_TEXT, XML_STYLE_NAME,
        				GetExport().EncodeStyleName( sOpenRubyCharStyle) );

				SvXMLElementExport aRubyElement(
					GetExport(), XML_NAMESPACE_TEXT, XML_RUBY_TEXT,
					sal_False, sal_False);

				GetExport().Characters(sOpenRubyText);
			}

			// and finally, close the ruby
			GetExport().EndElement(XML_NAMESPACE_TEXT, XML_RUBY, sal_False);
			bOpenRuby = sal_False;
		}
	}
}

void XMLTextParagraphExport::exportMeta(
    const Reference<XPropertySet> & i_xPortion,
    sal_Bool i_bAutoStyles, sal_Bool i_isProgress)
{
    static OUString sMeta(RTL_CONSTASCII_USTRINGPARAM("InContentMetadata"));

    bool doExport(!i_bAutoStyles); // do not export element if autostyles
    // check version >= 1.2
    switch (GetExport().getDefaultVersion()) {
        case SvtSaveOptions::ODFVER_011: // fall thru
        case SvtSaveOptions::ODFVER_010: doExport = false; break;
        default: break;
    }

    const Reference< XTextContent > xTextContent(
            i_xPortion->getPropertyValue(sMeta), UNO_QUERY_THROW);
    const Reference< XEnumerationAccess > xEA( xTextContent, UNO_QUERY_THROW );
    const Reference< XEnumeration > xTextEnum( xEA->createEnumeration() );

    if (doExport)
    {
        const Reference<rdf::XMetadatable> xMeta(xTextContent, UNO_QUERY_THROW);

        // text:meta with neither xml:id nor RDFa is invalid
        xMeta->ensureMetadataReference();

        // xml:id and RDFa for RDF metadata
        GetExport().AddAttributeXmlId(xMeta);
        GetExport().AddAttributesRDFa(xTextContent);
    }

    SvXMLElementExport aElem( GetExport(), doExport,
        XML_NAMESPACE_TEXT, XML_META, sal_False, sal_False );

    // recurse to export content
    exportTextRangeEnumeration( xTextEnum, i_bAutoStyles, i_isProgress );
}


void XMLTextParagraphExport::PreventExportOfControlsInMuteSections(
    const Reference<XIndexAccess> & rShapes,
    UniReference<xmloff::OFormLayerXMLExport> xFormExport   )
{
    // check parameters ad pre-conditions
    if( ( ! rShapes.is() ) || ( ! xFormExport.is() ) )
    {
        // if we don't have shapes or a form export, there's nothing to do
        return;
    }
    DBG_ASSERT( pSectionExport != NULL, "We need the section export." );

    Reference<XEnumeration> xShapesEnum = pBoundFrameSets->GetShapes()->createEnumeration();
    if(!xShapesEnum.is())
        return;
    while( xShapesEnum->hasMoreElements() )
    {
        // now we need to check
        // 1) if this is a control shape, and
        // 2) if it's in a mute section
        // if both answers are 'yes', notify the form layer export

        // we join accessing the shape and testing for control
        Reference<XControlShape> xControlShape(xShapesEnum->nextElement(), UNO_QUERY);
        if( xControlShape.is() )
        {
            //            Reference<XPropertySet> xPropSet( xControlShape, UNO_QUERY );
            //            Reference<XTextContent> xTextContent;
            //            xPropSet->getPropertyValue( OUString( RTL_CONSTASCII_USTRINGPARAM( "TextRange" ) ) ) >>= xTextContent;

            Reference<XTextContent> xTextContent( xControlShape, UNO_QUERY );
            if( xTextContent.is() )
            {
                if( pSectionExport->IsMuteSection( xTextContent, sal_False ) )
                {
                    // Ah, we've found a shape that
                    // 1) is a control shape
                    // 2) is anchored in a mute section
                    // so: don't export it!
                    xFormExport->excludeFromExport(
                        xControlShape->getControl() );
                }
                // else: not in mute section -> should be exported -> nothing
                // to do
            }
            // else: no anchor -> ignore
        }
        // else: no control shape -> nothing to do
    }
}
sal_Int32 XMLTextParagraphExport::GetHeadingLevel( const OUString& rStyleName )
{
	if( !pHeadingStyles )
	{
		pHeadingStyles = new XMLStringVector;
		SvxXMLNumRuleExport::GetOutlineStyles( *pHeadingStyles,
											   GetExport().GetModel() );
	}
	for( XMLStringVector::size_type i=0; i < pHeadingStyles->size(); ++i )
	{
		if( (*pHeadingStyles)[i] == rStyleName )
			return static_cast < sal_Int32 >( i );
	}

	return -1;
}

// --> OD 2008-05-08 #refactorlists#
void XMLTextParagraphExport::PushNewTextListsHelper()
{
    mpTextListsHelper = new XMLTextListsHelper();
    maTextListsHelperStack.push_back( mpTextListsHelper );
}

void XMLTextParagraphExport::PopTextListsHelper()
{
    delete mpTextListsHelper;
    mpTextListsHelper = 0;
    maTextListsHelperStack.pop_back();
    if ( !maTextListsHelperStack.empty() )
    {
        mpTextListsHelper = maTextListsHelperStack.back();
    }
}
// <--
