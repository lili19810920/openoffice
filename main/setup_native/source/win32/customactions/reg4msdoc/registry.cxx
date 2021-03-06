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



#include "registry.hxx"

#include <Shlwapi.h>
#include <assert.h>
#include <algorithm>

#ifdef _MSC_VER
#pragma warning(disable : 4786 4350)
#endif

//-----------------------------------------------------
/** Create instance and open the specified Registry key

	@throws  RegistryWriteAccessDenyException
			 RegistryAccessDenyException
			 RegistryKeyNotFoundException
*/
RegistryKeyImpl::RegistryKeyImpl(HKEY RootKey, const std::wstring& KeyName) :
	m_hRootKey(RootKey),
	m_hSubKey(0),
	m_KeyName(KeyName),
	m_IsWriteable(false)
{
}

//-----------------------------------------------------
/** Create instance and open the specified Registry key

	@throws  RegistryWriteAccessDenyException
			 RegistryAccessDenyException
			 RegistryKeyNotFoundException
*/
RegistryKeyImpl::RegistryKeyImpl(HKEY RootKey) :
	m_hRootKey(RootKey),
	m_hSubKey(0),
	m_IsWriteable(false)
{
}

//-----------------------------------------------------
/** Create an instances of the specified Registry key,
	the key is assumed to be already opened.
*/
RegistryKeyImpl::RegistryKeyImpl(HKEY RootKey, HKEY SubKey, const std::wstring& KeyName, bool Writeable) :
	m_hRootKey(RootKey),
	m_hSubKey(SubKey),
	m_KeyName(KeyName),
	m_IsWriteable(Writeable)
{
}

//-----------------------------------------------------
/**
*/
RegistryKeyImpl::~RegistryKeyImpl()
{
	if (IsOpen())
		Close();
}


//############################################
// Queries
//############################################


//-----------------------------------------------------
/** The name of the key at hand, maybe empty
	if this is any of the root keys
*/
std::wstring RegistryKeyImpl::GetName() const
{
	return m_KeyName;
}

//-----------------------------------------------------
/** 
*/
bool RegistryKeyImpl::IsOpen() const
{
	return m_hSubKey != 0;
}

//-----------------------------------------------------
/** Is this one of the root keys
	HKEY_CLASSES_ROOT
	HKEY_CURRENT_USER
	etc.
*/
bool RegistryKeyImpl::IsRootKey() const
{
	return (0 == m_KeyName.length());
}

//-----------------------------------------------------
/** Do we have write access on the key at hand
*/
bool RegistryKeyImpl::IsWriteable() const
{
	return m_IsWriteable;
}

//-----------------------------------------------------
/** Convenience function to determine if the
	Registry key at hand has the specified 
	value

	@precond IsOpen = true

	throws RegistryAccessDenyException
*/
bool RegistryKeyImpl::HasValue(const std::wstring& Name) const
{
	StringListPtr names = GetSubValueNames();

	StringList::iterator iter_end = names->end();
	StringList::iterator iter = std::find(names->begin(), iter_end, Name);

	return (iter != iter_end);
}

struct CompareNamesCaseInsensitive
{
	CompareNamesCaseInsensitive(const std::wstring& Name) :
		name_(Name)
	{}
	
	bool operator() (const std::wstring& value)
	{
		return (0 == StrCmpI(value.c_str(), name_.c_str()));
	}
	
	std::wstring name_;
};

//-----------------------------------------------------
/** Convenience function to determine if the
	Registry key at hand has the specified 
	sub-key

	@precond IsOpen = true

	throws RegistryAccessDenyException
*/
bool RegistryKeyImpl::HasSubKey(const std::wstring& Name) const
{
	StringListPtr names = GetSubKeyNames();

	StringList::iterator iter_end = names->end();
	StringList::iterator iter = std::find_if(names->begin(), iter_end, CompareNamesCaseInsensitive(Name));

	return (iter != iter_end);
}

//-----------------------------------------------------
/**
*/
void RegistryKeyImpl::Close()
{
	if (RegCloseKey(m_hSubKey) != ERROR_SUCCESS) {
        assert(false);
    }
    
	m_hSubKey = 0;
	m_IsWriteable = false;
}

//-----------------------------------------------------
/** Copies the specified value from RegistryKey to 
	the registry key at hand, if a value with this 
	name already exist under the registry key at hand
	it will be overwritten

	@precond IsOpen = true
			 IsWriteable = true
			 RegistryKey.HasSubValue(Name) = true

	@throws RegistryIOException
			RegistryWriteAccessDeniedException
			RegistryValueNotFoundException
*/
void RegistryKeyImpl::CopyValue(const RegistryKey& RegistryKey, const std::wstring& Name)
{	
	assert(RegistryKey->HasValue(Name));  
#ifdef __MINGW32__
	SetValue((const RegistryValue&)(RegistryKey->GetValue(Name)));    
#else
	SetValue(RegistryKey->GetValue(Name));    
#endif
    assert(HasValue(Name));
}

/** Copies the specified value from RegistryKey to 
	the registry key at hand under a new name, 
	if a value with this name already exist there
	it will be overwritten

	@precond IsOpen = true
			 IsWriteable = true
			 RegistryKey.HasSubValue(Name) = true

	@throws RegistryIOException
			RegistryWriteAccessDeniedException
			RegistryValueNotFoundException
*/
void RegistryKeyImpl::CopyValue(const RegistryKey& RegistryKey, const std::wstring& Name, const std::wstring& NewName)
{
	assert(RegistryKey->HasValue(Name));
    
	RegistryValue RegVal = RegistryKey->GetValue(Name);
	RegVal->SetName(NewName);
	SetValue(RegVal);
    
    assert(HasValue(NewName));
}
