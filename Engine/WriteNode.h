/* ***** BEGIN LICENSE BLOCK *****
 * This file is part of Natron <http://www.natron.fr/>,
 * Copyright (C) 2013-2017 INRIA and Alexandre Gauthier-Foichat
 *
 * Natron is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Natron is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Natron.  If not, see <http://www.gnu.org/licenses/gpl-2.0.html>
 * ***** END LICENSE BLOCK ***** */

#ifndef Engine_WriteNode_h
#define Engine_WriteNode_h

// ***** BEGIN PYTHON BLOCK *****
// from <https://docs.python.org/3/c-api/intro.html#include-files>:
// "Since Python may define some pre-processor definitions which affect the standard headers on some systems, you must include Python.h before any standard headers are included."
#include <Python.h>
// ***** END PYTHON BLOCK *****

#include "Global/Macros.h"

#if !defined(Q_MOC_RUN) && !defined(SBK_RUN)
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#endif

#include "Engine/NodeGroup.h"

#include "Engine/EngineFwd.h"

NATRON_NAMESPACE_ENTER;

#define kNatronWriteNodeParamEncodingPluginChoice "encodingPluginChoice"
#define kNatronWriteNodeParamEncodingPluginID "encodingPluginID"

#define kNatronWriteParamFrameStep "frameIncr"
#define kNatronWriteParamFrameStepLabel "Frame Increment"
#define kNatronWriteParamFrameStepHint "The number of frames the timeline should step before rendering the new frame. " \
    "If 1, all frames will be rendered, if 2 only 1 frame out of 2, " \
    "etc. This number cannot be less than 1."

#define kNatronWriteParamReadBack "readBack"
#define kNatronWriteParamReadBackLabel "Read back file"
#define kNatronWriteParamReadBackHint "When checked, the output of this node comes from reading the written file instead of the input node"


/**
 * @brief A wrapper around all OpenFX Writers nodes so that to the user they all appear under a single Write node that has a dynamic
 * settings panel.
 * Every method forwards to the corresponding OpenFX plug-in.
 **/
struct WriteNodePrivate;
class WriteNode
    : public NodeGroup
{
GCC_DIAG_SUGGEST_OVERRIDE_OFF
    Q_OBJECT
GCC_DIAG_SUGGEST_OVERRIDE_ON

public:
    /**
     * @brief Returns if the given plug-in is compatible with this WriteNode container.
     * by default all nodes which inherits GenericWriter in OpenFX are.
     **/
    static bool isBundledWriter(const std::string& pluginID);

private: // derives from EffectInstance
    // TODO: enable_shared_from_this
    // constructors should be privatized in any class that derives from boost::enable_shared_from_this<>
    WriteNode(const NodePtr& n);

public:

    static EffectInstancePtr create(const NodePtr& node) WARN_UNUSED_RETURN
    {
        return EffectInstancePtr( new WriteNode(node) );
    }

    static PluginPtr createPlugin() WARN_UNUSED_RETURN;

    virtual ~WriteNode();

    NodePtr getEmbeddedWriter() const;

    void setEmbeddedWriter(const NodePtr& node);

    virtual bool isWriter() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual bool isVideoWriter() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual bool isGenerator() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual bool isOutput() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual bool getCreateChannelSelectorKnob() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual bool isHostChannelSelectorSupported(bool* defaultR, bool* defaultG, bool* defaultB, bool* defaultA) const OVERRIDE WARN_UNUSED_RETURN;
    virtual void onEffectCreated(const CreateNodeArgs& defaultParamValues) OVERRIDE FINAL;
    virtual bool isSubGraphUserVisible() const OVERRIDE FINAL WARN_UNUSED_RETURN
    {
        return false;
    }

    virtual bool isSubGraphPersistent() const OVERRIDE FINAL WARN_UNUSED_RETURN
    {
        return false;
    }

    void renderSequenceStarted();
    void renderSequenceEnd();

    virtual bool isViewAware() const OVERRIDE FINAL WARN_UNUSED_RETURN;

public Q_SLOTS:

    void onSequenceRenderStarted();
    void onSequenceRenderFinished();

private:

    virtual StatusEnum getFrameRange(const TreeRenderNodeArgsPtr& render, double *first, double *last) OVERRIDE FINAL;
    virtual void initializeKnobs() OVERRIDE FINAL;
    virtual void onKnobsAboutToBeLoaded(const SERIALIZATION_NAMESPACE::NodeSerialization& serialization) OVERRIDE FINAL;
    virtual bool knobChanged(const KnobIPtr& k,
                             ValueChangedReasonEnum reason,
                             ViewSetSpec view,
                             TimeValue time) OVERRIDE FINAL;
    boost::scoped_ptr<WriteNodePrivate> _imp;
};

inline WriteNodePtr
toWriteNode(const EffectInstancePtr& effect)
{
    return boost::dynamic_pointer_cast<WriteNode>(effect);
}

NATRON_NAMESPACE_EXIT;

#endif // Engine_WriteNode_h
