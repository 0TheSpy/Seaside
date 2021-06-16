#ifndef DAR_H
#define DAR_H

#ifdef _WIN32
#pragma once
#endif

#include <stdlib.h>
#include <string.h>
#include "VGUI.h"
#include "utlvector.h"

#include "memdbgon.h"

namespace vgui
{

	template<class ELEMTYPE> class Dar : public CUtlVector< ELEMTYPE >
	{
		typedef CUtlVector< ELEMTYPE > BaseClass;

	public:
		Dar()
		{
		}
		Dar(int initialCapacity) :
			BaseClass(0, initialCapacity)
		{
		}

	public:
		void SetCount(int count)
		{
			this->EnsureCount(count);
		}
		int GetCount()
		{
			return this->Count();
		}
		int AddElement(ELEMTYPE elem)
		{
			return this->AddToTail(elem);
		}
		void MoveElementToEnd(ELEMTYPE elem)
		{
			if (this->Count() == 0)
				return;

			if (this->Element(this->Count() - 1) == elem)
				return;

			int idx = this->GetOffset(elem);
			if (idx == this->InvalidIndex())
				return;

			this->Remove(idx);
			this->AddToTail(elem);
		}
		int FindElement(ELEMTYPE elem)
		{
			return this->GetOffset(elem);
		}
		bool HasElement(ELEMTYPE elem)
		{
			if (this->FindElement(elem) != this->InvalidIndex())
			{
				return true;
			}
			return false;
		}
		int PutElement(ELEMTYPE elem)
		{
			int index = this->FindElement(elem);
			if (index >= 0)
			{
				return index;
			}
			return this->AddElement(elem);
		}
		void InsertElementAt(ELEMTYPE elem, int index)
		{
			this->InsertBefore(index, elem);
		}
		void SetElementAt(ELEMTYPE elem, int index)
		{
			this->EnsureCount(index + 1);
			this->Element(index) = elem;
		}
		void RemoveElementAt(int index)
		{
			this->Remove(index);
		}

		void RemoveElementsBefore(int index)
		{
			if (index <= 0)
				return;
			this->RemoveMultiple(0, index - 1);
		}

		void RemoveElement(ELEMTYPE elem)
		{
			this->FindAndRemove(elem);
		}

		void* GetBaseData()
		{
			return this->Base();
		}

		void CopyFrom(Dar<ELEMTYPE>& dar)
		{
			this->CopyArray(dar.Base(), dar.Count());
		}
	};

}

#include "memdbgoff.h"

#endif  