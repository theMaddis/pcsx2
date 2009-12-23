/*  PCSX2 - PS2 Emulator for PCs
 *  Copyright (C) 2002-2009  PCSX2 Dev Team
 *
 *  PCSX2 is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU Lesser General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  PCSX2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with PCSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "Common.h"
#include "VifDma_internal.h"
#include "newVif.h"

#include <xmmintrin.h>
#include <emmintrin.h>

using namespace x86Emitter;

#ifdef newVif

// --------------------------------------------------------------------------------------
//  VpuUnpackSSE_Base
// --------------------------------------------------------------------------------------
class VpuUnpackSSE_Base
{
public:
	bool			usn;			// unsigned flag
	bool			doMask;			// masking write enable flag
	int				doMode;			// two bit value representing... something!

protected:
	xAddressInfo	dstIndirect;
	xAddressInfo	srcIndirect;

public:
	VpuUnpackSSE_Base();
	virtual ~VpuUnpackSSE_Base() throw() {}

	void xUnpack( int upktype );

protected:
	virtual void doMaskWrite(const xRegisterSSE& regX ) const=0;

	virtual void xMovDest(const xRegisterSSE& srcReg) const;
	virtual void xShiftR(const xRegisterSSE& regX, int n) const;
	virtual void xPMOVXX8(const xRegisterSSE& regX) const;
	virtual void xPMOVXX16(const xRegisterSSE& regX) const;

	virtual void xUPK_S_32() const;
	virtual void xUPK_S_16() const;
	virtual void xUPK_S_8() const;

	virtual void xUPK_V2_32() const;
	virtual void xUPK_V2_16() const;
	virtual void xUPK_V2_8() const;

	virtual void xUPK_V3_32() const;
	virtual void xUPK_V3_16() const;
	virtual void xUPK_V3_8() const;

	virtual void xUPK_V4_32() const;
	virtual void xUPK_V4_16() const;
	virtual void xUPK_V4_8() const;
	virtual void xUPK_V4_5() const;

};

// --------------------------------------------------------------------------------------
//  VpuUnpackSSE_Simple
// --------------------------------------------------------------------------------------
class VpuUnpackSSE_Simple : public VpuUnpackSSE_Base
{
	typedef VpuUnpackSSE_Base _parent;

public:
	int				curCycle;

public:
	VpuUnpackSSE_Simple(bool usn_, bool domask_, int curCycle_);
	virtual ~VpuUnpackSSE_Simple() throw() {}

protected:
	virtual void doMaskWrite(const xRegisterSSE& regX ) const;
};

// --------------------------------------------------------------------------------------
//  VpuUnpackSSE_Dynarec
// --------------------------------------------------------------------------------------
class VpuUnpackSSE_Dynarec : public VpuUnpackSSE_Base
{
	typedef VpuUnpackSSE_Base _parent;

public:
	bool			isFill;

protected:
	const nVifStruct&	v;			// vif0 or vif1
	const nVifBlock&	vB;			// some pre-collected data from VifStruct
	int					vCL;		// internal copy of vif->cl

public:
	VpuUnpackSSE_Dynarec(const nVifStruct& vif_, const nVifBlock& vifBlock_);
	VpuUnpackSSE_Dynarec(const VpuUnpackSSE_Dynarec& src)	// copy constructor
		: _parent(src)
		, v(src.v)
		, vB(src.vB)
	{
		isFill	= src.isFill;
		vCL		= src.vCL;
	}

	virtual ~VpuUnpackSSE_Dynarec() throw() {}

	void CompileRoutine();

protected:
	virtual void doMaskWrite(const xRegisterSSE& regX) const;
	void SetMasks(int cS) const;
	void writeBackRow() const;
	
	static VpuUnpackSSE_Dynarec FillingWrite( const VpuUnpackSSE_Dynarec& src )
	{
		VpuUnpackSSE_Dynarec fillingWrite( src );
		fillingWrite.doMask = true;
		fillingWrite.doMode = 0;
		return fillingWrite;
	}
};
#endif
