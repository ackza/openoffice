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



#ifndef SC_XMLSTYLI_HXX
#define SC_XMLSTYLI_HXX

#include <rtl/ustring.hxx>
#include <vector>
#include <xmloff/xmlimp.hxx>
#include <xmloff/xmlictxt.hxx>
#include <xmloff/maptype.hxx>
#include <xmloff/prstylei.hxx>
#include <xmloff/xmlimppr.hxx>
#include <xmloff/XMLTextMasterPageContext.hxx>
#include <xmloff/XMLTextMasterStylesContext.hxx>
#include <xmloff/txtstyli.hxx>
#include <com/sun/star/sheet/ConditionOperator.hpp>
#include "xmlimprt.hxx"

class ScSheetSaveData;

class ScXMLCellImportPropertyMapper : public SvXMLImportPropertyMapper
{
protected:

public:

	ScXMLCellImportPropertyMapper(
			const UniReference< XMLPropertySetMapper >& rMapper,
            SvXMLImport& rImport);
	virtual ~ScXMLCellImportPropertyMapper();

	/** this method is called for every item that has the MID_FLAG_SPECIAL_ITEM_IMPORT flag set */
/*	virtual sal_Bool handleSpecialItem(
			XMLPropertyState& rProperty,
			::std::vector< XMLPropertyState >& rProperties,
			const ::rtl::OUString& rValue,
			const SvXMLUnitConverter& rUnitConverter,
			const SvXMLNamespaceMap& rNamespaceMap ) const;*/

	/** this method is called for every item that has the MID_FLAG_NO_ITEM_IMPORT flag set */
/*	virtual sal_Bool handleNoItem(
			sal_Int32 nIndex,
			::std::vector< XMLPropertyState >& rProperties,
		   	const ::rtl::OUString& rValue,
		   	const SvXMLUnitConverter& rUnitConverter,
		   	const SvXMLNamespaceMap& rNamespaceMap ) const;*/

	/** This method is called when all attributes have been processed. It may be used to remove items that are incomplete */
	virtual void finished(
			::std::vector< XMLPropertyState >& rProperties, sal_Int32 nStartIndex, sal_Int32 nEndIndex ) const;
};

class ScXMLRowImportPropertyMapper : public SvXMLImportPropertyMapper
{
protected:

public:

	ScXMLRowImportPropertyMapper(
			const UniReference< XMLPropertySetMapper >& rMapper,
            SvXMLImport& rImport);
	virtual ~ScXMLRowImportPropertyMapper();

	/** This method is called when all attributes have been processed. It may be used to remove items that are incomplete */
	virtual void finished(
			::std::vector< XMLPropertyState >& rProperties, sal_Int32 nStartIndex, sal_Int32 nEndIndex ) const;
};
struct ScXMLMapContent;

class XMLTableStyleContext : public XMLPropStyleContext
{
	::rtl::OUString				sDataStyleName;
	rtl::OUString				sPageStyle;
	const rtl::OUString			sNumberFormat;
	SvXMLStylesContext*			pStyles;
	std::vector<ScXMLMapContent>	aMaps;
	com::sun::star::uno::Any	aConditionalFormat;
	sal_Int32					nNumberFormat;
    sal_Int32                   nLastSheet;
	sal_Bool					bConditionalFormatCreated;
	sal_Bool					bParentSet;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

    void SetOperator(
            ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue >& rProps,
            ::com::sun::star::sheet::ConditionOperator eOp ) const;

    void SetBaseCellAddress(
            ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue >& rProps,
            const ::rtl::OUString& rBaseCell ) const;

    void SetStyle(
            ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue >& rProps,
            const ::rtl::OUString& rApplyStyle ) const;

    void SetFormula(
        ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue >& rProps,
        sal_Int32 nFormulaIdx, const ::rtl::OUString& rFormula,
        const ::rtl::OUString& rFormulaNmsp, ::formula::FormulaGrammar::Grammar eGrammar, bool bHasNmsp ) const;

	void GetConditionalFormat(
		::com::sun::star::uno::Any& aAny, const rtl::OUString& sCondition,
		const rtl::OUString& sApplyStyle, const rtl::OUString& sBaseCell) const;
protected:

	virtual void SetAttribute( sal_uInt16 nPrefixKey,
							   const ::rtl::OUString& rLocalName,
							   const ::rtl::OUString& rValue );

public:

	TYPEINFO();

	XMLTableStyleContext( ScXMLImport& rImport, sal_uInt16 nPrfx,
			const ::rtl::OUString& rLName,
			const ::com::sun::star::uno::Reference< ::com::sun::star::xml::sax::XAttributeList > & xAttrList,
			SvXMLStylesContext& rStyles, sal_uInt16 nFamily, sal_Bool bDefaultStyle = sal_False );
	virtual ~XMLTableStyleContext();

	virtual SvXMLImportContext *CreateChildContext(
			sal_uInt16 nPrefix,
			const ::rtl::OUString& rLocalName,
			const ::com::sun::star::uno::Reference< ::com::sun::star::xml::sax::XAttributeList > & xAttrList );

	virtual void FillPropertySet(const ::com::sun::star::uno::Reference<
				::com::sun::star::beans::XPropertySet > & rPropSet );

	virtual void SetDefaults();

  	void AddProperty(sal_Int16 nContextID, const com::sun::star::uno::Any& aValue);
    XMLPropertyState* FindProperty(const sal_Int16 nContextID);

	sal_Int32 GetNumberFormat();// { return nNumberFormat; }

    sal_Int32 GetLastSheet() const       { return nLastSheet; }
    void SetLastSheet(sal_Int32 nNew)    { nLastSheet = nNew; }

private:
    using XMLPropStyleContext::SetStyle;
};

class XMLTableStylesContext : public SvXMLStylesContext
{
	::com::sun::star::uno::Reference <
					::com::sun::star::container::XNameContainer > xCellStyles;
	::com::sun::star::uno::Reference <
					::com::sun::star::container::XNameContainer > xColumnStyles;
	::com::sun::star::uno::Reference <
					::com::sun::star::container::XNameContainer > xRowStyles;
	::com::sun::star::uno::Reference <
					::com::sun::star::container::XNameContainer > xTableStyles;
	const ::rtl::OUString sCellStyleServiceName;
	const ::rtl::OUString sColumnStyleServiceName;
	const ::rtl::OUString sRowStyleServiceName;
	const ::rtl::OUString sTableStyleServiceName;
	sal_Int32 nNumberFormatIndex;
	sal_Int32 nConditionalFormatIndex;
	sal_Int32 nCellStyleIndex;
	sal_Int32 nMasterPageNameIndex;
	sal_Bool bAutoStyles;

	UniReference < SvXMLImportPropertyMapper > xCellImpPropMapper;
	UniReference < SvXMLImportPropertyMapper > xColumnImpPropMapper;
	UniReference < SvXMLImportPropertyMapper > xRowImpPropMapper;
	UniReference < SvXMLImportPropertyMapper > xTableImpPropMapper;

	const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
	ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

protected:

	// Create a style context.
	virtual SvXMLStyleContext *CreateStyleStyleChildContext(
			sal_uInt16 nFamily,
			sal_uInt16 nPrefix,
			const ::rtl::OUString& rLocalName,
			const ::com::sun::star::uno::Reference< ::com::sun::star::xml::sax::XAttributeList > & xAttrList );

	virtual SvXMLStyleContext *CreateDefaultStyleStyleChildContext(
		sal_uInt16 nFamily, sal_uInt16 nPrefix,
		const ::rtl::OUString& rLocalName,
		const ::com::sun::star::uno::Reference<
			::com::sun::star::xml::sax::XAttributeList > & xAttrList );

//	virtual SvXMLImportPropertyMapper *GetImpPropMapper();

public:

	XMLTableStylesContext( SvXMLImport& rImport, sal_uInt16 nPrfx ,
			const ::rtl::OUString& rLName ,
			const ::com::sun::star::uno::Reference< ::com::sun::star::xml::sax::XAttributeList > & xAttrList,
			const sal_Bool bAutoStyles );
	virtual ~XMLTableStylesContext();

	// Create child element.
/*	virtual SvXMLImportContext *CreateChildContext( sal_uInt16 nPrefix,
		const ::rtl::OUString& rLocalName,
		const ::com::sun::star::uno::Reference<
			::com::sun::star::xml::sax::XAttributeList > & xAttrList );*/

	virtual void EndElement();

	virtual UniReference < SvXMLImportPropertyMapper > GetImportPropertyMapper(
						sal_uInt16 nFamily ) const;
	virtual ::com::sun::star::uno::Reference <
					::com::sun::star::container::XNameContainer >
		GetStylesContainer( sal_uInt16 nFamily ) const;
	virtual ::rtl::OUString GetServiceName( sal_uInt16 nFamily ) const;

	sal_Int32 GetIndex(const sal_Int16 nContextID);
};

class ScXMLMasterStylesContext : public SvXMLStylesContext
{
protected:
	virtual SvXMLStyleContext *CreateStyleChildContext( sal_uInt16 nPrefix,
		const ::rtl::OUString& rLocalName,
		const ::com::sun::star::uno::Reference<
			::com::sun::star::xml::sax::XAttributeList > & xAttrList );

	virtual SvXMLStyleContext *CreateStyleStyleChildContext( sal_uInt16 nFamily,
		sal_uInt16 nPrefix, const ::rtl::OUString& rLocalName,
		const ::com::sun::star::uno::Reference<
			::com::sun::star::xml::sax::XAttributeList > & xAttrList );

	virtual sal_Bool InsertStyleFamily( sal_uInt16 nFamily ) const;

public:
	TYPEINFO();

	ScXMLMasterStylesContext( SvXMLImport& rImport, sal_uInt16 nPrfx,
		const ::rtl::OUString& rLName,
		const ::com::sun::star::uno::Reference<
			::com::sun::star::xml::sax::XAttributeList > & xAttrList);

	virtual ~ScXMLMasterStylesContext();
	virtual void EndElement();
};

namespace com { namespace sun { namespace star {
	namespace style { class XStyle; }
} } }

class ScMasterPageContext : public XMLTextMasterPageContext
{
	com::sun::star::uno::Reference<com::sun::star::beans::XPropertySet> xPropSet;
	const rtl::OUString		sEmpty;
	sal_Bool				bContainsRightHeader;
	sal_Bool				bContainsRightFooter;

	void ClearContent(const rtl::OUString& rContent);
public:

	TYPEINFO();

	ScMasterPageContext( SvXMLImport& rImport, sal_uInt16 nPrfx,
			const ::rtl::OUString& rLName,
			const ::com::sun::star::uno::Reference<
				::com::sun::star::xml::sax::XAttributeList > & xAttrList,
			sal_Bool bOverwrite );
	virtual ~ScMasterPageContext();

	virtual SvXMLImportContext *CreateChildContext(
			sal_uInt16 nPrefix,
			const ::rtl::OUString& rLocalName,
			const ::com::sun::star::uno::Reference< ::com::sun::star::xml::sax::XAttributeList > & xAttrList );

	virtual SvXMLImportContext *CreateHeaderFooterContext(
			sal_uInt16 nPrefix,
			const ::rtl::OUString& rLocalName,
			const ::com::sun::star::uno::Reference< ::com::sun::star::xml::sax::XAttributeList > & xAttrList,
			const sal_Bool bFooter,
			const sal_Bool bLeft );

	virtual void Finish( sal_Bool bOverwrite );
};

class ScCellTextStyleContext : public XMLTextStyleContext
{
    sal_Int32   nLastSheet;

    const ScXMLImport& GetScImport() const { return (const ScXMLImport&)GetImport(); }
    ScXMLImport& GetScImport() { return (ScXMLImport&)GetImport(); }

public:
    ScCellTextStyleContext( SvXMLImport& rImport, sal_uInt16 nPrfx,
            const ::rtl::OUString& rLName,
            const ::com::sun::star::uno::Reference<
                ::com::sun::star::xml::sax::XAttributeList > & xAttrList,
            SvXMLStylesContext& rStyles, sal_uInt16 nFamily,
            sal_Bool bDefaultStyle = sal_False );
    virtual ~ScCellTextStyleContext();

    // overload FillPropertySet to store style information
    virtual void FillPropertySet(
            const ::com::sun::star::uno::Reference<
                ::com::sun::star::beans::XPropertySet > & rPropSet );
};


#endif

