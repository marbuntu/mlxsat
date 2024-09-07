/**
 * @brief   SOURCE TEMPLATE
 * 
 * @file    _template.cc
 * @author  M. Anschuetz (martin.anschuetz@vert-tec.io)
 * @version 1.0
 * @date    2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "orientation-helper.h"
#include "ns3/vector-extensions.h"
#include "ns3/angles.h"



namespace ns3
{


    Vector Normalize(const Vector &vec)
    {
        double N = 1 / vec.GetLength();
        return Vector(vec.x * N, vec.y * N, vec.z * N);
    }


    double ConvertPhi(const double phi)
    {
        return DegreesToRadians(WrapTo180(phi));
    }


    double ConvertPsi(const double psi)
    {
       return DegreesToRadians(WrapTo180(psi));
    }


    double ConvertTheta(const double theta)
    {
        double a = fmod(theta + 90, 180);
        if (a < 0)
        {
            a += 180;
        }
        a -= 90;

        NS_ASSERT_MSG(-90 <= a && a < 90, "Invalid wrap to [-90, 90[ / a=" << a);

        return DegreesToRadians(a);
    }



    Quaternion::Quaternion()
    {
    }


    Quaternion::~Quaternion()
    {
    }


    Quaternion Quaternion::FromRotation(const Vector &vec, const double theta)
    {
        //NS_LOG_FUNCTION(this << vec << theta);

        Vector nrm = Normalize(vec);
        double half_theta = 0.5 * theta;

        this->x = nrm.x * sin(half_theta);
        this->y = nrm.y * sin(half_theta);
        this->z = nrm.z * sin(half_theta);
        this->w = cos(half_theta);

        return *this;
    }


    Vector Quaternion::RotateVector(const Vector &vec) const
    {
        // Extract the vector part of the quaternion
        Vector u(this->x, this->y, this->z);

        // Extract the scalar part of the quaternion
        double s = this->w;

        // Do the math
        Vector vprime = 2.0 * DotProduct(u, vec) * u + ( s*s - DotProduct(u, u)) * vec + 2.0 * s * CrossProduct(u, vec);


        return vprime;
    }


    std::ostream& operator<<(std::ostream& out, const Quaternion &quat)
    {
        return out << "( w: " << quat.w << " , x: " << quat.x << ", y: " << quat.y << ", z: " << quat.z << " )"; 
    }



    OrientationTransformationHelper::OrientationTransformationHelper()
    {
    }


    OrientationTransformationHelper::~OrientationTransformationHelper()
    {
    }


    void OrientationTransformationHelper::SetAngles(const double phi, const double theta, const double psi)
    {
        m_phi = ConvertPhi(phi);
        m_psi = ConvertPsi(psi);
        m_the = ConvertTheta(theta);
    }


    TypeId LVLHReference::GetTypeId()
    {
        static TypeId tid = TypeId()
            .SetParent<Object>()
            .AddConstructor<LVLHReference>()
        ;
        return tid;
    }


    LVLHReference::LVLHReference()
    {
    }


    LVLHReference::~LVLHReference()
    {
    }


    void LVLHReference::UpdateLocalReference(const Vector &position, const Vector &velocity)
    {
        NS_ASSERT_MSG(velocity.GetLength() > 0, "The Velocity Vector cannot be (0, 0, 0)!");

        if (position.GetLength() == 0)
        {
            m_hr.x = 0;
            m_hr.y = 0;
            m_hr.z = 1;
        }
        else
        {
            m_hr = Normalize(position);
        }

        m_hl = Normalize(CrossProduct(position, velocity));
        m_ht = Normalize(CrossProduct(m_hl, m_hr));
    }

    
    Vector OrientationTransformationHelper::Yaw(const Vector &vec, const Vector &axis) const
    {
        Quaternion quat = Quaternion().FromRotation(axis, m_psi);
        return quat.RotateVector(vec);
    }


    Vector OrientationTransformationHelper::Pitch(const Vector &vec, const Vector &axis) const
    {
        Quaternion quat = Quaternion().FromRotation(axis, m_the);
        return quat.RotateVector(vec);
    }


    Vector OrientationTransformationHelper::Roll(const Vector &vec, const Vector &axis) const
    {
        Quaternion quat = Quaternion().FromRotation(axis, m_phi);
        return quat.RotateVector(vec);
    }


    Vector OrientationTransformationHelper::TransformVector(const Vector &vec, const LVLHReference &ref) const
    {
        Vector imd = vec;
        
        if (m_psi != 0.0)
        {
            imd = Yaw(imd, ref.m_hr);
        }

        if (m_the != 0.0)
        {
            imd = Pitch(imd, ref.m_hl);
        }

        if (m_phi != 0.0)
        {
            imd = Roll(imd, ref.m_ht);
        }

        return imd;
    }


}   /* namespace ns3 */