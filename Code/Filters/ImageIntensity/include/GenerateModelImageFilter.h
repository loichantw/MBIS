// --------------------------------------------------------------------------------------
// File:          GenerateModelImageFilter.h
// Date:          May 21, 2012
// Author:        code@oscaresteban.es (Oscar Esteban)
// Version:       1.0 beta
// License:       GPLv3 - 29 June 2007
// Short Summary: 
// --------------------------------------------------------------------------------------
//
// Copyright (c) 2012, code@oscaresteban.es (Oscar Esteban)
// with Signal Processing Lab 5, EPFL (LTS5-EPFL)
// and Biomedical Image Technology, UPM (BIT-UPM)
// All rights reserved.
//
// This file is part of MBIS
//
// MBIS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// MBIS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with MBIS.  If not, see <http://www.gnu.org/licenses/>.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#ifndef GENERATEMODELIMAGEFILTER_H_
#define GENERATEMODELIMAGEFILTER_H_

#include <itkImage.h>
#include <itkImageToImageFilter.h>
#include <itkComposeImageFilter.h>

using namespace itk;

namespace mfbs {

template<class TInputVectorImage, class TProbabilityInput>
class GenerateModelImageFilter: public ImageToImageFilter<TInputVectorImage, TInputVectorImage> {
public:
	/** Standard class typedefs */
	typedef GenerateModelImageFilter Self;
	typedef ImageToImageFilter<TInputVectorImage, TInputVectorImage> Superclass;
	typedef SmartPointer<Self> Pointer;
	typedef SmartPointer<const Self> ConstPointer;

	itkStaticConstMacro( Dimension, unsigned int, TInputVectorImage::ImageDimension );

	typedef TInputVectorImage InputVectorImageType;
	typedef typename InputVectorImageType::PixelType MeasurementVectorType;
	typedef typename MeasurementVectorType::ValueType MeasurementType;

	typedef InputVectorImageType OutputImageType;
	typedef typename OutputImageType::Pointer OutputImagePointer;

	typedef itk::Image<MeasurementType, Dimension> InputChannelImageType;

	itkTypeMacro( GenerateModelImageFilter, ImageToImageFilter );
	itkNewMacro( GenerateModelImageFilter );

	/** Extract some information from the image types */
	typedef           TProbabilityInput               ProbabilityImageType;
	typedef typename TProbabilityInput::PixelType ProbabilityPixelType;
	typedef typename TProbabilityInput::Pointer ProbabilityImagePointer;
	typedef typename TProbabilityInput::ConstPointer ProbabilityImageConstPointer;
	typedef typename std::vector<ProbabilityImageConstPointer> ProbabilityVectorType;

	typedef std::vector<MeasurementVectorType> ParametersVectorType;

	void SetNthInput(unsigned int idx, const TProbabilityInput * image) {
		if (idx < m_InputVector.size())
			m_InputVector[idx] = image;
	}

	void SetInput(unsigned int idx, const TProbabilityInput * image) {
		this->SetNthInput(idx, const_cast<TProbabilityInput *>(image));
	}

	void AddInput(TProbabilityInput * image) {
		m_InputVector.push_back(const_cast<TProbabilityInput *>(image));
	}

	void SetInputVector(const ProbabilityVectorType& inputs) {
		typename ProbabilityVectorType::const_iterator end = inputs.end();

		m_InputVector.empty();

		for (typename ProbabilityVectorType::const_iterator it = inputs.begin(); it != end; it++) {
			this->AddInput(*it);
		}
	}

	itkSetMacro( UseBackgroundLabel, bool );
	itkGetConstMacro( UseBackgroundLabel, bool );

	itkGetConstObjectMacro(EstimatedBackground, ProbabilityImageType );

	void SetParameters(ParametersVectorType& p) {
		m_Parameters = p;
	}
	ParametersVectorType& GetParameters() {
		return m_Parameters;
	}

protected:
	GenerateModelImageFilter();
	~GenerateModelImageFilter() {};

	//virtual void PrintSelf(std::ostream& os, Indent indent) const;

	void GenerateData();
	//virtual void GenerateOutputInformation(void);

	virtual void SetNthInput(unsigned int num, DataObject *input) {
		Superclass::SetNthInput(num, input);
	}

private:
	GenerateModelImageFilter(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented

	ProbabilityVectorType m_InputVector;
	ParametersVectorType m_Parameters;
	bool m_UseBackgroundLabel;
	ProbabilityImagePointer m_EstimatedBackground;
};

}

#include "GenerateModelImageFilter.hxx"

#endif /* GENERATEMODELIMAGEFILTER_H_ */
