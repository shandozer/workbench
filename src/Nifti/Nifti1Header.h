/*LICENSE_START*/ 
/* 
 *  Copyright 1995-2011 Washington University School of Medicine 
 * 
 *  http://brainmap.wustl.edu 
 * 
 *  This file is part of CARET. 
 * 
 *  CARET is free software; you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation; either version 2 of the License, or 
 *  (at your option) any later version. 
 * 
 *  CARET is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details. 
 * 
 *  You should have received a copy of the GNU General Public License 
 *  along with CARET; if not, write to the Free Software 
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 * 
 */ 

#ifndef NIFTI_HEADER_H
#define NIFTI_HEADER_H

#include <QtCore>
#include "nifti1.h"
#include "iostream"
#include "NiftiException.h"
#include "ByteSwapping.h"
#include <vector>

#define NIFTI1_HEADER_SIZE 348

namespace caret {

/// Simple Container class for storing Nifti1Header data
class Nifti1Header {
public:
   Nifti1Header() throw (NiftiException);

   Nifti1Header(const nifti_1_header &header) throw (NiftiException);
   ~Nifti1Header();
   void getHeaderStruct(nifti_1_header &header) const throw (NiftiException);
   void setHeaderStuct(const nifti_1_header &header) throw (NiftiException);
   QString *getHeaderAsString();
   void initHeaderStruct(nifti_1_header &header);
   void initHeaderStruct();
private:
   nifti_1_header m_header;
};

} // namespace caret

#endif // NIFTI_HEADER_H
