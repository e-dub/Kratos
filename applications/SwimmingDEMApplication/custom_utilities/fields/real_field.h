#if !defined(KRATOS_REAL_FIELD_H)
#define KRATOS_REAL_FIELD_H

// /* External includes */

// System includes

// Project includes
#include "includes/variables.h"

/* System includes */
#include <limits>
#include <iostream>
#include <iomanip>

/* External includes */
#ifdef _OPENMP
#include <omp.h>
#endif

/* Project includes */
#include "includes/define.h"
#include "utilities/openmp_utils.h"
#include "real_functions.h"
#include "includes/variables.h"
#include "includes/model_part.h"

namespace Kratos
{
class RealField
{
public:

KRATOS_CLASS_POINTER_DEFINITION(RealField);

/// Default constructor.

RealField(){}

/// Destructor.

virtual ~RealField(){}

//***************************************************************************************************************
//***************************************************************************************************************

virtual void UpdateCoordinates(const double time,
                               const array_1d<double, 3>& coor,
                               const int i_thread = 0){}

virtual void UpdateCoordinates(const double time,
                               const DenseVector<double>& coor,
                               const int i_thread = 0){}


virtual double Evaluate(const double time,
                        const array_1d<double, 3>& coor,
                        const int i_thread = 0)
{
    return 0.0;
}

//***************************************************************************************************************
//***************************************************************************************************************

virtual double CalculateTimeDerivative(const double time,
                                       const array_1d<double, 3>& coor,
                                       const int i_thread = 0)
{
    return 0.0;
}

//***************************************************************************************************************
//***************************************************************************************************************

virtual void CalculateGradient(const double time,
                               const array_1d<double, 3>& coor,
                               array_1d<double, 3>& gradient,
                               const int i_thread = 0)
{
    gradient[0] = 0.0;
    gradient[1] = 0.0;
    gradient[2] = 0.0;
}

//***************************************************************************************************************
//***************************************************************************************************************

virtual void CalculateLaplacian(const double time,
                                const array_1d<double, 3>& coor,
                                array_1d<double, 3>& laplacian,
                                const int i_thread = 0)
{
    laplacian[0] = 0.0;
    laplacian[1] = 0.0;
    laplacian[2] = 0.0;
}

//***************************************************************************************************************
//***************************************************************************************************************

///@}
///@name Inquiry
///@{


///@}
///@name Input and output
///@{

/// Turn back information as a stemplate<class T, std::size_t dim> tring.

virtual std::string Info() const
{
return "";
}

/// Print information about this object.

virtual void PrintInfo(std::ostream& rOStream) const
{
}

/// Print object's data.

virtual void PrintData(std::ostream& rOStream) const
{
}

virtual void ImposeFieldOnNodes(ModelPart& r_model_part, const VariablesList& variables_to_be_imposed){}

///@}
///@name Friends
///@{

///@}

protected:
///@name Protected static Member r_variables
///@{


///@}
///@name Protected member r_variables
///@{ template<class T, std::size_t dim>


///@}
///@name Protected Operators
///@{


///@}
///@name Protected Operations
///@{

// Value

virtual double U(const int i_thread = 0){return 0.0;}

// First-order derivatives

virtual double UDT(const int i_thread = 0){return 0.0;}
virtual double UD0(const int i_thread = 0){return 0.0;}
virtual double UD1(const int i_thread = 0){return 0.0;}
virtual double UD2(const int i_thread = 0){return 0.0;}

// Second-order derivatives
virtual double UDTDT(const int i_thread = 0){return 0.0;}
virtual double UDTD0(const int i_thread = 0){return 0.0;}
virtual double UDTD1(const int i_thread = 0){return 0.0;}
virtual double UDTD2(const int i_thread = 0){return 0.0;}
virtual double UD0D0(const int i_thread = 0){return 0.0;}
virtual double UD0D1(const int i_thread = 0){return 0.0;}
virtual double UD0D2(const int i_thread = 0){return 0.0;}
virtual double UD1D1(const int i_thread = 0){return 0.0;}
virtual double UD1D2(const int i_thread = 0){return 0.0;}
virtual double UD2D2(const int i_thread = 0){return 0.0;}

///@}
///@name Protected  Access
///@{

///@}
///@name Protected Inquiry
///@{


///@}
///@name Protected LifeCycle
///@{


///@}

private:

///@name Static Member r_variables
///@{


///@}
///@name Member r_variables
///@{

///@}
///@name Private Operators
///@{

///@}
///@name Private Operations
///@{


///@}
///@name Private  Access
///@{


///@}
///@name Private Inquiry
///@{


///@}
///@name Un accessible methods
///@{

/// Assignment operator.
RealField & operator=(RealField const& rOther);


///@}

}; // Class RealField

///@}

///@name Type Definitions
///@{


///@}
///@name Input and output
///@{

} // namespace Kratos.
#endif // KRATOS_REAL_FIELD_H
