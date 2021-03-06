/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
// Original file Copyright Crytek GMBH or its affiliates, used under license.

#ifndef CRYINCLUDE_CRYCOMMON_IBREAKABLEGLASSSYSTEM_H
#define CRYINCLUDE_CRYCOMMON_IBREAKABLEGLASSSYSTEM_H
#pragma once

struct EventPhysCollision;

struct IBreakableGlassSystem
{
    virtual ~IBreakableGlassSystem() {}

    virtual void    Update(const float frameTime) = 0;
    virtual bool    BreakGlassObject(const EventPhysCollision& physEvent, const bool forceGlassBreak = false) = 0;
    virtual void    ResetAll() = 0;
    virtual void    GetMemoryUsage(ICrySizer* pSizer) const = 0;
};

#endif // CRYINCLUDE_CRYCOMMON_IBREAKABLEGLASSSYSTEM_H
