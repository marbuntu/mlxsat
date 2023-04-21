/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2023
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Martin Anschuetz <anschuet@uni-bremen.de>
 */

#pragma once

#include <ns3/object.h>
#include <ns3/object-base.h>

namespace ns3
{

class PseudoSatTLE : public Object
{
public:

    static TypeId GetTypeId (void);

    TypeId GetInstanceTypeId (void) const;

    /**
     * @brief Construct a new Pseudo TLE Set object
     * 
     */
    PseudoSatTLE();


    /**
     * @brief Destroy the Pseudo Sat TLE object
     * 
     */
    virtual ~PseudoSatTLE()
    {
    }

    std::string TleLine1 ( void );

    std::string TleLine2 ( void );

    std::string TleFull ( void );

private:
    int m_satNo;
    int m_launchYear;
    int m_launchNo;
    int m_epochYear;
    double  m_epochDay;
    double m_inclination;
    double m_raan;          // Right Ascension of Ascending Node
    double m_eccentricty;
    double m_meanAnomaly;
    double m_meanMotion;    // MM
    double m_dtmm;          // 1. Time-derivative of MM
    double m_dt2mm;         // 2. Time-Derivative of MM
    double m_perigee;
    long m_revs;


};  // PseudoSatTLE


}   // namespace ns3