/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2012 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef NATIVECONTROLOBJECT_H_
#define NATIVECONTROLOBJECT_H_

#include "NativeObject.h"
#include <bb/cascades/Control>

/*
 * NativeControlObject
 *
 * Base class for all UI controls
 */

class NativeControlObject : public NativeObject
{
public:
    virtual int setPropertyValue(int propertyNumber, const char* value);

    virtual int setBackgroundColor(const char* text);
    virtual int setLabel(const char* text);
    virtual int setMax(float max);
    virtual int setMin(float min);
    virtual int setText(const char* text);
    virtual int setTextAlign(const char* align);
    virtual int setTop(float top);
    virtual int setValue(float value);

protected:
    NativeControlObject();
    virtual ~NativeControlObject();
    virtual bb::cascades::Control* getControl() const;
    virtual void setControl(bb::cascades::Control* control);

private:
    bb::cascades::Control* control_;
};

#endif /* NATIVECONTROLOBJECT_H_ */
